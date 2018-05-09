//
// Created by huang on 18-5-7.
//
#include "ThreadTimerTask.h"

#define NO_SCREEN

CPrintThread::CPrintThread(CClientEpoll *client_epoll)
{
		this->client_epoll = client_epoll;
}

void CPrintThread::Run()
{
	map<int,int>::iterator it;
	int time = 0;
	//int connect_time = 0;
	while (1)
	{

		sleep(1);
#ifndef NO_SCREEN
		NeoInfo("=============================================================================\n");
        NeoInfo("connect: %d\n", CServerEpoll::fds.size());
		//printf("package: RX-%d  TX-%d\n", rcv_Count, ack_Count);
        NeoInfo("=============================================================================\n");
#endif


		if (time > 2)
		{
			for (it = CServerEpoll::fds.begin(); it != CServerEpoll::fds.end(); it++)
			{
                NeoDebug("count %d\n",it->second);
				if (it->second <= 0)
				{
					client_epoll->DelEvent(it->first, EPOLLOUT);
					close(it->first);
					CServerEpoll::fds.erase(it->first);
				}else{
                    int count_ = it->second - 1;
					CServerEpoll::fds.set(it->first, count_);
				}
			}
			time = 0;
		}
		time++;
	}
}
