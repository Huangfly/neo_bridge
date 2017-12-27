#ifndef _CSTREAMBASE_H_
#define _CSTREAMBASE_H_

#include "CHostAddr.h"

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

class CStreamBase
{
public:
	CStreamBase();
	CStreamBase(int fd);
	virtual ~CStreamBase();
	void setFd(int fd);
	int getFd() const;
	int Read(char *buf, const int buf_len);
	int Read(char *buf, const int buf_len, int timeout);//Ĭ϶
	int Read(char *buf, const int buf_len, CHostAddr &remote_addr);
	int Read(char *buf, const int buf_len, CHostAddr &remote_addr, int timeout);
	int Write(const char *buf, const int buf_len);
	int Write(const char *buf, const int buf_len, CHostAddr remote_addr);
protected:
	int m_fd;
};

#endif
