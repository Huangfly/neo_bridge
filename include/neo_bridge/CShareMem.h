#ifndef _CSHAREMEM_H_
#define _CSHAREMEM_H_

#include "packet.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <semaphore.h>
using namespace std;

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
							 (Linux-specific) */
};

union sys_status {
    int val;
    unsigned char sys_preserver_ack;
    unsigned char sys_backserver_ack;
    unsigned char sys_user1;
    unsigned char sys_user2;
};

class CSem
{
public:
	CSem(int val, key_t sem_key);
	~CSem();
	void Create();
	void SemOp(int op);
private:
	int val;
	int sem_id;
	key_t sem_key;
};

class CShareMem
{
public:
	CShareMem(int shm_key, int sem_key, int nBlockCount = 10, int BlockSize = 200);
	~CShareMem();
	void Create();
	void Write(char *buf, int Len, int fd);
	void Read(char *buf, int *Len, int *fd, Neo_Packet::HEAD *head);
private:
	int shm_id;
	int shm_key;
	int size;
	char *pBase;
	int BlockSize;
	int nBlockCount;
	int sem_id;
	int sem_key;
	CSem *empty_sem;
	CSem *full_sem;
};

#endif
