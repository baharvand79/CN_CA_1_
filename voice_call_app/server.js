const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 3000 });

let clients = {};

wss.on('connection', (ws) => {
    ws.on('message', (message) => {
        const data = JSON.parse(message);

        switch (data.type) {
            case 'register':
                clients[data.id] = ws;
                break;
            case 'offer':
            case 'answer':
            case 'candidate':
                const targetClient = clients[data.targetId];
                if (targetClient) {
                    targetClient.send(JSON.stringify(data));
                }
                break;
            case 'disconnect':
                delete clients[data.id];
                break;
        }
    });

    ws.on('close', () => {
        for (let id in clients) {
            if (clients[id] === ws) {
                delete clients[id];
                break;
            }
        }
    });
});

console.log('Signaling server running on ws://localhost:3000');
