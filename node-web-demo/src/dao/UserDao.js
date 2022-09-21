const DataSource = require('../dao/DataSource');
const User = require('../domain/model/User');



// 添加一个用户
module.exports.addUser = async (user,connection)=> {
    const sql = "insert into user(username,password,created,updated) values (?,?,now(),now());";

    const results = await DataSource.execSql(sql,[user.username, user.password],connection).catch((err)=>{
        console.log("添加用户失败!",err);
        throw err;
    });

    return results;
}

// 根据用户名获取用户信息
module.exports.getUserByUserName = async (username,connection)=> {
    const sql = "select * from user where username = ?;";

    const results = await DataSource.execSql(sql,[username],connection).catch((err)=>{
        console.log("根据用户名获取用户信息失败",err);
        throw err;
    });

    return results[0];
}


// 根据用户名和密码获取用户信息
module.exports.getUserByUser = async (user,connection)=> {
    const sql = "select * from user where username = ? and password = ?;";

    const results = await DataSource.execSql(sql,[user.username, user.password],connection).catch((err)=>{
        console.log("根据用户名和密码获取用户信息失败",err);
        throw err;
    });

    return results[0];
}
