#include "jsonprocess.h"
#include <iostream>

JsonProcess::JsonProcess(EventLoop *loop, const InetAddress &listenAddr, map<int,vector<string>> &mp):server_(loop, listenAddr, "JsonProcess"), jsonMap(mp),totalId(0)
{
    server_.setMessageCallback(std::bind(&JsonProcess::onMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));

    server_.setConnectionCallback(std::bind(&JsonProcess::onConnection, this, placeholders::_1));

    sqlConnection_.connectToSqlServer();

    sqlConnection_.query(jsonMap, totalId);

//    for(auto it=jsonMap.begin(); it!=jsonMap.end(); ++it)
//    {
//        st.insert(it->second);
//    }
}

void JsonProcess::start()
{
    LOG_INFO << "HttpServer[" << server_.name() << "] starts listenning on " << server_.ipPort();
    server_.start();
}

void JsonProcess::onConnection(const TcpConnectionPtr &conn)
{
    LOG_INFO << "New JsonProcess Connection: " << conn->peerAddress().toIpPort();
}

void JsonProcess::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp t)
{
    LOG_INFO << "New Json Message";
    parseJson(buf, conn->peerAddress().toIp());
}

void JsonProcess::parseJson(Buffer *buf, string ip)
{
    const char *str = buf->peek();
    //不处理粘包情况　FIX ME:
    string jsonStr(str, str+buf->readableBytes());

    //解析json
    Json::Value jsonRoot;
    Json::Reader jsonReader;
    if(!jsonReader.parse(jsonStr, jsonRoot))
    {
        LOG_WARN << "Json Message is not completed";
        return ;
    }
//    int id = jsonRoot["id"].asInt();    //弃置
    int id = totalId++;
    string license_plate = jsonRoot["license_plate"].asString();
    LOG_INFO << "parse json result:" << id << ":" << license_plate;

    //查重
//    if(st.find(license_plate) != st.end())
//    {
//        LOG_WARN << "license already existed!!";
//        buf->retrieveAll();
//        return ;
//    }
//    else
//        st.insert(license_plate);

    long long myTime = Timestamp::now().secondsSinceEpoch();
    Timestamp t((myTime+28800)*1e6);

    //插入到mysql
    if(!sqlConnection_.insert(id, license_plate, myTime, ip))
    {
        LOG_WARN << "insert to sql fail!!";
        return ;
    }

    //插入到map
    jsonMap[id] = vector<string>{license_plate, t.toFormattedString(), ip};
    LOG_INFO << id << ":" << license_plate;

    buf->retrieveAll();
}
