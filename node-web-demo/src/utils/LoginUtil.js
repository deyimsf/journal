// 
const GlobalConfig = require('../GlobalConfig');
const jwt = require("jwt-simple");
const LoginTokenInfo = require('../domain/LoginTokenInfo');


// 生成token
module.exports.createToken = (username)=>{
    const tokenInfo = new LoginTokenInfo();
    tokenInfo.exp = Date.now() + (1000 * 60 * 60 * 24);
    tokenInfo.username = username;

    return jwt.encode(tokenInfo, GlobalConfig.login_token_key);
}

// 解密token
module.exports.decodeToken = (loginTokeStr)=>{
    // TODO 解密失败如何操作
    return jwt.decode(loginTokeStr, GlobalConfig.login_token_key);
}

// 校验token


