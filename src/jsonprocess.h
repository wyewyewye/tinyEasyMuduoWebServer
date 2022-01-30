#ifndef JSONPROCESS_H
#define JSONPROCESS_H
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Logging.h>
#include <functional>
#include <map>
#include <unordered_set>
#include <jsoncpp/json/json.h>
#include "sqlconnection.h"

using namespace muduo;
using namespace muduo::net;
using namespace std;

class JsonProcess
{
public:
    explicit JsonProcess(EventLoop *loop,const InetAddress& listenAddr, map<int,vector<string>> &mp);
    ~JsonProcess()
    {
        sqlConnection_.disconnectFromSqlServer();
    }

    void start();

private:
    void onConnection(const TcpConnectionPtr &conn);

    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp t);

    void parseJson(Buffer *buf, string ip);

    const map<int,vector<string>>& getJsonMap()
    {
        return jsonMap;
    }

public:
    int totalId;

private:
    TcpServer server_;
    SqlConnection sqlConnection_;
    map<int,vector<string>> &jsonMap;
    unordered_set<string> st;
};

#endif // JSONPROCESS_H
