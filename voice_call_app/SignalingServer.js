const express = require('express');
const http = require('http');
const socketIo = require('socket.io');

const app = express();
const server = http.createServer(app);
const io = socketIo(server);

io.on('connection', (socket) => {
    console.log('A user connected:', socket.id);

    socket.on('offer', (offer, room) => {
        socket.to(room).emit('offer', offer);
    });

    socket.on('answer', (answer, room) => {
        socket.to(room).emit('answer', answer);
    });

    socket.on('ice-candidate', (candidate, room) => {
        socket.to(room).emit('ice-candidate', candidate);
    });

    socket.on('join-room', (room) => {
        socket.join(room);
        console.log(`User joined room: ${room}`);
    });

    socket.on('disconnect', () => {
        console.log('User disconnected:', socket.id);
    });
});

server.listen(3000, () => {
    console.log('Server is running on port 3000');
});
