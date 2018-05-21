#include "CMachine.h"
#include "CQueue.h"
#include "CThread.h"

CMachine::CMachine()
{
	m_IdleThreadNum = INVALIDE;
	m_pcQueue = NULL;
	m_bFullRunning = FALSE;
}
CMachine::~CMachine()
{
	m_IdleThreadNum = INVALIDE;
	m_uiMachineid = INVALIDE;
	m_pcQueue = NULL;

	//release 8 threads
	for(ULONG ulThread = 0; ulThread < WHOLETHREADNUM; ulThread++ )
	{
		if(NULL != m_vcThreadList[ulThread])
		{
			m_vcThreadList[ulThread]->DestroyThread();
			delete m_vcThreadList[ulThread];
			m_vcThreadList[ulThread] = NULL;
		}
	}

}

long CMachine::CreateMachine(ULONG uiMachineid, CQueue* pcQueue)
{
	long lRet = TRUE;

	m_IdleThreadNum = g_servthreadNumForMachine;
	m_uiMachineid = uiMachineid;
	m_pcQueue = pcQueue;

	CThread* pcThread = NULL;

	//create 8 threads
	for(ULONG ulThread = 0; ulThread < WHOLETHREADNUM; ulThread++ )
	{
		pcThread = new CThread;
		if(NULL == pcThread)
		{
			lRet = FALSE;
			return lRet;
		}

		lRet = pcThread->CreateThread(ulThread, this);

		m_vcThreadList.push_back(pcThread);

	}

	return lRet;
}

long CMachine::DestroyMachine()
{
	long lRet = TRUE;

	m_IdleThreadNum = g_servthreadNumForMachine;
	m_uiMachineid = INVALIDE;

	//create 8 threads
	for(ULONG ulThread = 0; ulThread < WHOLETHREADNUM; ulThread++ )
	{
		if(NULL != m_vcThreadList[ulThread])
		{
			lRet = m_vcThreadList[ulThread]->DestroyThread();
			delete m_vcThreadList[ulThread];
			m_vcThreadList[ulThread] = NULL;
		}
	}

	return lRet;
}

long CMachine::run(stReq* pstReq)
{
	long lRet = TRUE;
	assert(NULL != pstReq);
	

	ULONG ulThreadid = pstReq->vstMTidlist[0].ulThreadid;
	lRet = m_vcThreadList[ulThreadid]->Run( pstReq );

	return TRUE;
}

BOOL CMachine::IsFullRunning()
{
	return m_bFullRunning;
}

long CMachine::GetAvilableThread(stReq*  pstReq)
{
	int iThreadNum = g_servthreadNum;
	stMTids stTempid;
	memset(&stTempid, 0, sizeof(stTempid));

	for(ULONG ulThread = 0; ulThread < g_servthreadNumForMachine; ulThread++)
	{
		if ( FALSE == m_vcThreadList[ulThread]->IsInuse())
		{
			stTempid.ulMachineid = m_uiMachineid;
			stTempid.ulThreadid = ulThread;
			pstReq->vstMTidlist.push_back(stTempid);

			m_vcThreadList[ulThread]->SetInuseFlag(TRUE);
			iThreadNum--;
			m_IdleThreadNum--;

			if(0 == iThreadNum)
			{
				break;
			}
		}
	}

	pstReq->ulRunningThreadNumInTheMachine = g_servthreadNumForMachine - m_IdleThreadNum;
	if(m_IdleThreadNum < g_servthreadNum )
	{
		m_bFullRunning = TRUE;
	}

	return TRUE;
}

void CMachine::SetFullRunningFlag( BOOL bFlag )
{
	m_bFullRunning = bFlag;
	return ;
}

void CMachine::GetFullRunningFlag( BOOL& bFlag )
{
	bFlag = m_bFullRunning;
	return ;
}

void CMachine::ReqCompletionCallBack(ULONG ulReqid)
{

	m_pcQueue->ReqCompletionCallBack(ulReqid);
	return;
}

ULONG CMachine::GetMachineid()
{
	return m_uiMachineid;
}

void CMachine::RleaseOneThread(stReq* pstReq)
{
	for( ULONG iloop = 0; iloop < pstReq->vstMTidlist.size(); iloop++ )
	{
		if((m_uiMachineid == pstReq->vstMTidlist[iloop].ulMachineid) 
			&& ( TRUE == m_vcThreadList[pstReq->vstMTidlist[iloop].ulThreadid]->IsInuse()))
		{
			m_vcThreadList[pstReq->vstMTidlist[iloop].ulThreadid]->SetInuseFlag(FALSE);
			m_IdleThreadNum++;
		}
	}

	if(!(m_IdleThreadNum < g_servthreadNum))
	{
		SetFullRunningFlag(FALSE);
	}

	return;
}
