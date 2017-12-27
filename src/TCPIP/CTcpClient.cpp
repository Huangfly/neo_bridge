#include "CTcpClient.h"

CTcpClient::CTcpClient()
:CTcpSocket()
{

}

CTcpClient::~CTcpClient()
{

}

bool CTcpClient::Connect(CHostAddr &addr)
{
	int ctl;
	ctl = connect(m_fd, addr.getAddr(), addr.getSize());
	return (ctl == -1) ? false : true;
}
