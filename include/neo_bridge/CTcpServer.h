#ifndef _CTCPSERVER_H_
#define _CTCPSERVER_H_

#include "CTcpSocket.h"

class CTcpServer:public CTcpSocket
{
public:
	CTcpServer();
	virtual ~CTcpServer();
	bool Listen(int nlisten);
	CTcpSocket Accept();
private:
};

#endif
