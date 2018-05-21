#ifndef CTHREAD_H
#define CTHREAD_H

#include "common.h"
//#include "CMachine.h"

class CMachine;

class CThread
{
public:
	CThread();
	~CThread();

	long CreateThread(ULONG uiThreadid, CMachine* pcMachine);
	long DestroyThread();
	long GetThreadID(ULONG uiThreadid);
	long Run(stReq* pstReq);
	BOOL IsInuse();
	void SetInuseFlag( BOOL bInuseFlag );
	void GetInuseFlag( BOOL& bInuseFlag );

private:

	CMachine * m_pCmachine;  // for callback
	ULONG m_uiThreadid;
	BOOL m_bInuse;

};

#endif