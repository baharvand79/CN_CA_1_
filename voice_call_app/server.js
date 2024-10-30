const WebSocket = require('ws');

const wss = new WebSocket.Server({ port: 3000 });

console.log('Signaling server is running on ws://localhost:3000');

const clients = new Map(); // Store clients by their Peer IDs

wss.on('connection', (ws) => {
    console.log('New client connected');

    ws.on('message', (message) => {
        const data = JSON.parse(message);
        const { type, peerId } = data;

        switch (type) {
            case 'register':
                // Register the client
                clients.set(peerId, ws);
                console.log(`Client registered: ${peerId}`);
                break;
            case 'offer':
                // Forward the offer to the intended recipient
                const recipient = clients.get(data.to);
                if (recipient) {
                    recipient.send(JSON.stringify(data));
                    console.log(`Forwarding offer from ${peerId} to ${data.to}`);
                }
                break;
            case 'answer':
                // Forward the answer to the offerer
                const offerer = clients.get(data.to);
                if (offerer) {
                    offerer.send(JSON.stringify(data));
                    console.log(`Forwarding answer from ${peerId} to ${data.to}`);
                }
                break;
            case 'ice-candidate':
                // Forward the ICE candidate to the intended recipient
                const iceRecipient = clients.get(data.to);
                if (iceRecipient) {
                    iceRecipient.send(JSON.stringify(data));
                    console.log(`Forwarding ICE candidate from ${peerId} to ${data.to}`);
                }
                break;
            default:
                console.log('Unknown message type:', type);
        }
    });

    ws.on('close', () => {
        // Remove client from the map on disconnect
        for (const [id, client] of clients.entries()) {
            if (client === ws) {
                clients.delete(id);
                console.log(`Client disconnected: ${id}`);
                break;
            }
        }
    });
});
