//
// Created by huang on 18-5-7.
//
#include "CUnDataPackTask.h"
#include "CBusTask.h"
#include "packet.h"

//============================================================================
//
//  CUnDataPackTask
//
//============================================================================
CUnDataPackTask::CUnDataPackTask(char *bus_buf, int Len, int fd, CThreadPool *busctl_pool)
:CTask()
{
	this->m_BusPool = busctl_pool;
	this->buf = new char[Len];
	memcpy(this->buf, bus_buf, Len);
	this->nRead = Len;
	this->fd = fd;
}

CUnDataPackTask::~CUnDataPackTask()
{
	delete []buf;
}

void CUnDataPackTask::doAction() {
	Neo_Packet::HEAD head = {0};
	char head_str[2] = {HEADER_CHR, HEADER_CHR};
	char end_str[2] = {ENDER_CHR, ENDER_CHR};

	for (int i = 0; i < nRead; i++) {
		if (memcmp(buf + i, head_str, HEADER_LEN) == 0) {
			memcpy(&head, buf + i + HEADER_LEN, sizeof(Neo_Packet::HEAD));
			if (head.size > nRead - i) {
				continue;
			}
			if (memcmp(buf + i + head.size + HEADER_LEN + 1, end_str, HEADER_LEN) == 0) {
				m_BusPool->addTask(new CBusTask(buf + i + HEADER_LEN, fd, head.size));
				i += (head.size - 1);

				if (*(buf + i + 1) != HEADER_CHR) {
					return;
				}
				continue;
			} else {
				continue;
			}
		}
	}
}
