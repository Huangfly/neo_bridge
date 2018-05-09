#include "CBackServer.h"
#include "CRosNodeManager.h"
#include "CConfig.h"

using namespace Neo_Config;

extern CShareMem shm_bus;
extern CShareMem shm_ack;

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
	Neo_Packet::HEAD head = {0};
	int Len = 0;
	int fd;

    CRosNode rosnode;
    rosnode.Create();

	CRosNodeManager nodeManager;
	nodeManager.Create();

    while(systerm_exit == false)
	{
		shm_bus.Read(bus_pack, &Len, &fd, NULL);

        if (Len > 1)
		{

			busctl_pool->addTask(new CUnpackTask(bus_pack, Len, fd, busctl_pool));
			memset(bus_pack, 0, Len);
		}
    }
    printf("CBackServer exit.\n");
    systerm_exit = true;
}

CShareMem* CBackServer::GetBusShareMemery(){
	return &shm_bus;
}

CShareMem* CBackServer::GetAckShareMemery(){
	return &shm_ack;
}
