const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 3000 });
let clients = {};

wss.on('connection', (ws) => {
    console.log("[Server] New client connected");

    ws.on('message', (message) => {
        const data = JSON.parse(message);
        console.log(`[Server] Received message of type: ${data.type} from client: ${data.id}`);

        switch (data.type) {
            case 'register':
                clients[data.id] = ws;
                console.log(`[Server] Client registered with ID: ${data.id}`);
                break;
            case 'offer':
                handleOffer(data);
                break;
            case 'answer':
                handleAnswer(data);
                break;
            case 'candidate':
                handleCandidate(data);
                break;
            case 'disconnect':
                handleDisconnect(data.id);
                break;
            default:
                console.log(`[Server] Unknown message type: ${data.type}`);
        }
    });

    ws.on('close', () => {
        removeClient(ws);
    });
});

function handleOffer(data) {
    console.log(`[Server] Forwarding offer from ${data.id} to ${data.targetId}`);
    console.log(`[Server] Offer SDP: ${data.sdp}`);  // Log received SDP
    const offerTarget = clients[data.targetId];
    if (offerTarget) {
        offerTarget.send(JSON.stringify(data));
        console.log(`[Server] Offer sent to ${data.targetId}`);
    } else {
        console.log(`[Server] Target client ${data.targetId} not found`);
    }
}

function handleAnswer(data) {
    console.log(`[Server] Forwarding answer from ${data.id} to ${data.targetId}`);
    console.log(`[Server] Answer SDP: ${data.sdp}`);  // Log received SDP
    const answerTarget = clients[data.targetId];
    if (answerTarget) {
        answerTarget.send(JSON.stringify(data));
        console.log(`[Server] Answer sent to ${data.targetId}`);
    } else {
        console.log(`[Server] Target client ${data.targetId} not found`);
    }
}

function handleCandidate(data) {
    console.log(`[Server] Forwarding candidate from ${data.id} to ${data.targetId}`);
    console.log(`[Server] ICE Candidate: ${data.candidate}`);  // Log received candidate
    const candidateTarget = clients[data.targetId];
    if (candidateTarget) {
        candidateTarget.send(JSON.stringify(data));
        console.log(`[Server] Candidate sent to ${data.targetId}`);
    } else {
        console.log(`[Server] Target client ${data.targetId} not found`);
    }
}

function handleDisconnect(id) {
    delete clients[id];
    console.log(`[Server] Client ${id} disconnected`);
}

function removeClient(ws) {
    for (let id in clients) {
        if (clients[id] === ws) {
            console.log(`[Server] Client ${id} has disconnected`);
            delete clients[id];
            break;
        }
    }
}

console.log('Signaling server running on ws://localhost:3000');
