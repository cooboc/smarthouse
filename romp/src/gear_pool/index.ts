import * as net from 'net';


class Light {
    constructor() {

    }
};

class GearInput {
    constructor() {

    }
};

type GearOutputType = {
    id: number;
    status: number;
};



type RompPacket = {
    version: number,
    chipId: number,
    gearType: number,
    packetSeq: number,
    packetType: number,
    payload: Buffer
};

const ROMP_PROTOCOL_PREFIX: string = "A5";
type NewPacketCallbcak = (packet: RompPacket) => void;
class RompParser {

    private newPacketCallback_: NewPacketCallbcak = () => { }
    constructor() {

    }

    readonly parse = (data: Buffer): void => {
        // 1. check head

        const isHeadCorrect = (data[0] === ROMP_PROTOCOL_PREFIX.charCodeAt(0)) && (data[1] === ROMP_PROTOCOL_PREFIX.charCodeAt(1));
        if (isHeadCorrect && data.length >= 20) {
            const packet: RompPacket = {
                version: data[2],
                chipId: data.readUint32LE(3),
                gearType: data[7],
                packetSeq: data[8],
                packetType: data[9],
                payload: data.subarray(10, 20)
            };
            if ((packet.gearType < 0) || (packet.gearType >= GearGenes.length)) {
                console.error("bad packet, invalid geartype");
            } else {
                this.newPacketCallback_(packet);
            }
        } else {
            console.log("bad packet");
        }
    }

    readonly onNewPacket = (callback: NewPacketCallbcak): void => {
        this.newPacketCallback_ = callback;
    }
};

const PACKET_TYPE_HEARTBEAT: number = 0x01;
const PACKET_TYPE_USER_EVENT: number = 0x02;
const PACKET_TYPE_REQUEST_GEAR: number = 0x03;

interface IGear {
    handlePacket: (packetType: number, payload: Buffer) => void;
    generateOutputPayload: (outputPortId: number, status: number) => Buffer | undefined;
    getOutputStatus: (outputPortId: number) => number;
    setStatus: (outputPortId: number, status: number) => void;
};

class TestGear implements IGear {
    readonly handlePacket = () => { }
    readonly generateOutputPayload = () => { return undefined; }
    readonly getOutputStatus = () => { return -1; }
    readonly setStatus: (outputPortId: number, status: number) => void = (outputPortId: number, status: number): void => { };
}

class Button3Gear implements IGear {
    readonly REQUEST_CHANGE_STATUS: number = 0x01;

    readonly USER_ACTION_TYPE: number = 0x00;

    private readonly chipId_: number;
    private readonly outputs_: GearOutputType[];
    constructor(chipId: number) {
        this.chipId_ = chipId;
        this.outputs_ = [{ id: 0, status: -1 }, { id: 1, status: -1 }, { id: 2, status: -1 }];
    }
    handlePacket: (packetType: number, payload: Buffer) => void = (packetType: number, payload: Buffer) => {
        switch (packetType) {
            case (PACKET_TYPE_HEARTBEAT): {
                const buttonsStatus: number = payload.readUint8(0);
                const relaysStatus: number = payload.readUint8(1);
                this.outputs_.forEach((go: GearOutputType) => {
                    const relayStatus: boolean = (relaysStatus & (0x01 << go.id)) !== 0;
                    go.status = relayStatus ? 1 : 0;
                });
                console.log("Button status, ", this.chipId_, buttonsStatus, relaysStatus);
                break;
            }
            case (PACKET_TYPE_USER_EVENT): {
                const userActionType: number = payload.readUInt8(0);
                const userActionValue: number = payload.readUint8(1);
                const relaysStatus: number = payload.readUint8(2);
                if (userActionType == this.USER_ACTION_TYPE) {
                    console.log("PushDown", userActionValue);
                    this.outputs_.forEach((go: GearOutputType) => {
                        const relayStatus: boolean = (relaysStatus & (0x01 << go.id)) !== 0;
                        go.status = relayStatus ? 1 : 0;
                    });
                } else {
                    console.error("unknown user action: ", payload);
                }
                break;
            }
            default: {
                console.error("unknown packet type", packetType);
                break;
            }
        }
    };

    readonly generateOutputPayload: (outputPortId: number, status: number) => Buffer | undefined = (outputPortId: number, status: number): Buffer | undefined => {
        const output: GearOutputType | undefined = this.outputs_.find((o: GearOutputType) => {
            return o.id === outputPortId;
        });
        if (output == undefined) {
            console.error("output port not found", outputPortId);
            return undefined;
        }
        const buffer: Buffer = Buffer.alloc(10);
        buffer.fill(0);
        buffer[0] = this.REQUEST_CHANGE_STATUS;
        buffer[1] = 1 << output.id;
        buffer[2] = (1 & status) << output.id;
        return buffer;
    };

    readonly getOutputStatus = (outputPortId: number): number => {
        const go: GearOutputType | undefined = this.outputs_.find((go: GearOutputType) => {
            return go.id === outputPortId;
        });
        if (go === undefined) return -1;
        try {
            //throw "3";
        } catch (error) {
            console.trace(error);
        }
        return go.status;
    }

    readonly setStatus: (outputPortId: number, status: number) => void = (outputPortId: number, status: number): void => {
        const output: GearOutputType | undefined = this.outputs_.find((o: GearOutputType) => {
            return o.id === outputPortId;
        });
        if (output != undefined) {
            output.status = status;
        }
    };

};

interface IGearGene {
    makeInstance: (id: number) => IGear;
};
class TestGearGene implements IGearGene {
    makeInstance: () => IGear = (): IGear => {
        return new TestGear();
    }
};
class Button3GearGene implements IGearGene {
    makeInstance: (id: number) => IGear = (id: number): IGear => {
        return new Button3Gear(id);
    }
};
const GearGenes: IGearGene[] = [new TestGearGene(), new Button3GearGene()];


let gearIdGenerator: number = 0;
class Gear {
    private readonly tempId_: number;
    private readonly conn_: net.Socket;
    private readonly parser_: RompParser;
    private readonly inetAddress_: string;
    private chipId_: number = -1;
    private gear_: IGear | undefined = undefined;
    private readonly packetBuffer_: Buffer;
    private packetSeq_: number;


    constructor(conn: net.Socket) {
        this.tempId_ = gearIdGenerator;
        this.conn_ = conn;
        this.parser_ = new RompParser();
        this.inetAddress_ = conn.remoteAddress + ':' + conn.remotePort

        // romp encoder
        this.packetBuffer_ = Buffer.alloc(20);
        this.packetSeq_ = 0;

        gearIdGenerator++;
        this.parser_.onNewPacket(this.onNewPacket);
        this.conn_.on("data", this.parser_.parse);

        this.packetBuffer_.write("A5", 0, 2, "ascii");
        this.packetBuffer_[2] = 0x01;

    }

    private readonly onNewPacket = (packet: RompPacket): void => {
        this.chipId_ = packet.chipId;
        this.getGearInstance(packet.gearType).handlePacket(packet.packetType, packet.payload);
    }

    private readonly getGearInstance = (gearType: number): IGear => {
        if (this.gear_ === undefined) {
            this.gear_ = GearGenes[gearType].makeInstance(this.chipId_);
        }
        return this.gear_;
    }
    readonly getTempId = (): number => { return this.tempId_; }
    readonly getChipId = (): number => { return this.chipId_; }
    readonly setOutput = (outputId: number, status: number) => {
        const buffer: Buffer | undefined = this.gear_?.generateOutputPayload(outputId, status);
        if (buffer === undefined) {
            console.error("CANNot request, bad input");
        } else {
            this.packetBuffer_.writeUint32LE(this.chipId_, 3);
            this.packetBuffer_[8] = this.packetSeq_;
            this.packetSeq_++;
            this.packetSeq_ = this.packetSeq_ & 0x00FF;
            this.packetBuffer_[9] = PACKET_TYPE_REQUEST_GEAR;
            buffer.copy(this.packetBuffer_, 10, 0, 10);
            console.log(buffer);
            console.log(this.packetBuffer_.length, this.packetBuffer_);
            this.conn_.write(this.packetBuffer_, (err: Error | undefined) => {
                console.log("Send OK? ", err);
                if (!err) {
                    (this.gear_ as IGear).setStatus(outputId, status);
                }
            });
        }
    }
    readonly getOutputStatus = (outputId: number): number | undefined => {
        if (this.gear_) {
            return this.gear_.getOutputStatus(outputId);
        } else {
            return undefined;
        }

    }
};

class ClientPool {
    // connection list

    // gear list
    private gearList_: Gear[] = [];


    constructor() {

    }

    private readonly removeGearByTempId = (tempId: number): void => {
        let index: number = -1;
        this.gearList_.forEach((gear: Gear, idx: number) => {
            if (gear.getTempId() == tempId) {
                index = idx;
            }
        });
        if (index >= 0) {
            console.log("ready to remove the element @", index);
            this.gearList_.splice(index, 1);
        }

    }

    readonly add = (conn: net.Socket): void => {
        // Because the heartbeat interval is 2000, so set the timeout to 5000
        conn.setTimeout(5000);
        const gear: Gear = new Gear(conn);
        this.gearList_.push(gear);

        const remoteAddress: string = conn.remoteAddress + ':' + conn.remotePort;
        conn.on("close", (hadError: boolean): void => {
            if (hadError) {
                console.log("Client " + gear.getTempId() + " " + remoteAddress + " closed with Error");
            } else {
                console.log("Client " + gear.getTempId() + " " + remoteAddress + " closed without Error");
            }
            this.removeGearByTempId(gear.getTempId());
        });

        conn.on("end", (): void => {
            console.log("Client " + gear.getTempId() + " " + remoteAddress + " end");
        });

        conn.on("error", (): void => {
            console.log("Client " + gear.getTempId() + " " + remoteAddress + " error");
        });

        conn.on("timeout", (): void => {
            console.log("Client " + gear.getTempId() + " " + remoteAddress + " timeout");
            // If the timeout occur, kill this client
            conn.destroy();
        });
    }


    readonly requestOutput = (gearId: number, outputId: number, status: number): void => {
        // Find the gear 
        const gear: Gear | undefined = this.gearList_.find((gear: Gear) => {
            return gear.getChipId() === gearId;
        });
        if (gear === undefined) {
            console.log("TODO: add callback to tell failed");
        } else {
            gear.setOutput(outputId, status);
        }
    }

    readonly getGearPortStatus = (gearId: number, outputId: number): number | undefined => {
        // Find the gear 
        const gear: Gear | undefined = this.gearList_.find((gear: Gear) => {
            return gear.getChipId() === gearId;
        });
        if (gear === undefined) {
            return undefined;
        } else {
            return gear.getOutputStatus(outputId);
        }
    }
};

const clientPool_ = new ClientPool();

class RompServer {
    private readonly server_: net.Server;

    constructor() {
        this.server_ = net.createServer();


        // setup Server
        this.server_.on("close", (): void => {
            console.log("Server closed");
        });

        this.server_.on("error", (err: Error): void => {
            console.error(err);
        });

        this.server_.on("listening", (): void => {
            console.log("Server listening");
        });

        this.server_.on("drop", (data?: net.DropArgument): void => {
            console.log("Server drop", data);
        });

        this.server_.on("connection", this.onNewClient);
    }

    private readonly onNewClient = (conn: net.Socket): void => {
        const remoteAddress: string = conn.remoteAddress + ':' + conn.remotePort;
        console.log("client come", remoteAddress);
        clientPool_.add(conn);
    }

    readonly begin = (): void => {
        this.server_.listen(8113, () => {
            console.log('server listening to %j', this.server_.address());
        });
    }
};

//     const remoteAddress: string = conn.remoteAddress + ':' + conn.remotePort;
//     conn.on("close",()=>{});
//     console.log("client come", remoteAddress);
//     conn.on("data", (data: Buffer): void => {
//         console.log("data", data);
//     });

//     // let s: boolean = false;
//     // let data: Buffer = Buffer.alloc(2);
//     // setInterval(() => {
//     //     if (s) {
//     //         console.log("set to on");
//     //         data[0] = 0x2;
//     //         data[1] = 0x1;
//     //     } else {
//     //         console.log("set to off");
//     //         data[0] = 0x2;
//     //         data[1] = 0x0;
//     //     }
//     //     conn.write(data);
//     //     s = !s;
//     // }, 2000);
// });






// Central Manager

// A map
/**
 * Gear pool 
 *  - Gear chip id
 *  - Gear button information ...
 * 
 * Virtual Device 
 *  e.g. The light 
 * 
 * 
 * The light -- mapping to --> the Relay on one gear
 * 
 * 1. Need a representation of the physical device
 * 2. Need a representation of Gear device
 * 
 * 
 * 
 */

const TwoFloorLivingLight = {
    // light

    // Output:
    gearId: 2090368,
    outputId: 0,
};

class GearServer {
    constructor() { }

    readonly begin = (): void => {
        const server = new RompServer();
        server.begin();
    }
    readonly requestOutput = clientPool_.requestOutput;

    readonly getGearPortStatus = clientPool_.getGearPortStatus;
};

export const gearServer: GearServer = new GearServer();

