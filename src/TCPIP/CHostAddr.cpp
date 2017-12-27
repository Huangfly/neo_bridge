#include "CHostAddr.h"

//ĬϹ캯ֻʼ
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

//ι캯
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

//IP
void CHostAddr::setIP(char *ip)
{
	addr.sin_addr.s_addr = inet_addr(ip);
}

//ö˿ں
void CHostAddr::setPort(int port)
{
	addr.sin_port = htons(port);
}

//ȡIP
char* CHostAddr::getIP() const
{
	return inet_ntoa(addr.sin_addr);
}

//ȡ˿ں
int CHostAddr::getPort() const
{
	return ntohs(addr.sin_port);
}

//ȡַ
socklen_t CHostAddr::getSize() const
{
	return iLen;
}

//ȡַȵַ
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
