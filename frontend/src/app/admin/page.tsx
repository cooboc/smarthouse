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


let PROD_SERVER:string = "http://10.1.99.60:3000";
if ((process.env.NODE_ENV as string) === "production") {
    PROD_SERVER = "http://10.1.5.5:3000";
}

// let PROD_SERVER = "http://10.1.5.5:3000";
// if (process.env.ROMP_HOST) {
//     PROD_SERVER = process.env.ROMP_HOST as string;
// }

// console.log("process.env.NODE_ENV", process.env.NODE_ENV);
// console.log("process.env.NEXT_PUBLIC_SERVER", process.env.NEXT_PUBLIC_SERVER);

// console.log("HOST: ", process.env.ROMP_HOST);
// const host: string = process.env.ROMP_HOST as string;
// console.log("host: ", host);

// console.log("server = " + PROD_SERVER);
const fetchDetailedDeviceList = async function (callback: (data: DeviceStatusType[]) => void) {
    const response = await axios.get(PROD_SERVER+"/api/detailed_devices");
    const data = await response.data;
    callback(data);
}

const updateDeviceStatus = async function (req: any) {
    await axios.post(PROD_SERVER+"/api/device_status", req);
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
