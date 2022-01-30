#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <iostream>
#include <muduo/base/Timestamp.h>
#include <string>
#include <unordered_map>

using namespace std;
using namespace muduo;

class HttpRequest
{
public:
    enum Method
    {
        INVALID,
        GET,
        POST,
        HEAD,
        PUT,
        DELETE
    };
    enum Version
    {
        UNKNOWN,
        HTTP10,
        HTTP11
    };

    HttpRequest();

    void setVersion(Version v);
    Version getVersion() const;

    bool setMethod(const char *start, const char *end);
    Method method() const;
    const char* methodString() const;

    void setPath(const char* start, const char* end);
    const string& path() const;

    void setQuery(const char *start, const char *end);
    const string& query() const;

    void setReceiveTime(Timestamp t);
    Timestamp receiveTime() const;

    void addHeader(const char *start, const char *colon, const char *end);
    string getHeader(const string &field) const;

    const unordered_map<string,string>& headers() const;
    void swap(HttpRequest& that);

private:
    Method method_;
    Version version_;
    string path_;
    string query_;
    Timestamp receiveTime_;
    unordered_map<string,string> headers_;
};

#endif // HTTPREQUEST_H
