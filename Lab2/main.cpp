#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include <errno.h>

int server_port;

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
        //https://blog.csdn.net/daiyudong2020/article/details/51868813
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

            // 返回请求的数据
            request[strlen(request)+1]='\0';
            printf("%s\n",request);

            printf("successeful!\n");
            char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应
            int s = send(connfd,buf,strlen(buf),0);//发送响应
            //printf("send=%d\n",s);
            int fd = open("index.html",O_RDONLY);//消息体
            sendfile(connfd,fd,NULL,2500);//零拷贝发送消息体
            close(fd);
            close(connfd);
        }
    }
	close(socketfd);

}

int main(int argc,char **argv)
{
    //默认参数s
    server_port=8888;

    if(argc < 0)
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