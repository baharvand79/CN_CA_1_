const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 3000 });

let clients = {};

wss.on('connection', (ws) => {
        console.log("New client is connected")
    ws.on('message', (message) => {
        const data = JSON.parse(message);
        console.log(`Received message of type: ${data.type} from client: ${data.id}`);

        switch (data.type) {
            case 'register':
                clients[data.id] = ws;
                console.log(`Client registered: ${data.id}`);
                break;
            case 'offer':
                console.log(`Forwarding offer from ${data.id} to ${data.targetId}`);
                const offerTarget = clients[data.targetId];
                if (offerTarget) {
                    offerTarget.send(JSON.stringify(data));
                    console.log(`Offer sent to ${data.targetId}`);
                }
                break;
            case 'answer':
                console.log(`Forwarding answer from ${data.id} to ${data.targetId}`);
                const answerTarget = clients[data.targetId];
                if (answerTarget) {
                    answerTarget.send(JSON.stringify(data));
                    console.log(`Answer sent to ${data.targetId}`);
                }
                break;
            case 'candidate':
                console.log(`Forwarding candidate from ${data.id} to ${data.targetId}`);
                const candidateTarget = clients[data.targetId];
                if (candidateTarget) {
                    candidateTarget.send(JSON.stringify(data));
                    console.log(`Candidate sent to ${data.targetId}`);
                }
                break;
            case 'disconnect':
                delete clients[data.id];
                console.log(`Client disconnected: ${data.id}`);
                break;
        }
    });

    ws.on('close', () => {
        for (let id in clients) {
            if (clients[id] === ws) {
                console.log(`Client ${id} has disconnected`);
                delete clients[id];
                break;
            }
        }
    });
});

console.log('Signaling server running on ws://localhost:3000');
