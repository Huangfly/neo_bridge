#ifndef _CTCPCLIENT_H_
#define _CTCPCLIENT_H_

#include "CTcpSocket.h"

class CTcpClient:public CTcpSocket
{
public:
	CTcpClient();
	~CTcpClient();
	bool Connect(CHostAddr &addr);
private:
};

#endif
