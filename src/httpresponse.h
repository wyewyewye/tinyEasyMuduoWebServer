#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <muduo/net/Buffer.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;

class HttpResponse
{
public:
    enum HttpStatusCode
    {
        CODE_UNKNOWN,
        CODE_200 = 200,
        CODE_301 = 301,
        CODE_400 = 400,
        CODE_404 = 404
    };

    explicit HttpResponse(bool close):statusCode_(CODE_UNKNOWN),closeConnection_(close)
    {

    }
    void setStatusCode(HttpStatusCode code);
    void setStatusMessage(const string &message);
    void setCloseConnection(bool on);
    bool closeConnction() const;
    void setContentType(const string &contentType);
    void addHeader(const string &key, const string &value);
    void setBody(const string &body);
    void appendToBuffer(Buffer *output) const;


private:
    //响应头
    unordered_map<string,string> headers_;
    //响应码
    HttpStatusCode statusCode_;
    //状态信息
    string statusMessage_;
    //是否keep_alive
    bool closeConnection_;
    //响应报文
    string body_;
};

#endif // HTTPRESPONSE_H
