//
// Created by huang on 18-5-7.
//
#include "CBusTask.h"
#include "CShareMem.h"
#include "CPreServer.h"
extern CShareMem shm_bus;
//============================================================================
//
//  CBusTask
//
//============================================================================
CBusTask::CBusTask(char *buf, int fd, int Len)
        :CTask()
{
    this->buf = new char[Len];
    memcpy(this->buf, buf, Len);
    this->Len = Len;
    this->fd = fd;
}

CBusTask::~CBusTask()
{
    delete []buf;
}

void CBusTask::doAction()
{
    shm_bus.Write(buf, Len, fd);
}