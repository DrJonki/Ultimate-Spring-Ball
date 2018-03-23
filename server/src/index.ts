import './alias';
import * as dotenv from 'dotenv';
import * as net from 'net';
import { each, has } from 'lodash';

dotenv.config();

const socketContainer: any = {};

const handleMessage = (data: any, socket: any) => {
  try {
    // Handle message
  } catch (err) {
    console.error(err);
  }
};

const tcpServer = net.createServer((sock) => {
  const url = `${sock.remoteAddress}:${sock.remotePort}`;
  console.log('TCP connected:', url);

  const sockInstance = sock;

  sock.on('error', (err) => {
    console.error(err);
  }).on('data', (data) => {
    const obj = JSON.parse(data.toString());

    handleMessage(obj, sockInstance);
  }).on('close', () => {
    handleMessage(new Buffer(JSON.stringify({ package: 'disconnect' })), sockInstance);
    delete socketContainer[url];

    console.log('TCP disconnected', url);
  });
}).on('listening', () => {
  console.log('TCP Server listening on', `${tcpServer.address().address}:${tcpServer.address().port}`);
}).on('error', (err) => {
  console.error(err);
}).listen(Number(process.env.SERVER_PORT_TCP), '0.0.0.0');

const handleClose = () => {
  each(socketContainer, (socket) => {
    socket.send('restart', undefined, false);
  });

  tcpServer.close();
  console.log('Closing...');
};

const handleCrash = () => {
  each(socketContainer, (socket) => {
    socket.send('crash', undefined, false);
  });
};

process.on('uncaughtException', () => {
  handleCrash();
});

process.on('unhandledRejection', () => {
  handleCrash();
});

process.on('SIGTERM', () => {
  handleClose();
}).on('SIGINT', () => {
  handleClose();
});