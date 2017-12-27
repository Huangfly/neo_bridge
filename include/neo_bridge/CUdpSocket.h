#ifndef _CUDPSOCKET_H_
#define _CUDPSOCKET_H_

#include "CSocket.h"

class CUdpSocket:public CSocket
{
public:
	CUdpSocket(CHostAddr *addr);
	virtual ~CUdpSocket();
	int Send(CHostAddr *dst_addr, void *buf, int count);
	int Recv(void *buf, int count);
private:
	CHostAddr *src_addr;//Դַ
};

#endif
