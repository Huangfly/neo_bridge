#ifndef _CHOSTADDR_H_
#define _CHOSTADDR_H_

#include <netinet/in.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;


class CHostAddr
{
public:
	CHostAddr();
	CHostAddr(const CHostAddr &other);
	CHostAddr(char *ip, int port);
	~CHostAddr();
	void setIP(char *ip);
	void setPort(int port);
	char* getIP() const;
	int getPort() const;
	socklen_t getSize() const;
	socklen_t* getSizePtr() const;
	struct sockaddr* getAddr();
	struct sockaddr_in* getInAddr();
	CHostAddr & operator= (const CHostAddr& other);
private:
	struct sockaddr_in addr;
	int iLen;
};

#endif