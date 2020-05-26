#include "participant.h"
#include "rpc/server.h"



void participant_init(vector<int> port)
{
    Participant part;
    //part.data.insert(std::pair<string,string>("CS06142","Hello!"));
    rpc::server srv(port[0]);

    srv.bind("get",[&part](string key){return part.get_value(key);});
    srv.bind("set",[&part](string key,string value){return part.set_value(key,value);});
    srv.bind("del",[&part](string key){return part.del_value(key);});
    
    srv.run();
}
