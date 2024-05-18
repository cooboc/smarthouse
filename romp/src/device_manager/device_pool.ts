


// type def

export type CoobocDeviceTypeType = "Light";
export type DeviceIdType = number;
export type CoobocDeviceType = {
    id: DeviceIdType;
    name: string;
    type: CoobocDeviceTypeType;
};

export type CoobocDeviceListType = CoobocDeviceType[];

const coobocDeviceList_: CoobocDeviceListType = [
    {
        id: 0,
        name: "test-0",
        type: "Light",
    }, {
        id: 1,
        name: "test-1",
        type: "Light",
    }, {
        id: 2,
        name: "test-2",
        type: "Light",
    }, {
        id: 3,
        name: "2楼阳台灯",
        type: "Light",
    }, {
        id: 4,
        name: "2楼客厅灯",
        type: "Light",
    }, {
        id: 5,
        name: "楼梯灯",
        type: "Light",
    }, {
        id: 6,
        name: "卧室主灯",
        type: "Light",
    }, {
        id: 7,
        name: "卧室灯带",
        type: "Light",
    },
];

class DevicePool {
    constructor() { }
    readonly getDeviceList = (): CoobocDeviceListType => {
        return coobocDeviceList_;
    }
    readonly getDeviceById = (id: DeviceIdType): CoobocDeviceType | undefined => {
        return coobocDeviceList_.find((item: CoobocDeviceType) => {
            return item.id === id;
        });
    }
};

export const devicePool = new DevicePool();
