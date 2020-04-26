#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>   
#include "HttpServer.h"

// const int port = 8887;
const int port = 8888;

string file_path="", request_method="";

void http_get_method(int connfd)
{
    string path_tmp = ".";
    path_tmp += file_path;
    cout << "db db db path_tmp:" << path_tmp <<"\n";

    int fd = open(path_tmp.c_str(),O_RDONLY);//消息体
    if(fd < 0)
    {
        perror("open file error\n");
    }
    else
    {
        printf("db db db: found file\n");
    }
 
    struct stat f_stat;
    stat(path_tmp.c_str(), &f_stat);
    char buf[520]="HTTP/1.1 200 OK\r\n\r\n";//HTTP响应
    write(connfd, buf, strlen(buf));
    sendfile(connfd,fd,NULL,4096);//零拷贝发送消息体
    // int s = send(connfd,buf,strlen(buf),0);//发送响应
    // printf("send_buf_length = %d\n",s);
   
    close(fd);
    close(connfd);
}

//功能划分，为封装类做准备
void http(int connfd)
{
	char request[1024];
    recv(connfd,request,1024,0);

    // 返回请求的数据
    request[strlen(request)+1]='\0';
    printf("%s\n",request);
    printf("successeful!\n");
    
    string str_request = "";
    str_request += request;
    cout << "db str_request:" << str_request <<"\n";
    cout << "I am here!!! \n\n";

    int position = 0;   //字符位置指针
    if((str_request.find("\r\n\r\n"))!=string::npos)
    {
        request_method = "";
        while(str_request[position]!=' ')
        {
            request_method += str_request[position];
            position++;
        }
        cout << "db db db request_method:" << request_method << "\n"; //db 
        position++;
        cout << "db db db position: " << position << "\n";
        file_path = "";
        while(str_request[position]!=' ')
        {
            file_path += str_request[position];
            position++;
        }
        cout << "db db db file_path:" << file_path << "\n";

        if(request_method == "GET")
        {
            cout  << "db db db request_method: GET GET GET " << request_method << "\n";

            http_get_method(connfd);
        }
    }
}

int main(int argc,char *argv[])
{
    if(argc < 0)
    {
        printf("./httpServer --ip ip_address --port port_number [--number-thread thread_number] \n");
        exit(1);
    }
    int socketfd;
    int connfd;
    struct sockaddr_in sever_address;
    bzero(&sever_address,sizeof(sever_address));    //清零
    sever_address.sin_family = AF_INET; 
    sever_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // sever_address.sin_addr.s_addr = htons(INADDR_ANY);  //sin_addr存储IP地址，使用in_addr这个数据结构；s_addr按照网络字节顺序存储IP地址
    sever_address.sin_port = htons(port);   //端口号
 
    socketfd = socket(AF_INET,SOCK_STREAM,0);   //建立TCP socket
    if(socketfd == -1)
    {
        perror("Failed to create a new socket\n");
        exit(errno);
    }
    //************************************************************
    const int reuse = 1;
    if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt error\n");
        return -1;
    }
    //************************************************************
    
    //绑定
    int ret;
    if(ret=bind(socketfd, (struct sockaddr*)&sever_address,sizeof(sever_address)) == -1) 
    {
        perror("Failed to bind on socket\n");
        exit(errno);
    }
 
    //监听
    if(ret=listen(socketfd,1) == -1)
    {
        perror("Failed to listen on socket\n");
        exit(errno);
    }

    //这里可以开始创建线程了

    while(true)
    {
        cout << "db db db I am here here here. Connecting...\n";
        //等待连接
        //https://blog.csdn.net/daiyudong2020/article/details/51868813
        struct sockaddr_in client;
        socklen_t client_addrlength = sizeof(client);
        connfd = accept(socketfd, (struct sockaddr*)&client, &client_addrlength);   //返回连接套接字描述符
        cout << "db db db connfd: " << connfd << "\n";
        if(connfd<0)    //该描述符不为0
        {
            perror("err connfd\n");
        }

        cout << "db db db I am here here here. Connect OKOKOK\n";

        http(connfd);

    }
	close(socketfd);
	
    return 0;
}
