#ifndef _CTCPSOCKET_H_
#define _CTCPSOCKET_H_

#include "CSocket.h"

class CTcpSocket:public CSocket
{
public:
	CTcpSocket();
	CTcpSocket(const CTcpSocket &other);
	virtual ~CTcpSocket();
	CTcpSocket & operator= (const CTcpSocket &other);
	CHostAddr* getRemoteAddr() const;
	void setRemoteAddr(CHostAddr *remote_addr);
	int Read(char *buf, const int buf_len);
	int Read(char *buf, const int buf_len, int timeout);
	int Write(char *buf, const int buf_len);
protected:
	CHostAddr *m_remote_addr;
};

#endif
