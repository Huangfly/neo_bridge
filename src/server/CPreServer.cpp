#include "CPreServer.h"
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <sys/vfs.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define NO_SCREEN

extern bool systerm_exit;

extern CShareMem shm_bus;
extern CShareMem shm_ack;
extern map<int,int> user_list;
extern map<int,int> connect_list;

extern int connect_Count;
extern int rcv_Count;
extern int ack_Count;

extern int user_Count;
extern int video_channel;
extern int video_type;
extern int video_region;
extern int video_list;
extern int play_data;
extern int play_time;

CClientEpollThread::CClientEpollThread(CClientEpoll *client_epoll)
:CThreadBase()
{
	this->client_epoll = client_epoll;
}

//
CPrintThread::CPrintThread(CClientEpoll *client_epoll)
{
		this->client_epoll = client_epoll;
}

//
void CPrintThread::Run()
{
	map<int,int>::iterator it;
	int time = 0;
	int connect_time = 0;
	while (1)
	{

		sleep(1);
#ifndef NO_SCREEN
		printf("=============================================================================\n");
		printf("connect: %d\n", connect_Count);
		printf("package: RX-%d  TX-%d\n", rcv_Count, ack_Count);
		printf("=============================================================================\n");
#endif		


	/*	if (time > 5)
		{
			for (it = user_list.begin(); it != user_list.end(); it++)
			{
				if (it->second <= 0)
				{
					client_epoll->DelEvent(it->first, EPOLLOUT);
					close(it->first);
					user_list.erase(it->first);
					connect_Count--;
					user_Count--;
				}
			}
			time = 0;
		}*/

		/*if (connect_time > CLIENTEPOLL_TIMEOUT)
		{
			for (it = connect_list.begin(); it != connect_list.end(); it++)
			{
				if (it->second <= 0)
				{
					client_epoll->DelEvent(it->first, EPOLLOUT);
					close(it->first);
					connect_list.erase(it->first);
					connect_Count--;
				}
			}
			connect_time = 0;
        }*/
		connect_time++;
		time++;
	}
}

//ͻ˼߳
void CClientEpollThread::Run()
{
	client_epoll->Start(-1);
}

//Ӧ
CSendPackTask::CSendPackTask(char *buf, int Len, int fd, P_HEAD *ack_head,CClientEpoll *client_epoll)
:CTask()
{
	this->client_epoll = client_epoll;
	this->buf = new char[Len];
	memcpy(this->buf,buf,Len);
	this->Len = Len;
	this->fd = fd;
	memcpy(&(this->ack_head), ack_head, sizeof(P_HEAD));
}

CSendPackTask::~CSendPackTask()
{
	delete []buf;
}

void CSendPackTask::doAction()
{
	CStreamBase write_ack;
	map<int,int>::iterator it;
	int ctl = 0;
	write_ack.setFd(fd);
	ctl = write_ack.Write(buf, Len);
	if (ctl < 0)
	{
		//client_epoll->DelEvent(fd, EPOLLOUT);
		//close(fd);
		//connect_list.erase(fd);
	}
	//printf("write succese fd = %d",fd);
	ack_Count++;
/*	if(ctl > 0)
	{
		
		switch(ack_head.funcId)
		{
		case PACK_LOGIN:
			{
				A_LOGIN login_ret = {0};
				memcpy(&login_ret, buf + H_E_LEN + sizeof(P_HEAD), sizeof(A_LOGIN));
				if (login_ret.login_ret == 1 && user_list.find(fd) == user_list.end() && connect_list.find(fd) != connect_list.end())
				{
					connect_list.erase(fd);
					user_list.insert(pair<int, int>(fd, 5));
					user_Count++;
				}
			}
			break;
		case PACK_CHANNEL:
			video_channel++;
			break;
		case PACK_TYPE:
			video_type++;
			break;
		case PACK_REGION:
			video_region++;
			break;
		case PACK_VIDEO:
			video_list++;
			break;
		case PACK_VPLAY:
			play_data++;
			break;
		case PACK_VTIME:
			play_time++;
			break;
		default:
			break;
		}
	}*/
}

CRcvAckThread::CRcvAckThread(CClientEpoll *client_epoll)
{
	this->client_epoll = client_epoll;
	this->sendack_poll = new CThreadPool(20);
}

void CRcvAckThread::Run()
{
	char ack_buf[4096] = {0};
	int Len;
	int fd;
	
	P_HEAD head = {0};
	
	
	while (1)
	{
		shm_ack.Read(ack_buf, &Len, &fd, &head);
		if (Len > 10)
		{
            //printf("ack pack len = %d ; num:%d\n",Len,ack_buf[1]);
			this->sendack_poll->addTask(new CSendPackTask(ack_buf, Len, fd, &head,this->client_epoll));
			Len = 0;
		}
	}
}

CPreServer::CPreServer()
{
	
}


CPreServer::~CPreServer()
{

}

void CPreServer::Exec(int argc,char **argv)
{

	while(1) {
		isRestart = false;
		struct ifaddrs *ifAddrStruct = NULL;
		void *tmpAddrPtr = NULL;
		char Myaddress[INET_ADDRSTRLEN];

		getifaddrs(&ifAddrStruct);

		while (ifAddrStruct != NULL) {
			if (ifAddrStruct->ifa_addr->sa_family == AF_INET) {   // check it is IP4
				// is a valid IP4 Address
				tmpAddrPtr = &((struct sockaddr_in *) ifAddrStruct->ifa_addr)->sin_addr;
				char addressBuffer[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
				//printf("%s IPV4 Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
				if (memcmp(addressBuffer, "192.168", 7) == 0) {
					memcpy(Myaddress, addressBuffer, strlen(addressBuffer) + 1);
				}
			} else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6) {   // check it is IP6
				// is a valid IP6 Address
				tmpAddrPtr = &((struct sockaddr_in *) ifAddrStruct->ifa_addr)->sin_addr;
				char addressBuffer[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
				//printf("%s IPV6 Address %s\n", ifAddrStruct->ifa_name, addressBuffer);
			}
			ifAddrStruct = ifAddrStruct->ifa_next;
		}
		printf("host Ip:%s\n", Myaddress);
		CHostAddr Server_addr(Myaddress, 8888);
		//
		CClientEpoll client_epoll(10001);//
		//
		CServerEpoll server_epoll(&Server_addr, 1000, &client_epoll);

		bool ret;

		ret = server_epoll.Connect();
		if (ret == true) {
			//break;
		} else {
			return;
		}
		//ClientEpoll
		CClientEpollThread client_thread(&client_epoll);
		client_thread.Create();
		//
		CPrintThread printf_thread(&client_epoll);
		printf_thread.Create();
		//
		CRcvAckThread rcvAck_Thread(&client_epoll);
		rcvAck_Thread.Create();
		///////////////////////////////////

		//CRosNode rosnode;
		//rosnode.Create();
#ifdef USE_ROS
		while (!isRestart) {
			server_epoll.Wait(10);
			//printf("CPreServer %d\n",systerm_exit);
			//ros::spinOnce();
		}
		if(isRestart){

			rcvAck_Thread.Cancel();
			printf_thread.Cancel();
			client_thread.Cancel();
		}
#else
		server_epoll.Start(-1);
#endif
	}
    printf("CPreServer exit.\n");
}

void CPreServer::Restart() {
	this->isRestart = true;
}
