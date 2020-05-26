#include<vector>
#include <iostream>


//SET GET DEL 
struct Request{
	std::string method;
	std::vector<std::string> key; //vecctor for DEL
	std::string value;
};

std::string handle_get(Request);

Request resp_parser(std::string);

void coordinator_init(int,std::vector<int>);