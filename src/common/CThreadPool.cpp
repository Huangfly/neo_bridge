#include "CThreadPool.h"

CTask::CTask()
{

}

CTask::~CTask()
{

}

void CTask::doAction()
{

}

//̳߳
CThreadPool::CThreadPool(int maxCount /* = 10 */)
{
	this->m_maxCount = maxCount;
	this->m_nowCount = 0;
	this->m_waitCount = 0;
	this->threadlist.clear();
	this->isQuit = false;

	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond, NULL);
	pthread_cond_init(&end_cond, NULL);
}

//̳߳
CThreadPool::~CThreadPool()
{
	//˳߳
	this->isQuit = true;
	//б
	pthread_mutex_lock(&m_mutex);
	threadlist.clear();
	//е߳
	pthread_cond_broadcast(&m_cond);
	//ȴ߳̽
	pthread_cond_wait(&m_cond, &m_mutex);
	//
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);

	printf("delete.\n");
}

//
void CThreadPool::addTask(CTask *Task)
{
	int rlt;
	//סбԴȴ̲߳б
	pthread_mutex_lock(&m_mutex);
	threadlist.push_back(Task);
	pthread_mutex_unlock(&m_mutex);

	//жǷеȴ߳
	if (m_waitCount > 0)//еȴ߳
	{
		//ѵȴĳһ
		pthread_mutex_lock(&m_mutex);
		pthread_cond_signal(&m_cond);
		pthread_mutex_unlock(&m_mutex);
	}
	else
	{
		//жǷ
		//ִе߳С߳ʱſԴµ߳
		if (m_nowCount < m_maxCount)
		{
			rlt = pthread_create(&tid, NULL, runtine, this);
			if (rlt != 0)
			{
				printf("thread count:%d\n",this->m_nowCount);
				perror("pthread_create fail");
				exit(1);
			}
			m_nowCount++;
		}
	}
}

//
void* CThreadPool::runtine(void *pSelf)
{
	CTask *pTask;
	CThreadPool *threadpool;
	threadpool = (CThreadPool*)pSelf;

	pthread_detach(pthread_self());

	while (threadpool->isQuit != true)
	{
		//////////////////////////////
		//סб߳ռȴ߳ͷ
		//
		//ʱ̹߳𣬲ͷȴ
		//ʱȡȻִ
		pthread_mutex_lock(&(threadpool->m_mutex));
		if (threadpool->threadlist.empty())//Ϊգ
		{
			//ͷȴȴѣֱбʱŻѣaddTaskжϣ
			threadpool->m_waitCount++;
			pthread_cond_wait(&(threadpool->m_cond), &(threadpool->m_mutex));
			pthread_mutex_unlock(&(threadpool->m_mutex));//waitὫػȥҪ
			threadpool->m_waitCount--;
		}
		else
		{
			//൱ڹԴԶĲҪڽ֮ǰ
			//Ƚȡбɾ
			pTask = threadpool->threadlist.front();
			threadpool->threadlist.pop_front();
			//ִ֮ǰͷŵִ
			pthread_mutex_unlock(&(threadpool->m_mutex));
			//ִ
			pTask->doAction();
			delete pTask;
		}
	}
	//߳̽
	threadpool->m_nowCount--;//ִе߳-1
	printf("task quit.\n");

	if (threadpool->m_nowCount <= 0)//ȫ߳̽
	{
		pthread_cond_signal(&(threadpool->m_cond));
	}
		
	pthread_exit(NULL);
}
