#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include <iostream>
using namespace std;

class HttpServer
{
    private:
        int socketfd;   //套接字文件描述符
    
    public:
        HttpServer(){}
        ~HttpServer(){}

        void http();
        void http_not_found();
        void http_POST();
        void http_GET();
        void http_not_GET_or_POST();
};

#endif