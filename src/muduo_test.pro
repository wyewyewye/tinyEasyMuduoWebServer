TEMPLATE = app
CONFIG += console c++11
CONFIG += static
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    httpserver.cpp \
    httpcontext.cpp \
    httprequest.cpp \
    httpresponse.cpp \
    jsonprocess.cpp \
    sqlconnection.cpp

INCLUDEPATH += /mnt/hgfs/winshare/muduo-master/build/release-install-cpp11/include

LIBS += /mnt/hgfs/winshare/muduo-master/build/release-install-cpp11/lib/libmuduo_base.a

LIBS += /mnt/hgfs/winshare/muduo-master/build/release-install-cpp11/lib/libmuduo_net.a

LIBS += /mnt/hgfs/winshare/muduo-master/build/release-install-cpp11/lib/libmuduo_http.a

LIBS += /mnt/hgfs/winshare/muduo-master/build/release-install-cpp11/lib/libmuduo_cdns.a

LIBS += /mnt/hgfs/winshare/muduo-master/build/release-install-cpp11/lib/libmuduo_curl.a

LIBS += -lpthread -lmysqlclient -ljsoncpp

HEADERS += \
    httpserver.h \
    httpcontext.h \
    httprequest.h \
    httpresponse.h \
    jsonprocess.h \
    sqlconnection.h
