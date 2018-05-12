#include "CHostAddr.h"
#include "CThreadBase.h"
#include "CPreServer.h"
#include "CShareMem.h"
#include "CBackServer.h"
#include "CConfig.h"

#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <neo_bridge/CDebug.h>
//#include "./CLog/CLog.h"
/////////

#define SHMBUS_KEY 1110 
#define SHMACK_KEY 1111 


#define SEMBUS_KEY 2220
#define SEMACK_KEY 2222
//////////
//
void PreProcess(int argc, char *argv[]);//
void BackProcess(int argc, char *argv[]);
//////////
//
CShareMem shm_bus(SHMBUS_KEY, SEMBUS_KEY, 50, 1024);
CShareMem shm_ack(SHMACK_KEY, SEMACK_KEY, 50, 3072);
//CLog server_log(SEMLOG_KEY);
map<int,int> user_list;
map<int,int> connect_list;

int connect_Count = 0;
int rcv_Count = 0;
int ack_Count = 0;

int user_Count = 0;
int video_channel = 0;
int video_type = 0;
int video_region = 0;
int video_list = 0;
int play_data = 0;
int play_time = 0;

int log_fd;
bool systerm_exit = false;

void Catch_signal(int no)
{
}

void killZombieProcess(pid_t mypid){

    NeoInfo("this process pid is %d\n",mypid);
    FILE* fp = popen("ps -ed | grep \'neo_bridge_node\' | awk \'{print $1}\'","r");
    char buffer[10] = {0};
    while(fgets(buffer, 10, fp) != NULL)
    {
        char popbuffer[20] = {0};
        pid_t pid = (pid_t)(atoi(buffer));

        if(pid == mypid) continue;
        NeoInfo("kill %d\n",pid);
        sprintf(popbuffer,"kill %d",pid);
        popen(popbuffer,"r");
    }
    pclose(fp);
}

int main(int argc, char *argv[])
{
	shm_bus.Create();
    shm_ack.Create();
	signal(SIGPIPE, Catch_signal);

    Neo_Config::CConfig config;
    config.LoadFille(argv[1]);
    Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();

    if( config_ptr == NULL ){
        printf("error: no found config file.\n");
        return 0;
    }

    Neo_Log::InitDebug();



    pid_t mypid = getpid();
    killZombieProcess(mypid);
    //PreProcess(argc,argv);

    //ros::init(argc, argv, "neo_bridge");
	pid_t pid;
	pid = fork();


	if (pid < 0)
	{
		perror("fork fail.");
	}
	else if(pid == 0)//
	{
        PreProcess(argc,argv);
	}
	else
	{
        BackProcess(argc,argv);
	}

	return 0;
}


void PreProcess(int argc, char *argv[])
{
#ifdef USE_ROS
    //ros::init(argc, argv, "neo_bridge");
#endif
	CPreServer pre_server;
    pre_server.Exec(argc,argv);
    systerm_exit = true;
}

void BackProcess(int argc, char *argv[])
{
    ros::init(argc, argv, "neo_bridge");
    CBackServer back_server;
    back_server.Exec(argc,argv);
    systerm_exit = true;
}
