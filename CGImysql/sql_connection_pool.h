#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "../lock/locker.h"
#include "../log/log.h"

using namespace std;
/**
 * Database pool
 */
class connection_pool {
public:
    connection_pool();
    ~connection_pool();
    MYSQL *GetConnection();                 //获取数据库连接
    bool ReleaseConnection(MYSQL *conn);    //释放连接
    int GetFreeConn();                      //获取连接
    void DestroyPool();                     //销毁所有连接

    /**
     * @brief Create a database pool as singleton
     * @return
     */
    static connection_pool *GetInstance();

    /**
     * @brief Initialize the database pool and build the connection of database
     * @param url
     * @param User
     * @param PassWord
     * @param DataBaseName
     * @param Port
     * @param MaxConn
     * @param close_log
     */
    void init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn, int close_log);

    string m_url;             //主机地址
    string m_Port;            //数据库端口号
    string m_User;            //登陆数据库用户名
    string m_PassWord;        //登陆数据库密码
    string m_DatabaseName;    //使用数据库名
    int m_close_log;          //日志开关

private:
    int m_MaxConn;  //最大连接数
    int m_CurConn;  //当前已使用的连接数
    int m_FreeConn; //当前空闲的连接数
    locker lock;
    list<MYSQL *> connList; //连接池
    sem reserve;
};

/**
 * A class that has sql_pool and connection
 */

class connectionRAII {

public:
    connectionRAII(MYSQL **con, connection_pool *connPool);

    ~connectionRAII();

private:
    MYSQL *conRAII;
    connection_pool *poolRAII;
};

#endif
