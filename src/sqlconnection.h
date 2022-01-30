#ifndef SQLCONNECTION_H
#define SQLCONNECTION_H

#include <map>
#include <vector>
#include <string>
#include <muduo/base/Logging.h>
#include <mysql/mysql.h>

using namespace std;
using namespace muduo;

class SqlConnection
{
public:
    SqlConnection(string server = "localhost", string user = "wye", string password = "nimabi123", string database = "license_plate");

    ~SqlConnection();

    bool connectToSqlServer();

    bool query(map<int,vector<string>> &mp, int& totalId);

    bool insert(int id, string license_plate, long long myTime, string ip);

    void disconnectFromSqlServer();
private:
    MYSQL *conn_;
    //
    string server_;
    string user_;
    string password_;
    string database_;
    //未使用
    bool isConnected_;
    //
    MYSQL_RES *res;
    //
    MYSQL_ROW row;
    //
    int cols;
};

#endif // SQLCONNECTION_H
