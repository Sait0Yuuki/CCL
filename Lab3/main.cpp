#include "participant.h" //already include coo.h in part.h
#include <fstream>
#include <iostream>
#include <string.h>


using namespace std;

int mode=0; //0 part 1 coo 

vector<int> partPort;
int cooPort;

void conf_parser(char *configPath)
{
	ifstream read_file;
	read_file.open(configPath,ios::binary);
	string line;
	while(getline(read_file, line))
	{
		if(line[0]!='!')
		{
			if(line.find("mode")!=string::npos)
			{
				if(line.find("participant")!=string::npos) mode=0;
				else mode=1;
			}
			else if(line.find("info")!=string::npos)
			{
				if(line.find("participant")!=string::npos)
				{
					int pos=line.find(':');
					string port=line.substr(pos+1);
					partPort.push_back(stoi(port));

				}
				else
				{
					int pos=line.find(':');
					string port=line.substr(pos+1);
					cooPort=stoi(port);

				}
			}
		}
	}

}

//./kvstore2pcsystem --config_path ./src/coordinator.conf
//./kvstore2pcsystem --config_path ./src/participant.conf
int main(int argc,char **argv)
{
    char *configPath;
	for(int i=1;i<argc;i++)
    {
        if(strcmp("--config_path",argv[i])==0)  // --port
        {
            configPath=argv[++i];
            if(!configPath){
                fprintf(stderr, "Expected argument after --config_path\n");
            }
        }
    }

	conf_parser(configPath);

	cout<<"MODE:"<<mode<<endl;
	cout<<"PART:"<<endl;
	for(int i=0;i<partPort.size();i++) cout<<partPort[i]<<" ";
	cout<<endl;
	cout<<"COO:"<<endl;
	cout<<cooPort<<endl;

	if(mode==0) participant_init(partPort);
	else coordinator_init(cooPort,partPort);
    
    return 0;
}