#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int port;
    std::cout << "This is client" << std::endl;
    std::cout <<"port:";
    std::cin >>port;
    // socket
    int client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1) {
        std::cout << "Error: socket" << std::endl;
        return 0;
    }
    // connect
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(client, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Error: connect" << std::endl;
        return 0;
    }
    std::cout << "...connect" << std::endl;
    char data0[255]="*2\r\n$3\r\nGET\r\n$7\r\nCS06142\r\n";
    char data1[255]="*3\r\n$3\r\nSET\r\n$7\r\nCS06142\r\n$5\r\nCloud\r\n";
    char data2[255]="*3\r\n$3\r\nDEL\r\n$7\r\nCS06142\r\n$5\r\nCS162\r\n";
    char buf[255];
    while (true) {
        std::string go;
        std::cin>>go;
        if(go=="GET")
        {
            send(client, data0, strlen(data0), 0);
            go="nope";
        }
        else if(go=="SET")
        {
            send(client, data1, strlen(data1), 0);
            go="nope";
        }
        else if(go=="DEL")
        {
            send(client, data2, strlen(data2), 0);
            go="nope";
        }
        memset(buf, 0, sizeof(buf));
        int len = recv(client, buf, sizeof(buf), 0);
        buf[len] = '\0';
        std::cout << buf << std::endl;
    }
    close(client);
    return 0;
}