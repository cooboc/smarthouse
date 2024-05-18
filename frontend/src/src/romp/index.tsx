// Data Definition

type CoobocDevice = {
    name: string;
    id: number;
};

type CoobocDeviceList = CoobocDevice[];

export const getDeviceList = (): Promise<CoobocDeviceList> => {
    return new Promise<CoobocDeviceList>((resolve, reject) => {

    });
}
