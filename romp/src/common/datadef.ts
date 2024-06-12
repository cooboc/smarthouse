export type GearView = {
    "remote": string,
    "chipId": number,
    "type": number | undefined,
    "typeName": string | undefined
};

export type GearViewListType = GearView[];

export type UpdateGearConfigType = {
    "chipId": number,
    "value": string
};