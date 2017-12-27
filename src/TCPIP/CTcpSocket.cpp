#include "CTcpSocket.h"

//TCP׽ֵĬϹ캯
CTcpSocket::CTcpSocket()
:CSocket(true, AF_INET, 0)
{
	m_remote_addr = NULL;
}

//TCP׽ֵĿ캯
CTcpSocket::CTcpSocket(const CTcpSocket &other)
:CSocket(true, AF_INET, 0)
{
	this->m_fd = other.getFd();
	this->m_stream.setFd(this->m_fd);
	this->m_remote_addr = other.getRemoteAddr();
}

//TCP׽ֵ
CTcpSocket::~CTcpSocket()
{
	
}

//ظֵ
CTcpSocket& CTcpSocket::operator =(const CTcpSocket &other)
{
	this->m_fd = other.getFd();
	this->m_stream.setFd(m_fd);
	m_remote_addr = other.getRemoteAddr();
	return *this;
}

CHostAddr* CTcpSocket::getRemoteAddr() const
{
	return m_remote_addr;
}

void CTcpSocket::setRemoteAddr(CHostAddr *remote_addr)
{
	m_remote_addr = remote_addr;
}

int CTcpSocket::Read(char *buf, const int buf_len)
{
	return m_stream.Read(buf, buf_len);
}

int CTcpSocket::Read(char *buf, const int buf_len, int timeout)
{
	return m_stream.Read(buf, buf_len, timeout);
}

int CTcpSocket::Write(char *buf, const int buf_len)
{
	return m_stream.Write(buf, buf_len);
}
