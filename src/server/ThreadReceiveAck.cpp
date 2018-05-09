//
// Created by huang on 18-5-7.
//
#include "ThreadReceiveAck.h"
#include "CPreServer.h"
extern CShareMem shm_ack;



CSendPackTask::CSendPackTask(char *buf, int Len, int fd, Neo_Packet::HEAD *ack_head,CClientEpoll *client_epoll)
        :CTask()
{
    this->client_epoll = client_epoll;
    this->buf = new char[Len];
    memcpy(this->buf,buf,Len);
    this->Len = Len;
    this->fd = fd;
    memcpy(&(this->ack_head), ack_head, sizeof(Neo_Packet::HEAD));
}

CSendPackTask::~CSendPackTask()
{
    delete []buf;
}

void CSendPackTask::doAction()
{
    CStreamBase write_ack;
    map<int,int>::iterator it;
    int ctl = 0;
    write_ack.setFd(fd);
    ctl = write_ack.Write(buf, Len);
    //printf("write succese fd = %d\n",fd);
}



CRcvAckThread::CRcvAckThread(CClientEpoll *client_epoll)
{
    this->client_epoll = client_epoll;
    this->sendack_poll = new CThreadPool(20);
}

void CRcvAckThread::Run()
{
    char ack_buf[4096] = {0};
    int Len;
    int fd;

    Neo_Packet::HEAD head = {0};


    while (1)
    {
        shm_ack.Read(ack_buf, &Len, &fd, &head);
        if (Len > 10)
        {
            this->sendack_poll->addTask(new CSendPackTask(ack_buf, Len, fd, &head,this->client_epoll));
            Len = 0;
        }
    }
}