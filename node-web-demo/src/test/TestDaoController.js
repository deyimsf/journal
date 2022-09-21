const express = require('express');
const router = express.Router();

const User = require('../domain/model/User');
const UserDao = require('../dao/UserDao')


// add test 用户controller
router.get("/test/user/add",async (req,resp,next)=> {

    const user = new User();
    user.username = req.query.username;
    user.password = req.query.password;

    const result = await UserDao.addUser(user).catch((err)=>{

        resp.send('注册失败');
    });

    resp.send(result);
})


// getUserByUserName test 用户controller
router.get("/test/user/getUserByUserName",async (req,resp,next)=> {

    const username = req.query.username;
    const result = await UserDao.getUserByUserName(username);

    if (result) {
        resp.send(result);
        return;
    }
    
    resp.send('没有找到username['+username+']');
})


// 根据用户名和密码获取用户信息 getUserByUser test 用户controller
router.get("/test/user/getUserByUser",async (req,resp,next)=> {

    const user = new User();
    user.username = req.query.username;
    user.password = req.query.password;

    const result = await UserDao.getUserByUser(user);
    if (result) {
        resp.send(result);
        return;
    }
    
    resp.send('没有找到username['+user.username+'] password['+user.password+']');
})



module.exports = router;
