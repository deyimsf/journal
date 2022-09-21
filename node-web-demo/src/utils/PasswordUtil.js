// 密码加密工具

const md5 = require("md5");
const GlobalConfig = require('../GlobalConfig');

module.exports.encode = (password)=>{
    return md5(password+GlobalConfig.password_md5_salt);
}