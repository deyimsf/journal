const express = require('express');
const router = express.Router();

const UserService = require('../service/UserService');
const Result = require('../domain/Result');

// 用户注册
router.post("/user/register",async (req,resp,next)=> {
    try{
        const user = req.body.user;
        const result = await UserService.register(user, user.checkPassword);

        // 注册失败
        if (result.code === -1) {
            resp.send(result);
            return;
        }
        
        // 注册成功
        result.code = 200;
        result.message = '注册成功';

        resp.send(result);
    }catch(err){
        const result = new Result();
        result.code = -1;
        result.message = '注册异常';
        resp.send(result);
        console.log(err);
    }
})

// 用户登录
router.post("/user/login",async (req,resp,next)=> {
    try{
        const user = req.body.user;
        const result = await UserService.userLogin(user);
        if (result.code === 1) {
            result.message = '登录成功';

            // TODO    token放到cookie中
            resp.send(result);
            return;
        }

        // 登录失败
        result.code = -2;
        result.message = '登录失败';
        resp.send(result);
    }catch(err){
        const result = new Result();
        result.code = -1;
        result.message = '登录异常';
        resp.send(result);
        console.log(err);
    }
})


module.exports = router;