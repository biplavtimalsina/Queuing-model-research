#include "CThread.h"

#include "CMachine.h"

CThread::CThread()
{
	m_bInuse = FALSE;
	m_pCmachine = NULL;
	m_uiThreadid = INVALIDE;
}

CThread::~CThread()
{
	m_bInuse = FALSE;
	m_pCmachine = NULL;
	m_uiThreadid = INVALIDE;
}

long CThread::CreateThread(ULONG uiThreadid, CMachine* pcMachine)
{
	long lRet = TRUE;

	m_bInuse = FALSE;
	m_pCmachine = pcMachine;
	m_uiThreadid = uiThreadid;

	return lRet;
}

long CThread::DestroyThread()
{
	long lRet = TRUE;

	m_bInuse = FALSE;
	m_pCmachine = NULL;
	m_uiThreadid = INVALIDE;

	return lRet;
}

long CThread::GetThreadID(ULONG uiThreadid)
{
	uiThreadid = m_uiThreadid;

	return TRUE;
}

long CThread::Run(stReq* pstReq)
{
	static double serviceTime[numTotalTimeFrame*numReqinUnitTimeFrame] = {0.0};
	static int localCount = 1;
	static int Run_count = 0;


	ULONG ulThreadParaIndex = pstReq->ulRunningThreadNumInTheMachine/g_servthreadNum-1; 
	SetInuseFlag(TRUE);
	// Service time = basic service time 
	//                * Multiple thread serving interference factor 
	//                * multiple serving machine factor
	//                * communication interference
//	double dBasicServiceTime = GetService();
	/*
	double x;
	if (RL == 1 && Run_count == 0) {
		for (int i = 0; i < numTotalTimeFrame*numReqinUnitTimeFrame ; i++) {
			serviceTime[i]=Exponential(100);
		}
	}

	if (RL == 1)
		x = serviceTime[Run_count];
	else
		x = serviceTime[Run_count - numTotalTimeFrame*numReqinUnitTimeFrame];

		*/

	//printMatrix((double *)serviceTime, 1,numTotalTimeFrame*numReqinUnitTimeFrame);

	//service[Run_count] = x;
	//cout << "Local Count" << localCount << "\t x==" << serviceTime[localCount - numTotalTimeFrame*numReqinUnitTimeFrame] << "\t localCount-TotalCount==" << localCount - numTotalTimeFrame*numReqinUnitTimeFrame << endl;
	//cout << "Inside CThread, RL==" << RL;
	
	//to change the basic service time at each time frame
	//double service[numTotalTimeFrame*numReqinUnitTimeFrame] = { 0.0 };
	/*
	if (RL == 0) {
		x = service[Run_count- numTotalTimeFrame*numReqinUnitTimeFrame];
	}
	else
		service[Run_count] = x;

	cout << "x=" << x << endl;
	*/
	
	//cout << "x==" << x << endl;
	
	/* to provide same service distribution to both SC and RL
	if (RL == 0 && localCount - numTotalTimeFrame*numReqinUnitTimeFrame == 1) {
		printMatrix((double *)serviceTime, 1, numTotalTimeFrame*numReqinUnitTimeFrame);
	}
	if (RL == 1) {
		cout << "Local Count" << localCount << "\t x==" << x << "\t localCount-TotalCount==" << localCount - numTotalTimeFrame*numReqinUnitTimeFrame << endl;
		serviceTime[localCount] = x;
	}
	else
	{
		cout << "Local Count" << localCount << "\t x==" << serviceTime[localCount - numTotalTimeFrame*numReqinUnitTimeFrame] << "\t localCount-TotalCount==" << localCount - numTotalTimeFrame*numReqinUnitTimeFrame << endl;
		x = serviceTime[localCount - numTotalTimeFrame*numReqinUnitTimeFrame];
	}
	localCount++;*/
	//cout << "_________________" << endl;
	//Run_count is for using the pre-randomly generated service time for basic service time information
	//static int firstLoop = 0;
	
	//if (RL == 0 && firstLoop == 0) { Run_count = 0; firstLoop = 1; }
	//double x = Uniform(g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0],1);
	
	//cout << "Run_count" << Run_count << endl;

	 

	//cout << "Run_count=" << Run_count << "\t RL=" << RL << "\t Basic Service Time=" << x << endl;
	//cout << "x=" << x << endl;
	double dBasicServiceTime;
	if (RL == 0 && Run_count == 0) {
		for (int i = 0; i < numTotalTimeFrame*numReqinUnitTimeFrame; i++) {
			serviceTime[i] = Uniform(g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0]-40, g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0] + 40);
			//serviceTime[i] = Exponential(g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0]);
		}

	}
	if (RL == 1) {
		 //dBasicServiceTime =Exponential(g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0] );
		dBasicServiceTime = Uniform(g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0] - 40, g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0] + 40);
	}
	else{
		dBasicServiceTime = serviceTime[Run_count];
		Run_count++; 
	}
	
	//double dBasicServiceTime = Exponential(g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0]*10);
	//double dBasicServiceTime = g_aMultifpleThreadInteferPara[g_servthreadNum - 1][0];
	pstReq->stTime.dBasicServiceTimeLength = dBasicServiceTime;

	if( 1  == g_machineNum )
	{
		pstReq->stTime.dComputingTime = dBasicServiceTime/g_machineNum;
		pstReq->stTime.dComputingTime = pstReq->stTime.dComputingTime * g_aMultifpleThreadInteferPara[g_servthreadNum-1][ulThreadParaIndex] / g_aMultifpleThreadInteferPara[g_servthreadNum-1][0];
									   //+ pstReq->stTime.dComputingTime * (g_aMultifpleThreadInteferPara[ulThreadParaIndex]-1) * g_MultifpleThreadSigleEventPara; //Feng
		pstReq->stTime.dCommunicationTime = 0;
	}
	else
	{
		pstReq->stTime.dComputingTime = dBasicServiceTime/g_machineNum;

		pstReq->stTime.dComputingTime = pstReq->stTime.dComputingTime * g_aMultifpleThreadInteferPara[g_servthreadNum-1][ulThreadParaIndex] / g_aMultifpleThreadInteferPara[g_servthreadNum-1][0];
									  // + pstReq->stTime.dComputingTime * (g_aMultifpleThreadInteferPara[ulThreadParaIndex]-1) * g_MultifpleThreadSigleEventPara;

		pstReq->stTime.dCommunicationTime = (dBasicServiceTime/g_machineNum) * (g_aNetworkingPercentange[g_machineNum-1]); 
		pstReq->stTime.dCommunicationTime = g_aNetworkingLatency[pstReq->ulRunningThreadNumInTheMachine/g_servthreadNum-1 ] 
											+ pstReq->stTime.dCommunicationTime * ( 1 + BANDWITHFACTOR * (g_machineNum * (pstReq->ulRunningThreadNumInTheMachine/g_servthreadNum ) ) );
	//	 cout << "Computing time=" << pstReq->stTime.dComputingTime << "\t Communication Time=" << pstReq->stTime.dCommunicationTime << endl;
	}

	pstReq->stTime.dServiceTimeLength = pstReq->stTime.dComputingTime + pstReq->stTime.dCommunicationTime;
	//cout << "Service Time length=" << pstReq->stTime.dServiceTimeLength << endl;
/*
	pstReq->stTime.dServiceTimeLength = (GetService())
		                                * g_aNetworkingInterferPara[g_machineNum]
		                                * g_aMultifpleThreadInteferPara[ulThreadParaIndex]
	                                    / g_machineNum;
*/
	pstReq->stTime.dCompletion = pstReq->stTime.dStartService + pstReq->stTime.dServiceTimeLength;
//	m_pCmachine->ReqCompletionCallBack(pstReq->ulReqid);

	return TRUE;
}

BOOL CThread::IsInuse()
{
	return m_bInuse;
}

void CThread::SetInuseFlag( BOOL bInuseFlag )
{
	m_bInuse = bInuseFlag;
}

void CThread::GetInuseFlag( BOOL& bInuseFlag )
{
	bInuseFlag = m_bInuse;
}
