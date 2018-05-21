#ifndef CMACHINE_H
#define CMACHINE_H

#include "common.h"
//#include "CQueue.h"
//#include "CThread.h"

class CQueue;
class CThread;

class CMachine
{
public:
	CMachine();
	~CMachine();

	long CreateMachine(ULONG uiMachineid, CQueue* pcQueue);
	long DestroyMachine();
	long run(stReq* pstReq);
	BOOL IsFullRunning();
	long GetAvilableThread(stReq*  pstReq);
	void SetFullRunningFlag( BOOL bFlag );
	void GetFullRunningFlag( BOOL& bFlag );
	void ReqCompletionCallBack(ULONG ulReqid);
	ULONG GetMachineid();
	void RleaseOneThread(stReq* pstReq);

private:

	ULONG m_uiMachineid;
	ULONG m_IdleThreadNum;
	BOOL m_bFullRunning;
	vector<CThread*> m_vcThreadList;
	CQueue* m_pcQueue; //for callback

};

#endif