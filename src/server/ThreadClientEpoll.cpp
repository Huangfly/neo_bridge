//
// Created by huang on 18-5-7.
//
#include "ThreadClientEpoll.h"


CClientEpollThread::CClientEpollThread(CClientEpoll *client_epoll)
        :CThreadBase()
{
    this->client_epoll = client_epoll;
}

void CClientEpollThread::Run()
{
    client_epoll->Start(-1);
}