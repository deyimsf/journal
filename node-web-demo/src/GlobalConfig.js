// 全局配置信息

// mysql
module.exports.dbinfo = {
    connectionLimit : 1,
    host            : '127.0.0.1',
    user            : 'root',
    password        : '123456',
    port            : '3306',
    database        : 'node-web'
}

// 密码md5加盐
module.exports.password_md5_salt = "_node_web_"

// 生成登录token用的加密key
module.exports.login_token_key = "_node_web_"

