#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "httpserver.h"
#include "httpresponse.h"
#include "httprequest.h"
#include "jsonprocess.h"

using namespace muduo;
using namespace muduo::net;
using namespace std;

#if 0
void init_mysql()
{
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char server[] = "localhost";
    char user[] = "wye";
    char password[] = "nimabi123";
    char database[] = "license_plate";

    conn = mysql_init(NULL);

    if(!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        LOG_WARN << mysql_error(conn);
        exit(-1);
    }

    mysql_query(conn, "use license_plate;");

    mysql_query(conn, "set names utf8");

    mysql_query(conn, "select * from cars;");

    res = mysql_store_result(conn);     //深拷贝

    cout << "MYSQL Tables in license_plate:" << endl;

    int cols = mysql_num_fields(res);
    cout << "total columns:" << cols << endl;

    int rows = mysql_num_rows(res);
    cout << "total rows:" << rows << endl;

    for(int i=0;i<cols;++i)
    {
        cout << mysql_fetch_field(res)->name << " ";
    }
    cout << endl;

    while((row = mysql_fetch_row(res)) != NULL)
    {
        for(int i=0;i<cols;++i)
        {
            cout << row[i] << " ";
        }
        cout << endl;
    }

    mysql_free_result(res);
    mysql_close(conn);

    cout << "finished" << endl;
}
#endif

#if 0
void onRequest(const HttpRequest& req, HttpResponse *resp)
{
    LOG_INFO << CurrentThread::tid();
    if(req.method() != HttpRequest::GET)
    {
        resp->setStatusCode(HttpResponse::CODE_400);
        resp->setStatusMessage("Bad Request");
        resp->setCloseConnection(true);
    }
    else
    {
        ifstream inFile;
        string path = req.path();

        int it = path.find('.');
        if(it == string::npos)
        {
            inFile.open("index.html", ios_base::in | ios_base::binary);
            resp->setContentType("text/html");
        }
        else
        {
            string str = path.substr(it+1);
            if(str == "jpg")
            {
                inFile.open("bird.jpg", ios_base::in | ios_base::binary);
                resp->setContentType("image/jpg");
            }
            else if(str == "html")
            {
                inFile.open("index.html", ios_base::in | ios_base::binary);
                resp->setContentType("text/html");
            }
            else
            {
                resp->setStatusCode(HttpResponse::CODE_404);
                resp->setStatusMessage("Not Found");
                resp->setCloseConnection(true);
                return ;
            }

        }

        if(!inFile.is_open())
        {
            resp->setStatusCode(HttpResponse::CODE_404);
            resp->setStatusMessage("Not Found");
            resp->setCloseConnection(true);
            return ;
        }

        ostringstream oss;
        oss << inFile.rdbuf();
        string body(oss.str());


        string body;
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        while(!inFile.eof())
        {
            inFile.read(buf,sizeof(buf));
            body += string(buf,buf+sizeof(buf));
            memset(buf, 0, sizeof(buf));
        }
        inFile.close();

        resp->setBody(body);
        resp->setStatusCode(HttpResponse::CODE_200);
        resp->setStatusMessage("OK");
    }
}
#endif

map<int,vector<string>> globalMap;

void onRequest(const HttpRequest& req, HttpResponse *resp)
{
    if(req.method() != HttpRequest::GET)
    {
        resp->setCloseConnection(true);
        resp->setStatusCode(HttpResponse::CODE_400);
        resp->setStatusMessage("Bad Request");
        return ;
    }

    string body;

    ifstream inFile;
    string path = req.path();
    int it = path.find('.');
    if(it != string::npos)
    {
        inFile.open("beijing.jpg", ios_base::in | ios_base::binary);
        resp->setContentType("image/jpg");

        if(!inFile.is_open())
        {
            resp->setStatusCode(HttpResponse::CODE_404);
            resp->setStatusMessage("Not Found");
            resp->setCloseConnection(true);
            return ;
        }

        char buf[1024];
        memset(buf, 0, sizeof(buf));
        while(!inFile.eof())
        {
            inFile.read(buf,sizeof(buf));
            body += string(buf,buf+sizeof(buf));
            memset(buf, 0, sizeof(buf));
        }
        inFile.close();
    }
    else
    {
        body += "<html><head><meta charset=\"utf8\"><title>车牌识别系统</title></head><body background=\"beijing.jpg\"><center><h1 style=\"color:#97CBFF;font-size:60px;\">车牌识别系统</h1></center><div style=\"width:15%;height:100px;float:left;\"></div><div style=\"width:70%;background-color:rgba(0,0,0,0.5);float:left;\"><table border=\"1\" border-color=\"#97CBFF\" style=\"width:100%;color:white;\"><tr><th>id号</th><th>车牌号码</th><th>时间</th><th>IP地址</th></tr>";
        int idx = 1;
        for(auto it=globalMap.begin();it!=globalMap.end();++it)
        {
            body += "<tr>";
            body += string("<td>") + to_string(idx++) + "</td>";
            body += string("<td>") + (it->second)[0] + "</td>";
            body += string("<td>") + (it->second)[1] + "</td>";
            body += string("<td>") + (it->second)[2] + "</td>";
            body += "</tr>";
        }
        body += "</table></div><div style = \"width:15%;height:100px;float:left;\"></div></body></html>";
        resp->setContentType("text/html");
    }

//    body += "<html><head><meta charset = \"utf8\"><title>车牌识别</title></head><center><h1>车牌系统</h1></center><ol style=\"color:blue\">";
//    for(auto it=globalMap.begin();it!=globalMap.end();++it)
//    {
//        body += string("<li>") + (it->second).first + " " + (it->second).second + "</li>";
//    }
//    body += "</ol><body></body></html>";


    resp->setBody(body);
    resp->setStatusCode(HttpResponse::CODE_200);
    resp->setStatusMessage("OK");
}

int main()
{
    LOG_INFO << "Hello muduo!";

    int numThreads = 2;
    EventLoop loop;

    HttpServer server(&loop, InetAddress("192.168.137.208", 51234));
    server.setHttpCallback(onRequest);
    server.setThreadNum(numThreads);
    server.start();

    JsonProcess jsonProcess(&loop, InetAddress("192.168.137.208", 51233), globalMap);
    jsonProcess.start();

    loop.loop();
    return 0;
}
