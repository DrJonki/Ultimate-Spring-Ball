"use strict";
exports.__esModule = true;
require("./alias");
var dotenv = require("dotenv");
var net = require("net");
var lodash_1 = require("lodash");
dotenv.config();
var socketContainer = {};
var handleMessage = function (data, socket) {
    try {
        // Handle message
    }
    catch (err) {
        console.error(err);
    }
};
var tcpServer = net.createServer(function (sock) {
    var url = sock.remoteAddress + ":" + sock.remotePort;
    console.log('TCP connected:', url);
    var sockInstance = sock;
    sock.on('error', function (err) {
        console.error(err);
    }).on('data', function (data) {
        var obj = JSON.parse(data.toString());
        handleMessage(obj, sockInstance);
    }).on('close', function () {
        handleMessage(new Buffer(JSON.stringify({ package: 'disconnect' })), sockInstance);
        delete socketContainer[url];
        console.log('TCP disconnected', url);
    });
}).on('listening', function () {
    console.log('TCP Server listening on', tcpServer.address().address + ":" + tcpServer.address().port);
}).on('error', function (err) {
    console.error(err);
}).listen(Number(process.env.SERVER_PORT_TCP), '0.0.0.0');
var handleClose = function () {
    lodash_1.each(socketContainer, function (socket) {
        socket.send('restart', undefined, false);
    });
    tcpServer.close();
    console.log('Closing...');
};
var handleCrash = function () {
    lodash_1.each(socketContainer, function (socket) {
        socket.send('crash', undefined, false);
    });
};
process.on('uncaughtException', function () {
    handleCrash();
});
process.on('unhandledRejection', function () {
    handleCrash();
});
process.on('SIGTERM', function () {
    handleClose();
}).on('SIGINT', function () {
    handleClose();
});
