#include "CSocket.h"

//캯׽ֵͻеַȥ
CSocket::CSocket(bool ifTCP, int in_family, int protocol)
{
	type = ifTCP;
	this->in_family = in_family;
	this->protocol = protocol;
	m_fd = -1;
}

//
CSocket::~CSocket()
{
}

bool CSocket::Create()
{
	if (type)//true TCP׽
	{
		m_fd = socket(in_family, SOCK_STREAM, protocol);
	}
	else
	{
		m_fd = socket(in_family, SOCK_DGRAM, protocol);
	}
	if (m_fd < 0)
	{
		return false;
	}
	m_stream.setFd(m_fd);
	return true;
}

//׽ֵİ
bool CSocket::Bind(CHostAddr &addr)
{
	int trl = 1;
	setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &trl, sizeof(int));
	trl = bind(m_fd, addr.getAddr(), addr.getSize());
	if (trl < 0)
	{
		perror("Bind fail.");
		return false;
	}
	return true;
}

bool CSocket::Close()
{
	if (close(m_fd) == -1)
	{
		return false;
	}
	return true;
}

int CSocket::getFd() const
{
	return m_fd;
}

void CSocket::setFd(int fd)
{
	m_fd = fd;
	m_stream.setFd(m_fd);
}
