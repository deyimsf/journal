const mysql = require('mysql');
const GlobalConfig = require('../GlobalConfig');

const mysqlPool = mysql.createPool({
    connectionLimit : GlobalConfig.dbinfo.connectionLimit,
    host            : GlobalConfig.dbinfo.host,
    user            : GlobalConfig.dbinfo.user,
    password        : GlobalConfig.dbinfo.password,
    port            : GlobalConfig.dbinfo.port,
    database        : GlobalConfig.dbinfo.database
});


// 获取链接
module.exports.obtainConnection = ()=> {
    return new Promise((resolve, reject)=> {
        mysqlPool.getConnection(function(err, connection){
            if(err){
                reject(err);
                return;
            }

            resolve(connection);
        });
    });
}

// 执行sql
module.exports.execSql = (sql,params,connection)=> {    
    return new Promise(async (resolve, reject)=>{
        try {
            // 如果connection来自于入参,则执行执行
            if(connection){
                connection.query(sql,params,function (error, results, fields){
                    if(error){
                        reject(error);
                        console.log('执行sql错误',sql,params);
                    }else{
                        // 返回结果
                        resolve(results);
                    }
                });

                return;
            }

            // 如果没有传入connection,则自行获取连接并执行
            const localConn = await this.obtainConnection();
            localConn.query(sql,params,function (error, results, fields){
                if(error){
                    localConn.release();
                    reject(error);
                    console.log('执行sql错误',sql,params);
                }else{
                    // 返回结果
                    resolve(results);
                    localConn.release();
                }
            });

        } catch (error) {
            reject(error);   
        }
    });

}

// 开启事务
module.exports.beginTransaction = (connection) => {
    return new Promise((resolve, reject)=> {
        try {
            connection.beginTransaction(function (err) {
                if (err) {
                    reject(err);
                } else {
                    resolve();
                }
            });
        } catch (error) {
            reject(error);
        }
    });

}

// 事务回滚
module.exports.rollback = (connection) => {
    return new Promise((resolve, reject) => {
        try{
            connection.rollback(function (err) {
                if (err) {
                    reject(err);
                } else {
                    resolve();
                }
            });
        }catch (error) {
            reject(error);
        }
    });
}

// 提交事务
module.exports.commit = (connection) => {
    return new Promise((resolve, reject) => {
        try{
            connection.commit(function (err) {
                if (err) {
                    reject(err);
                }
                
                resolve();
            });
        } catch (error) {
            reject(error);
        }
    });
}