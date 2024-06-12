import express from "express";
import cors from "cors";
import { clientPoolViewer, gearServer } from "../gear_pool";
import { devicePool, CoobocDeviceListType } from "src/device_manager/device_pool";
import { deviceManager } from "src/device_manager";
import * as bodyParser from "body-parser";


// ======== Express

const app = express();

app.get("/on_gear", (req: express.Request, res: express.Response): void => {

    console.log(req.query["gearid"]);
    console.log(req.query["portid"]);
    res.send("hello world")

});


//http://localhost:3000/control?id=0&status=1
app.get("/control", (req: express.Request, res: express.Response): void => {

    const deviceId: number = Number(req.query["id"]);
    const statusStr: string = String(req.query["status"]);
    const status: boolean = (statusStr === "on") || (statusStr === "1");

    // the mock mapping
    const mapping = [{
        deviceId: 0,
        gearId: 2090368,
        outputId: 0,
    }, {
        deviceId: 1,
        gearId: 2090368,
        outputId: 1,
    }, {
        deviceId: 2,
        gearId: 2090368,
        outputId: 2,
    },
    {
        // 2楼阳台灯
        deviceId: 3,
        gearId: 1010196,
        outputId: 0,
    }, {
        // 2楼客厅灯
        deviceId: 4,
        gearId: 1010196,
        outputId: 1,
    }, {
        // 楼梯灯
        deviceId: 5,
        gearId: 1010196,
        outputId: 2,
    }, {
        // 卧室主灯
        deviceId: 6,
        gearId: 3288695,
        outputId: 0,
    }, {
        // 卧室灯带
        deviceId: 7,
        gearId: 3288695,
        outputId: 1,
    },
    ];

    if (!isNaN(deviceId)) {
        const matchedDevice = mapping.find((rule) => {
            return rule.deviceId === deviceId;
        });
        if (matchedDevice != undefined) {
            gearServer.requestOutput(matchedDevice.gearId, matchedDevice.outputId, status ? 1 : 0);
        }
    }
    res.status(200).send("OK");
});




type CoobocGearOutput = {
    gearId: number;
    outputId: number;
    otuputType: "Relay";
    status: number;
}



const apiRouter: express.Router = express.Router();
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json())
apiRouter.get("/devices", (req: express.Request, resp: express.Response) => {
    resp.setHeader('Content-Type', 'application/json');
    resp.end(JSON.stringify(devicePool.getDeviceList()));
});

apiRouter.get("/detailed_devices", (req: express.Request, resp: express.Response) => {
    const deviceList: CoobocDeviceListType = devicePool.getDeviceList();

    resp.setHeader('Content-Type', 'application/json');
    resp.end(JSON.stringify(deviceManager.getDeviceGearMapping()));
});

type UpdateDeviceStatusType = {
    deviceId: number,
    status: number
}

apiRouter.post("/device_status", (req: express.Request, resp: express.Response) => {
    const data: UpdateDeviceStatusType = req.body as UpdateDeviceStatusType;
    deviceManager.udpateDeviceStatus(data.deviceId, data.status);
    resp.status(200).send("OK");
})

apiRouter.get("/online_gears", (req: express.Request, resp: express.Response) => {
    resp.setHeader('Content-Type', 'application/json');
    resp.end(JSON.stringify(clientPoolViewer.getGearViewList()));
});


// TODO: change post to get
apiRouter.post("/gear_config", (req: express.Request, resp: express.Response) => {
    const data: UpdateDeviceStatusType = req.body as UpdateDeviceStatusType;

    resp.setHeader('Content-Type', 'application/json');
    resp.end(JSON.stringify({ "TODO": "TODO" }));
})

app.use("/api", cors(), apiRouter);


class WebServer {
    constructor() { }
    readonly begin = (): void => {
        app.listen(3000, (): void => {

        });
    }
};

export const webServer = new WebServer();