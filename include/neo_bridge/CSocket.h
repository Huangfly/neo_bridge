#ifndef _CSOCKET_H_
#define _CSOCKET_H_

#include "CStreamBase.h"

class CSocket
{
public:
	CSocket(bool ifTCP, int in_family, int protocol);
	virtual ~CSocket();
	bool Create();
	bool Bind(CHostAddr &addr);
	bool Close();
	int getFd() const;
	void setFd(int fd);
protected:
	bool type;
	int in_family;
	int protocol;
	int m_fd;
	CStreamBase m_stream;
	//CHostAddr *addr;
};

#endif
