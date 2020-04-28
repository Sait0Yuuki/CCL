#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <iostream>

#include "httprequestparser.h"
#include "request.h"
using namespace httpparser;

int server_port;

void POST_method(int connfd, std::string rui)
{
	
}

void NOTFOUND_method(int connfd) //404
{
    char code[]="HTTP/1.1 404 Not Found\r\n";
	char Server[]="My Web Server\r\n";
    char content_type[]="Content-type: text/html\r\n\r\n";
    char end[]="\r\n\r\n";
    char entity1[]="<html><title>404 Not Found</title><body bgcolor=ffffff>\n Not Found\n";
    char file[]="<p>Could not find this file: \n";
    char entity2[]="<hr><em>HTTP Web Server</em>\n</body></html>\n";
    send(connfd,code,strlen(code),0);
	send(connfd,Server,strlen(Server),0);
    send(connfd,content_type,strlen(content_type),0);
    send(connfd,entity1,strlen(entity1),0);
    send(connfd,file,strlen(file),0);
    send(connfd,entity2,strlen(entity2),0);
    send(connfd,end,strlen(end),0);
    close(connfd);
}

//获取文件类型
char *http_get_mime_type(char *file_name) {
  char *file_extension = strrchr(file_name, '.');
  if (file_extension == NULL) {
    return "text/plain";
  }

  if (strcmp(file_extension, ".html") == 0 || strcmp(file_extension, ".htm") == 0) {
    return "text/html";
  } else if (strcmp(file_extension, ".jpg") == 0 || strcmp(file_extension, ".jpeg") == 0) {
    return "image/jpeg";
  } else if (strcmp(file_extension, ".png") == 0) {
    return "image/png";
  } else if (strcmp(file_extension, ".css") == 0) {
    return "text/css";
  } else if (strcmp(file_extension, ".js") == 0) {
    return "application/javascript";
  } else if (strcmp(file_extension, ".pdf") == 0) {
    return "application/pdf";
  } else {
    return "text/plain";
  }
}

void GET_method(int connfd,std::string uri)
{
    char *curi = new char[uri.length() + 1];
    strcpy(curi, uri.c_str());
    curi++; //删除"/"
    int fd;

    if(uri=="/") curi="index.html"; //默认消息体
    fd = open(curi,O_RDONLY);
    if(fd==-1) //找不到文件
    {
        NOTFOUND_method(connfd);
    }

    //printf("send=%d\n",s);
    
    //获取文件大小
    struct stat file;
    stat(curi,&file);
    int length=file.st_size;
    std::string slength = std::to_string(length);

    //构造请求
    char code[]="HTTP/1.1 200 OK\r\n";
    char server[]="Server: Tiny Web Server\r\n";
    char length_head[]="Content-length:";
    char const *clength = slength.c_str(); 
    char type_head[]="\r\nContent-type:";
    char *type=http_get_mime_type(curi);
    char end[]="\r\n\r\n";
    
    //发送响应
    send(connfd,code,strlen(code),0);
    send(connfd,server,strlen(server),0);
    send(connfd,length_head,strlen(length_head),0);
    send(connfd,clength,strlen(clength),0);
    send(connfd,type_head,strlen(type_head),0);
    send(connfd,type,strlen(type),0);
    send(connfd,end,strlen(end),0);

    sendfile(connfd,fd,NULL,2500);

    close(fd);
    close(connfd);
}

void NOT_Implemented(std::string method,int fd)
{
    std::string entity1="<html><title>501 Not Implemented</title><body bgcolor=ffffff>\n Not Implemented\n";
    std::string entity2="<p>Does not implement this method: "+method+"\n<hr><em>HTTP Web Server</em>\n</body></html>\n";
    std::string entity=entity1+entity2;
    std::string str="HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length: "+std::to_string(entity.length())+"\r\n\r\n";
    std::string total=str+entity;
    char buf[512];
    sprintf(buf,"%s",total.c_str());
    write(fd, buf, strlen(buf));
}

void handle_request(char text[1024],int connfd)
{
    Request request;
    HttpRequestParser parser;
    int length=strlen(text);

    HttpRequestParser::ParseResult res = parser.parse(request, text, text + length);

    if( res == HttpRequestParser::ParsingCompleted )
    {
        std::cout<<request.method<<std::endl;
        std::cout<<request.uri<<std::endl;
        //method(GET OR POST)
        //uri(/,/Post_show)
        //content(e.g. Name=HNU&ID=CS06142)

        std::string method=request.method;
        std::string uri=request.uri;
        if(method=="GET")
        {
            GET_method(connfd,uri);
        }
        else if(method!="GET"&&method!="POSt")
        {
            NOT_Implemented(method,connfd);
        }
		else if(method=="POST")
		{
			if(uri != "/Post_show")
			{
				NOTFOUND_method(connfd);
			}
			else
			{
				NOT_Implemented(method,connfd);
			}
			
		}
    }
}

void TCP_connect()
{
    int socketfd;
    int connfd;
    struct sockaddr_in server_address;

    socketfd = socket(AF_INET,SOCK_STREAM,0);   //建立TCP socket
    if (socketfd == -1) {
    perror("Failed to create a new socket");
    exit(errno);
    }
	
	int reuse = 1;
    if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
      perror("setsockopt error\n");
      return ;
    }

    bzero(&server_address,sizeof(server_address));    //清零
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = htons(INADDR_ANY);  //sin_addr存储IP地址，使用in_addr这个数据结构；s_addr按照网络字节顺序存储IP地址
    server_address.sin_port = htons(server_port);   //端口号

    if (bind(socketfd, (struct sockaddr *) &server_address,
        sizeof(server_address)) == -1) {
        perror("Failed to bind on socket");
        exit(errno);
    }

    if (listen(socketfd, 1024) == -1) 
    {
        perror("Failed to listen on socket");
        exit(errno);
    }

    printf("Listening on port %d...\n",server_port);

    while(true)
    {
        //等待连接
        struct sockaddr_in client;
        socklen_t client_addrlength = sizeof(client);
        connfd = accept(socketfd, (struct sockaddr*)&client, &client_addrlength);   //返回连接套接字描述符
        if(connfd<0)    //该描述符不为0
        {
            perror("err connfd\n");
        }
        else
        {
            char request[1024];
            recv(connfd,request,1024,0);
            request[strlen(request)+1]='\0';

            handle_request(request,connfd);

        }
    }
	close(socketfd);

}

int main(int argc,char **argv)
{
    //默认参数s
    server_port=8888;

    if(argc < 5)
    {
        printf("./httpServer --ip ip_address --port port_number [--number-thread thread_number] \n");
        exit(1);
    }

    for(int i=1;i<argc;i++)
    {
        if(strcmp("--port",argv[i])==0)  // --port
        {
            char *server_port_str=argv[++i];
            if(!server_port_str){
                fprintf(stderr, "Expected argument after --port\n");
            }
            server_port=atoi(server_port_str);
        }
        /*else if..*/
    }

    TCP_connect();

    return 0;
}
