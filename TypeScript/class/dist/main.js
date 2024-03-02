"use strict";
const express = require('express');
const server = express();
// import express from 'express';
// const server:express.Application = express();
server.get('/', (req, res) => {
    res.send('Hello World!');
});
const port = 8082;
const host = "0.0.0.0";
server.listen(port, host, () => {
    console.log('server start on: ' + port);
    console.log("for test: http://" + host + ":" + port + "/ ");
});
