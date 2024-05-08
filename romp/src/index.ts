import * as net from 'net';

class ClientPool {
    // connection list

    // gear list


    constructor() {

    }

    readonly add = (conn: net.Socket): void => {
        // Because the heartbeat interval is 2000, so set the timeout to 5000
        conn.setTimeout(5000);

        const remoteAddress: string = conn.remoteAddress + ':' + conn.remotePort;
        conn.on("close", (hadError: boolean): void => {
            if (hadError) {
                console.log("Client " + remoteAddress + " closed with Error");
            } else {
                console.log("Client " + remoteAddress + " closed without Error");
            }
            // TODO: remove client from pool
            console.log("TODO");
        });

        conn.on("end", (): void => {
            console.log("Client " + remoteAddress + " end");
        });

        conn.on("error", (): void => {
            console.log("Client " + remoteAddress + " error");
        });

        conn.on("timeout", (): void => {
            console.log("Client " + remoteAddress + " timeout");
            // If the timeout occur, kill this client
            conn.destroy();
        });

        conn.on("data", (data: Buffer): void => {
            console.log("Client " + remoteAddress + " data", data);
        });



    }
};

class RompServer {
    private readonly server_: net.Server;
    private readonly clientPool_: ClientPool;

    constructor() {
        this.server_ = net.createServer();
        this.clientPool_ = new ClientPool();

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
        this.clientPool_.add(conn);
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


const server = new RompServer();
server.begin();
