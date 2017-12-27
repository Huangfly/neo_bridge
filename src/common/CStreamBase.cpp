#include "CStreamBase.h"

//ĬϹ캯
CStreamBase::CStreamBase()
{
	m_fd = -1;
}

//γʼ
CStreamBase::CStreamBase(int fd)
{
	m_fd = fd;
}

//д
CStreamBase::~CStreamBase()
{

}

//ļ
void CStreamBase::setFd(int fd)
{
	m_fd = fd;
}

//ȡļ
int CStreamBase::getFd() const
{
	return m_fd;
}

//ļģʽ
int CStreamBase::Read(char *buf, const int buf_len)
{
	int nCount;
	nCount = read(m_fd, buf, buf_len);
	if (nCount == -1)
	{
		perror("read fail.");
//		exit(1);
	}

	return nCount;
}

//ļʱ
int CStreamBase::Read(char *buf, const int buf_len, int timeout)
{
	fd_set readset;
	struct timeval iTime = {0};
	int nEvent;
	FD_ZERO(&readset);
	FD_SET(m_fd, &readset);

	iTime.tv_sec = timeout;
	iTime.tv_usec = 0;
	nEvent = select(m_fd + 1, &readset, NULL, NULL, &iTime);

	switch(nEvent)
	{
	case -1:
		{
			perror("select fail");
			exit(1);
			return -1;
		}
		break;
	case 0:
		{
			printf("time out.\n");
			return 0;
		}
		break;
	default:
		{
			return read(m_fd, buf, buf_len);
		}
		break;
	}
}

//ļַ
int CStreamBase::Read(char *buf, const int buf_len, CHostAddr &remote_addr)
{
	int nCount;
	nCount = recvfrom(m_fd, buf, buf_len, 0, remote_addr.getAddr(), remote_addr.getSizePtr());
	if (nCount == -1)
	{
		perror("read fail.");
		exit(1);
	}
	
	return nCount;
}

//ļַʱ
int CStreamBase::Read(char *buf, const int buf_len, CHostAddr &remote_addr, int timeout)
{
	fd_set readset;
	struct timeval iTime = {0};
	int nEvent;
	FD_ZERO(&readset);
	FD_SET(m_fd, &readset);
	
	iTime.tv_sec = timeout;
	iTime.tv_usec = 0;
	nEvent = select(m_fd + 1, &readset, NULL, NULL, &iTime);
	
	switch(nEvent)
	{
	case -1:
		{
			perror("select fail");
			exit(1);
			return -1;
		}
		break;
	case 0:
		{
			printf("time out.\n");
			return 0;
		}
		break;
	default:
		{
			return recvfrom(m_fd, buf, buf_len, 0, remote_addr.getAddr(), remote_addr.getSizePtr());
		}
		break;
	}
}

//дļģʽ
int CStreamBase::Write(const char *buf, const int buf_len)
{
	int nCount;
	nCount = write(m_fd, buf, buf_len);
	if (nCount == -1)
	{
//		perror("write fail.");
	}
	
	return nCount;
}

//дļַ
int CStreamBase::Write(const char *buf, const int buf_len, CHostAddr remote_addr)
{
	int nCount;
	nCount = sendto(m_fd, buf, buf_len, 0, remote_addr.getAddr(), remote_addr.getSize());
	if (nCount == -1)
	{
//		perror("write fail.");
	}
	
	return nCount;
}
