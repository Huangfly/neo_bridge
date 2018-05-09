#include "CHostAddr.h"

CHostAddr::CHostAddr()
{
	iLen = sizeof(struct sockaddr);
	memset(&addr, 0, iLen);
}

CHostAddr::CHostAddr(const CHostAddr &other)
{
	iLen = sizeof(struct sockaddr);
	memset(&addr, 0, iLen);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(other.getIP());
	addr.sin_port = htons(other.getPort());
}

CHostAddr::CHostAddr(char *ip, int port)
{
	iLen = sizeof(struct sockaddr);
	memset(&addr, 0, iLen);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);
}

CHostAddr::~CHostAddr()
{

}

void CHostAddr::setIP(char *ip)
{
	addr.sin_addr.s_addr = inet_addr(ip);
}

void CHostAddr::setPort(int port)
{
	addr.sin_port = htons(port);
}

char* CHostAddr::getIP() const
{
	return inet_ntoa(addr.sin_addr);
}

int CHostAddr::getPort() const
{
	return ntohs(addr.sin_port);
}

socklen_t CHostAddr::getSize() const
{
	return iLen;
}

socklen_t* CHostAddr::getSizePtr() const
{
	return (socklen_t*)&iLen;
}

struct sockaddr* CHostAddr::getAddr()
{
	return (struct sockaddr*)&addr;
}

struct sockaddr_in* CHostAddr::getInAddr()
{
	return &(this->addr);
}

CHostAddr& CHostAddr::operator =(const CHostAddr& other)
{
	iLen = sizeof(struct sockaddr);
	memset(&addr, 0, iLen);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(other.getIP());
	addr.sin_port = htons(other.getPort());

	return *this;
}
