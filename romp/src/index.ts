import * as net from'net';

const server : net.Server = net.createServer();
server.on("connection", (conn: net.Socket):void => {
    const remoteAddress:string = conn.remoteAddress +':' + conn.remotePort;
    console.log("client come", remoteAddress);
    conn.on("data", (data:Buffer):void => {
        console.log("data", data);
    });

    let s:boolean = false;
    let data:Buffer = Buffer.alloc(2);
    setInterval(() => {
        if (s) {
            console.log("set to on");
            data[0] = 0x2;
            data[1] = 0x1;
        } else {
            console.log("set to off");
            data[0] = 0x2;
            data[1] = 0x0;
        }
        conn.write(data);
        s = !s;
    }, 2000);
});
server.listen(8113, () => {
    console.log('server listening to %j', server.address());
});
