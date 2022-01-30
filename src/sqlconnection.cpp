#include "sqlconnection.h"

SqlConnection::SqlConnection(string server, string user, string password, string database):server_(server), user_(user), password_(password), database_(database), isConnected_(false)
{
    conn_ = mysql_init(NULL);
}

SqlConnection::~SqlConnection()
{
    disconnectFromSqlServer();
}

bool SqlConnection::connectToSqlServer()
{
    if(!mysql_real_connect(conn_, server_.c_str(), user_.c_str(), password_.c_str(), database_.c_str(), 0, NULL, 0))
    {
        LOG_WARN << mysql_error(conn_);
        return false;
    }

    if(mysql_query(conn_, "use licensePlate;"))
    {
        LOG_WARN << mysql_error(conn_);
        return false;
    }

    if(mysql_query(conn_, "set names utf8"))
    {
        LOG_WARN << mysql_error(conn_);
        return false;
    }

    if(mysql_query(conn_, "select * from cars;"))
    {
        LOG_WARN << mysql_error(conn_);
        return false;
    }

    res = mysql_store_result(conn_);     //深拷贝

    cols = mysql_num_fields(res);

    mysql_free_result(res);

    LOG_WARN << "Connect to MYSQL";
    return true;
}

bool SqlConnection::query(map<int, vector<string>> &mp, int& totalId)
{
    if(mysql_query(conn_, "SELECT id,license_plate,unix_timestamp(time),ip FROM cars;"))
    {
        LOG_WARN << mysql_error(conn_);
        return false;
    }

    res = mysql_store_result(conn_);     //深拷贝

//    int rows = mysql_num_rows(res);

    while((row = mysql_fetch_row(res)) != NULL)
    {
        int id;
        string license_plate;
        Timestamp myTime;
        string ip;
        for(int i=0;i<cols;++i)
        {
            switch(i)
            {
            case 0:
            {
                id = atoi(row[i]);
                break;
            }
            case 1:
            {
                license_plate = row[i];
                break;
            }
            case 2:
            {
                Timestamp t((atoll(row[i]) + 28800)*10e5);
//                LOG_INFO << t.secondsSinceEpoch()-28800;
                myTime.swap(t);
                break;
            }
            case 3:
            {
                ip = row[i];
                break;
            }
            }
        }
        mp[id] = vector<string>{license_plate, myTime.toFormattedString(), ip};
        totalId = ++id;
    }


    mysql_free_result(res);
    return true;
}

bool SqlConnection::insert(int id, string license_plate, long long myTime, string ip)
{
//    time_t myTime = Timestamp::now().secondsSinceEpoch();

    string sqlInsert = "insert into cars(license_plate,time,ip) values(\"";
    sqlInsert += license_plate;
    sqlInsert += "\",FROM_UNIXTIME(";
    sqlInsert += to_string(myTime);
    sqlInsert += "),\"";
    sqlInsert += ip;
    sqlInsert += "\");";
    if(mysql_query(conn_, sqlInsert.c_str()))
    {
        LOG_WARN << mysql_error(conn_);
        return false;
    }

    return true;
}

void SqlConnection::disconnectFromSqlServer()
{
    mysql_free_result(res);
    mysql_close(conn_);
    LOG_WARN << "Disconnect from sqlserver";
}
