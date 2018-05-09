#ifndef _CPRESERVER_H_
#define _CPRESERVER_H_

#include "CShareMem.h"
#include "CThreadBase.h"
#include "CServerEpoll.h"
#include "CClientEpoll.h"
#include "CRosNode.h"

class CPreServer
{
public:
	CPreServer();
	~CPreServer();
        void Exec(int argc,char **argv);
	void Restart();
	void AutoIP(char Myaddress[]);
private:
	bool isRestart;
};

#endif
