#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
#include <muduo/net/Buffer.h>
#include "httpcontext.h"
#include "httprequest.h"
#include "httpresponse.h"

using namespace std;
using namespace muduo;
using namespace muduo::net;


class HttpServer
{
public:
    //http回调函数
    typedef function<void(const HttpRequest&,HttpResponse*)> HttpCallback;
    //构造、析构函数
    explicit HttpServer(EventLoop* loop,const InetAddress& listenAddr);
    ~HttpServer();

    EventLoop* getLoop() const { return server_.getLoop(); }

    void setHttpCallback(const HttpCallback& cb)
    {
        httpCallback_ = cb;
    }

    void setThreadNum(const int numThreads)
    {
        server_.setThreadNum(numThreads);
    }

    void start();


private:
    void onConnection(const TcpConnectionPtr &conn);

    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime);

    void onRequest(const TcpConnectionPtr &conn,const HttpRequest&);

private:
    TcpServer server_;
    HttpCallback httpCallback_;
};

#endif // HTTPSERVER_H
