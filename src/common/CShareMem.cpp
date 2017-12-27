#include "CShareMem.h"

//źĹ캯
CSem::CSem(int val, key_t sem_key)
{
	this->val = val;
	this->sem_key = sem_key;
}
//ź
CSem::~CSem()
{
	union semun value = {0};
	semctl(sem_id, 0, IPC_RMID, value);
}
//źĳʼ
void CSem::Create()
{
	sem_id = semget(sem_key,0,0);
	if (sem_id == -1)
	{
		perror("semget not create.");
		sem_id = semget(sem_key, 1, IPC_CREAT | 0766);
	}
	
	union semun value = {0};
	value.val = val;
	semctl(sem_id, 0, SETVAL, value);
}
//źPV
void CSem::SemOp(int op)
{
	struct sembuf sem_buf = {0};
	sem_buf.sem_num = 0;
	sem_buf.sem_op = op;
	sem_buf.sem_flg = 0;
	semop(sem_id, &sem_buf, 1);
//	union semun value = {0};
//	semctl(sem_id, 0, GETALL, value);
//	printf("id:%d  val:%d\n",sem_id, value.);
}

CShareMem::CShareMem(int shm_key, int sem_key, int nBlockCount /* = 10 */, int BlockSize /* = 200 */)
{
	this->shm_key = shm_key;
	this->nBlockCount = nBlockCount;
	this->BlockSize = BlockSize;
	this->shm_id = -1;
	this->sem_key = sem_key;
	this->empty_sem = new CSem(nBlockCount, sem_key);//ԴΪnBlockCount
	this->full_sem = new CSem(0, sem_key+1);//ɶԴΪ0
}

CShareMem::~CShareMem()
{
	shmdt(pBase);
	delete empty_sem;
	delete full_sem;
}


void CShareMem::Create()
{
	//ٹڴ
	shm_id = shmget(shm_key, 0, 0);
	if (shm_id >= 0)
	{
		shmctl(shm_id, IPC_RMID, NULL);
	}
	shm_id = shmget(shm_key, nBlockCount * BlockSize, IPC_CREAT | 0766);
	pBase = (char*)shmat(shm_id, NULL, 0);
	///////////////////////////
	//SYSTEM Vź,̼ͬ
	//ʼź
	empty_sem->Create();
	full_sem->Create();
}

//дݿ
void CShareMem::Write(char *buf, int Len, int fd)
{
	if (Len > BlockSize)
	{
		printf("Len:%d\n",Len);
		perror("Len more Big with BlockSize.");
		exit(1);
	}

	//ȴԴ,P-1
	empty_sem->SemOp(-1);

	//ȡеݿλã0 -1д 1Դ
	int pos = 0;
	while (pos < (nBlockCount * BlockSize) && *(pBase + pos) != 0)//ҵݿ
	{
		pos += BlockSize;
	}
	*(pBase + pos) = -1;//־ݿ
	//дݿ
	memcpy(pBase + pos + 1, &fd, sizeof(int));
	memcpy(pBase + pos + 1 + sizeof(int), &Len, sizeof(int));
	memcpy(pBase + pos + 1 + sizeof(int) * 2, buf, Len);

	//Դд
	*(pBase + pos) = 1;
	//ɶԴ+1
	full_sem->SemOp(+1);
}

//ȡԴݿ
void CShareMem::Read(char *buf, int *Len, int *fd, P_HEAD *head)
{
	int val;
	//ȴɶԴ-1
	full_sem->SemOp(-1);
	//Դݿ
	int pos = 0;
	while (pos < (nBlockCount * BlockSize) && *(pBase + pos) != 1)
	{
		pos += BlockSize;
	}
	if (pos >= (nBlockCount * BlockSize))
	{
		return;
	}
	//ȡ
	memcpy(fd, pBase + pos + 1, sizeof(int));
	memcpy(Len, pBase + pos + sizeof(int) + 1, sizeof(int));
	memcpy(buf, pBase + pos + sizeof(int) * 2 + 1, *Len);
	if (head != NULL)
	{
        memcpy(head, buf + HEAD_LEN, sizeof(P_HEAD));
	}
	
	//ȡ,ݿ
	memset(pBase + pos, 0, sizeof(int) * 2 + 1 + *Len);
	//ͷһԴ,V +1
	empty_sem->SemOp(+1);
}
