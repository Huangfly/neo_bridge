#include "CUdpSocket.h"

CUdpSocket::CUdpSocket(CHostAddr *addr)
:CSocket(false, AF_INET, 0)
{

}

CUdpSocket::~CUdpSocket()
{

}

int CUdpSocket::Recv(void *buf, int count)
{

}

int CUdpSocket::Send(CHostAddr *dst_addr, void *buf, int count)
{

}
