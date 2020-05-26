#include "rpc/client.h"
#include "parser.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "coordinator.h"

//g++ -pthread -std=c++0x -c -o coo coordinator.cpp -I include librpc.a

using namespace std;

void get_response(string value, int conn)
{
    string message;
    if (value == "-1")
        message = "*1\r\n$3\r\nnil\r\n";
    else
    {
        message = "*1\r\n$" + to_string(value.size()) + "\r\n" + value + "\r\n";
    }

    char *cstr = new char[message.length() + 1];
    strcpy(cstr, message.c_str());

    send(conn, cstr, strlen(cstr), 0);
}

void del_response(int num, int conn)
{
    string message = ":" + to_string(num) + "\r\n";

    char *cstr = new char[message.length() + 1];

    strcpy(cstr, message.c_str());
    send(conn, cstr, strlen(cstr), 0);
}

void set_response(int conn)
{
    char message[] = "+OK\r\n";
    send(conn, message, strlen(message), 0);
}

void error_response(int conn)
{
    char message[] = "-ERROR\r\n";
    send(conn, message, strlen(message), 0);
}

bool vote_step(int port)
{
    return 1;
}

//SET1 DONE SET0 ERROR
//"GET"+value GET DONE
//"DEL"+count DEL DONE
//0 ERROR
string commit_step(Request req, int port)
{
    //base:no participant failures  Detect failed connect is unnecessary
    rpc::client client("localhost", port);

    if (req.method == "GET")
    {
        string key = req.key[0];
        string value = client.call("get", key).as<string>();
        return "GET" + value;
    }
    else if (req.method == "SET")
    {
        string key = req.key[0];
        string value = req.value;
        
        //cout<<key<<" "<<value<<endl;

        bool ok = client.call("set", key, value).as<bool>();
        if (ok == 1)
            return "SET1";
        else
            return "SET0";
    }
    else if (req.method == "DEL")
    {
        int count = 0;
        int length = req.key.size();
        //cout<<port<<endl;
        for (int i = 0; i < length; i++)
        {
            count += client.call("del", req.key[i]).as<int>();
            //cout<<count<<endl;
        }
        //cout << "COUNT:" << count << endl;
        return "DEL" + to_string(count);
    }
}

//import message to Request
Request resp_parser(string message)
{
    Request req;
    string result = ParseRESP(message);

    //fetch method
    req.method = result.substr(2, 3);

    //["GET", "CS06142"]
    if (req.method == "GET")
    {
        //fetch key
        string tmp = result.substr(9);
        req.key.push_back(tmp.substr(0, tmp.length() - 2)); //去掉"]
    }
    //["SET", "CS06142", "Cloud", "Computing"]
    else if (req.method == "SET")
    {
        vector<int> comma;
        int pos = 0;
        int i = 0;
        //find every pos of comma
        while ((pos = result.find_first_of(",", pos)) != string::npos)
        {
            comma.push_back(pos);
            pos++;
        }

        //fetch key
        req.key.push_back(result.substr(comma[0] + 3, comma[1] - comma[0] - 4));
        //fetch value
        string tmp; //构造value
        for (int i = 1; i < comma.size(); i++)
        {
            if (i != comma.size() - 1) //逗号中间部分同key
            {
                tmp += result.substr(comma[i] + 3, comma[i + 1] - comma[i] - 4);
                tmp += ' ';
            }
            else //最后一个value特殊处理
            {
                tmp += result.substr(comma[i] + 3);
                req.value = tmp.substr(0, tmp.length() - 2);
            }
        }
    }

    else if (req.method == "DEL")
    {
        vector<int> comma;
        int pos = 0;
        int i = 0;
        //find every pos of comma
        while ((pos = result.find_first_of(",", pos)) != string::npos)
        {
            comma.push_back(pos);
            pos++;
        }

        //fetch key
        for (int i = 0; i < comma.size(); i++)
        {
            if (i != comma.size() - 1)
            {
                req.key.push_back(result.substr(comma[i] + 3, comma[i + 1] - comma[i] - 4));
            }
            else //最后一个key特殊处理
            {
                string tmp = result.substr(comma[i] + 3);
                req.key.push_back(tmp.substr(0, tmp.length() - 2));
            }
        }
    }
    // test
    cout << result << endl;
    // cout << req.method << endl;
    // for (int i = 0; i < req.key.size(); i++)
    //     cout << req.key[i] << ' ';
    // cout << endl;
    // cout << req.value << endl;
    // cout << "------------------" << endl;

    return req;
}

void handle_request(vector<int> partPort, string message, int conn)
{
    Request req = resp_parser(message);
    //for each participant
    string result;
    for (int i = 0; i < partPort.size(); i++)
    {
        //cout<<partPort.size()<<endl;
        bool ifCoomit = vote_step(partPort[i]);

        if (ifCoomit)
            result += commit_step(req, partPort[i]);
        
        //cout<<result<<endl;

        //if request is GET, get from one participant is enough
        if (result.find("GET") != string::npos)
        {
            string tmp = result.substr(3);
            get_response(tmp, conn);
            break;
        }

    }
    //request is SET

    int size=partPort.size();

    //SET response
    if(result.find("SET1")!=string::npos && result.find("GET")==string::npos)
    {
        int setCount=count(result.begin(),result.end(),'1');
        //cout<<setCount<<endl;
        if (setCount==size)
        {
            set_response(conn); //no error
        }
        else error_response(conn);
    }

    //DEL response
    if(result.find("DEL")!=string::npos && result.find("GET")==string::npos)
    {
        int pos=result.find_last_of("L");
        string snum=result.substr(pos+1);
        
        int num=stoi(snum);
        del_response(num,conn);
        //else error_response(conn);
    }

}

void coordinator_init(int cooPort, vector<int> partPort)
{
    // socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        std::cout << "Error: socket" << std::endl;
        return;
    }

    // bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(cooPort);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        std::cout << "Error: bind" << std::endl;
        return;
    }
    // listen
    if (listen(listenfd, 5) == -1)
    {
        std::cout << "Error: listen" << std::endl;
        return;
    }

    // accept
    int conn;
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    while (true)
    {
        std::cout << "...listening" << std::endl;
        conn = accept(listenfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (conn < 0)
        {
            std::cout << "Error: accept" << std::endl;
            continue;
        }
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "...connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

        char buf[1024];
        while (true)
        {
            memset(buf, 0, sizeof(buf));
            int len = recv(conn, buf, sizeof(buf), 0);
            buf[len] = '\0';

            string message(buf);
            handle_request(partPort, message, conn);

            //send(conn, buf, len, 0);
        }

        close(conn);
    }
    close(listenfd);
}
