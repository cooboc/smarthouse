import { DeviceIdType, CoobocDeviceListType, devicePool, CoobocDeviceType } from "./device_pool";
import { gearServer } from "src/gear_pool";



function cloneAny<T>(obj: T): T {
    const copiedObj = {} as T;
    for (const key in obj) {
        if (typeof obj[key] === 'object') {
            // Recursively copy nested objects
            copiedObj[key] = cloneAny(obj[key]);
        } else {
            // Shallow copy primitive values
            copiedObj[key] = obj[key];
        }
    }
    return copiedObj;
}


type GearPortType = {
    gearId: number,
    portId: number,
};

type DeviceStatusType = {
    device: CoobocDeviceType,
    gearPort: GearPortType,
    status: number
};

class DeviceManager {

    private readonly deviceGearMap_: Map<DeviceIdType, GearPortType>;

    constructor() {
        this.deviceGearMap_ = new Map<DeviceIdType, GearPortType>([
            [0, { gearId: 2090368, portId: 0 }],//dfa
            [1, { gearId: 2090368, portId: 1 }],//test
            [2, { gearId: 2090368, portId: 2 }],//test
            [3, { gearId: 1010196, portId: 0 }],// 2楼阳台灯
            [4, { gearId: 1010196, portId: 1 }],// 2楼客厅灯
            [5, { gearId: 1010196, portId: 2 }],// 楼梯灯
            [6, { gearId: 3288695, portId: 0 }],// 卧室主灯
            [7, { gearId: 3288695, portId: 1 }],// 卧室灯带
            [8, { gearId: 2664687, portId: 0 }],// 书房走廊
            [9, { gearId: 2664687, portId: 1 }],// 书房9
            [10, { gearId: 2664687, portId: 2 }],// 书房8
            [11, { gearId: 7765413, portId: 0 }],// 厨房水斗
            [12, { gearId: 7765413, portId: 1 }],// 厨房中央

        ]);
    }
    readonly getDeviceGearMapping = (): DeviceStatusType[] => {
        const ret: DeviceStatusType[] = [];

        this.deviceGearMap_.forEach((value: GearPortType, key: DeviceIdType) => {
            const device: CoobocDeviceType | undefined = devicePool.getDeviceById(key);
            if (device === undefined) {
                console.error("device not found in mapping", key);
            } else {
                let status: number | undefined = gearServer.getGearPortStatus(value.gearId, value.portId);
                if (status === undefined) {
                    status = -1;
                }
                const item: DeviceStatusType = {
                    device: device,
                    gearPort: value,
                    status: status
                }
                ret.push(item);
            }
        });

        return ret;
    }

    readonly udpateDeviceStatus = (deviceId: DeviceIdType, status: number): void => {
        if (this.deviceGearMap_.has(deviceId)) {
            const gearPort: GearPortType = this.deviceGearMap_.get(deviceId) as GearPortType;
            gearServer.requestOutput(gearPort.gearId, gearPort.portId, status);
        }
    }
};

export const deviceManager = new DeviceManager();
