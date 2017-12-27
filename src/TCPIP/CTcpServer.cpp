#include "CTcpServer.h"

CTcpServer::CTcpServer()
:CTcpSocket()
{
}

CTcpServer::~CTcpServer()
{

}

bool CTcpServer::Listen(int nlisten)
{
	if (listen(m_fd, nlisten) == -1)
	{
		perror("Listen fail.");
		return false;
	}
	return true;
}

CTcpSocket CTcpServer::Accept()
{
	CTcpSocket client;
	int client_fd;
	client_fd = accept(m_fd, NULL, NULL);
	client.setFd(client_fd);
	return client;
}
