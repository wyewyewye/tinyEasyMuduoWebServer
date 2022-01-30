#ifndef HTTPCONTEXT_H
#define HTTPCONTEXT_H

#include <iostream>
#include <algorithm>
#include <muduo/net/Buffer.h>
#include "httprequest.h"

using namespace std;
using namespace muduo;
using namespace muduo::net;

class HttpContext
{
public:
    enum HttpRequestParseState
    {
        kExpectRequestLine,
        kExpectHeaders,
        kExpectBody,
        kGotAll
    };

    HttpContext():state_(kExpectRequestLine)
    {

    }

    bool parseRequest(Buffer *buf, Timestamp receiveTime);

    bool gotAll() const
    {
        return state_ == kGotAll;
    }

    void reset()
    {
        state_ = kExpectRequestLine;
        HttpRequest dummy;
        request_.swap(dummy);
    }

    const HttpRequest& request() const
    {
        return request_;
    }

private:
    //解析请求行
    bool processRequestLine(const char* begin, const char* end);

private:
    HttpRequestParseState state_;
    //解析结果保存在request_成员中
    HttpRequest request_;
};

#endif // HTTPCONTEXT_H
