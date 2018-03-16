#include "CBackServer.h"
#include "CRosNodeManager.h"

extern CShareMem shm_bus;
extern bool systerm_exit;

CBackServer::CBackServer()
{
	busctl_pool = new CThreadPool(10);
}

CBackServer::~CBackServer()
{

}

void CBackServer::Exec(int argc,char **argv)
{
	char bus_pack[200] = {0};
	P_HEAD head = {0};
	int Len = 0;
	int fd;
	CRosNodeManager nodeManager;
	nodeManager.Create();
    CRosNode rosnode;
    rosnode.Create();
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

