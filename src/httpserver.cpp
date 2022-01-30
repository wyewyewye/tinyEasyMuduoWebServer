#include "httpserver.h"

void defaultHttpCallback(const HttpRequest&,HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::CODE_400);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop *loop, const InetAddress &listenAddr):server_(loop, listenAddr, "wyeHttpServer"), httpCallback_(defaultHttpCallback)
{
    server_.setConnectionCallback(std::bind(&HttpServer::onConnection, this, placeholders::_1));
    server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));
}

HttpServer::~HttpServer()
{

}

void HttpServer::start()
{
    LOG_WARN << "HttpServer[" << server_.name() << "] starts listenning on " << server_.ipPort();
    server_.start();
}


//新连接回调
void HttpServer::onConnection(const TcpConnectionPtr &conn)
{
    if(conn->connected())
    {
        conn->setContext(HttpContext());
    }
}

//消息回调
void HttpServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime)
{
    HttpContext *context = boost::any_cast<HttpContext>(conn->getMutableContext());
    //解析请求
    if(!context->parseRequest(buf, receiveTime))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    if(context->gotAll())
    {
        //请求解析完毕
        onRequest(conn, context->request());
        context->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr &conn, const HttpRequest &req)
{
    const string &connection = req.getHeader("Connection");
    bool close = connection == "close" || (req.getVersion() == HttpRequest::HTTP10 && connection != "Keep-Alive");
    HttpResponse response(close);//构造响应
    httpCallback_(req, &response);
    Buffer buf;
    //此时response已经构造好，将向客户发送response添加到buffer中
    response.appendToBuffer(&buf);
    conn->send(&buf);
    //如果非Keep-Alive就关闭
    if(response.closeConnction())
    {
        conn->shutdown();
    }
}



