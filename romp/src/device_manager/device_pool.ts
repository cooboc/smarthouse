


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
    }, {
        id: 8,
        name: "书房走廊",
        type: "Light",
    }, {
        id: 9,
        name: "书房9",
        type: "Light",
    }, {
        id: 10,
        name: "书房8",
        type: "Light",
    }, {
        id: 11,
        name: "厨房水斗",
        type: "Light",
    }, {
        id: 12,
        name: "厨房中央",
        type: "Light",
    }, {
        id: 13,
        name: "1楼客厅主灯",
        type: "Light",
    }, {
        id: 14,
        name: "1楼客厅灯带",
        type: "Light",
    }, {
        id: 15,
        name: "1楼客厅射灯",
        type: "Light",
    }, {
        id: 16,
        name: "玄关",
        type: "Light",
    }
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
