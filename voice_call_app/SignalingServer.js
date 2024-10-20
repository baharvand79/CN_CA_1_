const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
//const io = new Server(server);

//app.get('/', (req, res) => {
//  res.sendFile(__dirname + '/index.html');
//});

//io.on('connection', (socket) => {
//  console.log('a user connected');
//});

//server.listen(3000, () => {
//  console.log('listening on *:3000');
//});

const io = require('socket.io')(3000);

io.on('connection', socket => {
    console.log('New peer connected:', socket.id);

    // Handle SDP exchange
    socket.on('sdp', ({peerId, sdp}) => {
        console.log('Received SDP from peer:', peerId);
        socket.broadcast.emit('sdp', {peerId, sdp});
    });

    // Handle ICE candidate exchange
    socket.on('candidate', ({peerId, candidate, sdpMid}) => {
        console.log('Received ICE candidate from peer:', peerId);
        socket.broadcast.emit('candidate', {peerId, candidate, sdpMid});
    });
});
