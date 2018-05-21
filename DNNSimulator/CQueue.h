#ifndef CQUEUE_H
#define CQUEUE_H

#include "common.h"
//#include "CMachine.h"

class CMachine;

class CQueue
{
public:
	CQueue( ULONG ulMaxLength );
	~CQueue();

	long CreateQueue();
	long DestroyQueue();
	long Process();
	long ReqCompletionCallBack(ULONG ulReqid);
	BOOL IsFull();
	long GetAvilableMachine(stReq* pstReq);
	long MakeReq(stReq& stNewReq);
	void JudgeFull();
	void UpServTmForItfparaCng(BOOL bThreadNumIncr);

	//void SetInuseFlag( BOOL bInuseFlag );
	//void GetInuseFlag( BOOL& bInuseFlag );

private:

	//ULONG m_RequestNum;
	BOOL m_bReqCompletion;
	vector<CMachine*> m_vcpMachinelist;
	vector<stReq> m_vstRequestList;
	vector<stReq> m_vstProcessingRequestList;
	vector<stReq> m_vstCompletedRequestList;
	BOOL m_bFull;
	BOOL m_bMachFullRunning;
	ULONG m_ulIdleMachineNum;
	ULONG m_ulCompleteReqNum;

};


int VecCmpByArr(const stReq& lReq, const stReq& rReq);

int VecCmpBySSV(const stReq& lReq, const stReq& rReq);

int VecCmpBySVL(const stReq& lReq, const stReq& rReq);

int VecCmpByCmpt(const stReq& lReq, const stReq& rReq);


#endif