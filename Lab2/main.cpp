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

const int port = 8888;
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
    sever_address.sin_addr.s_addr = htons(INADDR_ANY);  //sin_addr存储IP地址，使用in_addr这个数据结构；s_addr按照网络字节顺序存储IP地址
    sever_address.sin_port = htons(8888);   //端口号
 
    socketfd = socket(AF_INET,SOCK_STREAM,0);   //建立TCP socket
    assert(socketfd>=0);
    
    //绑定
    int ret = bind(socketfd, (struct sockaddr*)&sever_address,sizeof(sever_address));
    assert(ret != -1);
 
    //监听
    ret = listen(socketfd,1);
    assert(ret != -1);

    //这里可以开始创建线程了

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
	
    return 0;
}