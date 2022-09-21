const UserDao = require('../dao/UserDao')
const User = require('../domain/model/User');
const Result = require('../domain/Result');
const PasswordUtil = require('../utils/PasswordUtil');
const LoginUtil = require('../utils/LoginUtil');


// 用户注册
module.exports.register = async (user,checkPassword)=> {
    const result = new Result();


    // 检查两次密码是否一致
    if (user.password != checkPassword) {
        result.code = -1;
        result.message = '两次密码不一致'
        return result;
    }

    // 查询该用户是否存在
    const localusername = await UserDao.getUserByUserName(user.username);
    if (localusername) {
        result.code = -1;
        result.message = '该用户名已存在'
        return result;
    }   
    
    // 注册
    // 密码加密
    user.password = PasswordUtil.encode(user.password);
    const flag = await UserDao.addUser(user);
    if(flag){
        return result;
    }
}



// 用户登录,成功后返回token
module.exports.userLogin = async (user)=> {
    const result = new Result();

    // 查询用户信息是否存在
    user.password = PasswordUtil.encode(user.password);
    const flag = await UserDao.getUserByUser(user);
    if(flag){
        result.code = 1;
        result.message = '登录成功';

        // 生成token
        result.data = LoginUtil.createToken(user.username);
        return result;
    }

    // 登录失败
    result.code = -1;
    result.message = '登录失败';

    return result;
}

