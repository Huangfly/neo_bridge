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
#include <neo_bridge/CConfig.h>
#include <neo_bridge/CDebug.h>
#include <neo_bridge/ThreadReceiveAck.h>
#include <neo_bridge/ThreadTimerTask.h>
#include <neo_bridge/ThreadClientEpoll.h>

extern bool systerm_exit;

extern CShareMem shm_bus;

//extern map<int,int> user_list;
//extern map<int,int> connect_list;

//extern int connect_Count;
//extern int rcv_Count;
//extern int ack_Count;




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
		char Myaddress[INET_ADDRSTRLEN+1] = {0};

		Neo_Config::ConfigParamer *config_ptr = Neo_Config::GetConfigParamer();

		if( config_ptr == NULL ){
			printf("error: no found config file.\n");
			return;
		}
		if(config_ptr->isAutoIP){
			this->AutoIP(Myaddress);
		}else{
			memcpy(Myaddress,config_ptr->ip_.c_str(),config_ptr->ip_.size());
		}

        NeoDebug("host Ip:%s\n", Myaddress);
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

		while (!isRestart) {
			server_epoll.Wait(10);
		}
		if(isRestart){

			rcvAck_Thread.Cancel();
			printf_thread.Cancel();
			client_thread.Cancel();
		}
	}
    printf("CPreServer exit.\n");
}

void CPreServer::AutoIP(char Myaddress[21]){
	struct ifaddrs *ifAddrStruct = NULL;
	void *tmpAddrPtr = NULL;
	//char Myaddress[INET_ADDRSTRLEN];

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
				break;
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
}

void CPreServer::Restart() {
	this->isRestart = true;
}
