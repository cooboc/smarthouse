"use client";
import { ReactNode, useEffect, useState } from "react"
import axios from "axios";
import Button from '@mui/material/Button';
import "../globals.css";


// udpate here, and register the callback 
type CoobocDeviceTypeType = "Light";
type DeviceIdType = number;
type CoobocDeviceType = {
    id: DeviceIdType;
    name: string;
    type: CoobocDeviceTypeType;
};
type GearPortType = {
    gearId: number,
    portId: number,
};

type DeviceStatusType = {
    device: CoobocDeviceType,
    gearPort: GearPortType,
    status: number
};


console.log("HOST: ", process.env.ROMP_HOST);
const host: string = process.env.ROMP_HOST as string;
console.log("host: ", host);

const fetchDetailedDeviceList = async function (callback: (data: DeviceStatusType[]) => void) {
    const response = await axios.get("http://10.1.5.5:3000/api/detailed_devices");
    const data = await response.data;
    callback(data);
}

const updateDeviceStatus = async function (req: any) {
    await axios.post("http://10.1.5.5:3000/api/device_status", req);
}


export default function Page() {


    let timer: NodeJS.Timeout;
    const startFetch = (callback: (data: DeviceStatusType[]) => void) => {
        timer = setInterval(() => {
            fetchDetailedDeviceList(callback);
        }, 500);
    }
    const stopFetch = () => {
        clearTimeout(timer);
    }



    const [deviceListState, setDeviceListState] = useState([]);

    useEffect(() => {
        startFetch(setDeviceListState as (data: DeviceStatusType[]) => void);
        return () => {
            stopFetch();
        };
    });

    const deviceList: React.ReactNode[] = deviceListState.map((detailedDevice: DeviceStatusType): ReactNode => {

        let variantTxt: "contained" | "outlined" = (detailedDevice.status > 0) ? "contained" : "outlined";

        const onclick = (evt: any) => {
            console.log(detailedDevice.device.name);
            const targetStatus: number = (detailedDevice.status > 0) ? 0 : 1;
            updateDeviceStatus({ deviceId: detailedDevice.device.id, status: targetStatus });
        }

        return <li key={detailedDevice.device.id} ><Button variant={variantTxt} disabled={(detailedDevice.status < 0)} onClick={onclick}>{detailedDevice.device.name}</Button></li>
    });

    return (<>
        <ol>
            {deviceList}
        </ol>
    </>);
}
