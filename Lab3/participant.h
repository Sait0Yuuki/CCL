#include <iostream>
#include "coordinator.h"
#include <map>

using std::vector;
using std::string;
using std::map;

struct Participant{
    vector<Request> log;
    map<string,string> data;

    string get_value(string key)
    {
        std::cout<<"GET "<<key<<std::endl;
        
        map<string, string>::iterator iter;
        iter=data.find(key);
        if(iter!=data.end()) return iter->second;
        return "-1";
    }

    bool set_value(string key, string value)
    {
        std::cout<<"SET "<<key<<" "<<value<<std::endl;
        data[key]=value;  
        return true;
    }

    int del_value(string key)
    {
        std::cout<<"DEL "<<key<<std::endl;

        map<string, string>::iterator iter;
        iter=data.find(key);
        if(iter==data.end()) return 0;
        else
        {
            data.erase(iter);
            return 1;
        }
    }
};



void participant_init(vector<int>);