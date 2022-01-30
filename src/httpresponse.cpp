#include "httpresponse.h"

void HttpResponse::setStatusCode(HttpResponse::HttpStatusCode code)
{
    statusCode_ = code;
}

void HttpResponse::setStatusMessage(const string &message)
{
    statusMessage_ = message;
}

void HttpResponse::setCloseConnection(bool on)
{
    closeConnection_ = on;
}

bool HttpResponse::closeConnction() const
{
    return closeConnection_;
}

void HttpResponse::setContentType(const string &contentType)
{
    addHeader("Content-Type", contentType);
}

void HttpResponse::addHeader(const string &key, const string &value)
{
    headers_[key] = value;
}

void HttpResponse::setBody(const string &body)
{
    body_ = body;
}

void HttpResponse::appendToBuffer(Buffer *output) const
{
    char buf[32];
    //构造响应行
    snprintf(buf, sizeof(buf), "HTTP/1.1 %d ", statusCode_);
    output->append(buf);
    output->append(statusMessage_);
    output->append("\r\n");

    if(closeConnection_)
    {
        output->append("Connection: close\r\n");
    }
    else
    {
        //Keep-Alive需要Content-Length
        snprintf(buf, sizeof(buf), "Content-Length: %zd\r\n", body_.size());
        output->append(buf);
        output->append("Connection: Keep-Alive\r\n");
    }

    for(auto it = headers_.begin(); it != headers_.end(); ++it)
    {
        output->append(it->first);
        output->append(": ");
        output->append(it->second);
        output->append("\r\n");
    }

    output->append("\r\n");
    //响应报文
    output->append(body_);
}


