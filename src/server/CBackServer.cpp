#include "CBackServer.h"
#include "CRosNodeManager.h"
#include "CConfig.h"

using namespace Neo_Config;

extern CShareMem shm_bus;
extern bool systerm_exit;
Neo::MutexMap<unsigned int, Neo_Type::UserData> CBackServer::UserDatas(1024);

CBackServer::CBackServer()
{
	busctl_pool = new CThreadPool(10);
}

CBackServer::~CBackServer()
{

}

void CBackServer::Exec(int argc,char **argv)
{
	char bus_pack[2200] = {0};
	P_HEAD head = {0};
	int Len = 0;
	int fd;

    printf("config file: %s\n",argv[1]);
    Neo_Config::CConfig config;
    config.LoadFille(argv[1]);

    CRosNode rosnode;
    rosnode.Create();

	CRosNodeManager nodeManager;
	nodeManager.Create();

    while(systerm_exit == false)
	{
        //usleep(50000);
		shm_bus.Read(bus_pack, &Len, &fd, NULL);

        if (Len > 1)//
		{

			busctl_pool->addTask(new CUnpackTask(bus_pack, Len, fd, busctl_pool));
			memset(bus_pack, 0, Len);
		}
    }
    printf("CBackServer exit.\n");
    systerm_exit = true;
}

