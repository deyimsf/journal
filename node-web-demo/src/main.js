const express = require('express');
const server = express();

// 引入中间件
const bodyParser=require("body-parser");
server.use(bodyParser.urlencoded({extended:false}));
server.use(bodyParser.json());


// user controller(路由)
server.use("/",require("./controller/UserController.js"));


// Test controller(路由)
server.use("/",require("./test/TestDaoController.js"));


server.listen(8080, ()=> {
    console.log('启动成功');
})

