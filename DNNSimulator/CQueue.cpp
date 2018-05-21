#include "CQueue.h"
#include "CMachine.h"
#include "common.h"


CQueue::CQueue(ULONG ulMaxLength)
{
	g_ulReqWholeNum = 0;
	memset(&g_stGlobalTime, 0, sizeof(GlobalTime));

	m_bReqCompletion = FALSE;
	m_bFull = FALSE;
	m_bMachFullRunning = FALSE;
	m_ulCompleteReqNum = 0;
}

CQueue::~CQueue()
{
	//release machines
	for(ULONG ulMachineIndex = 0; ulMachineIndex < MACHINENUM; ulMachineIndex++ )
	{
		if(NULL != m_vcpMachinelist[ulMachineIndex])
		{
			m_vcpMachinelist[ulMachineIndex]->DestroyMachine();
			delete m_vcpMachinelist[ulMachineIndex];
			m_vcpMachinelist[ulMachineIndex] = NULL;
		}
	}
}

long CQueue::CreateQueue()
{
	long lRet = TRUE;

	CMachine* pcMachine = NULL;

	for(ULONG ulMachine = 0; ulMachine < MACHINENUM; ulMachine++ )
	{
		pcMachine = new CMachine;
		if(NULL == pcMachine)
		{
			lRet = FALSE;
			return lRet;
		}

lRet = pcMachine->CreateMachine(ulMachine, this);

m_vcpMachinelist.push_back(pcMachine);

	}

	return lRet;
}

long CQueue::DestroyQueue()
{
	long lRet = TRUE;

	//release machines
	for (ULONG ulMachineIndex = 0; ulMachineIndex < MACHINENUM; ulMachineIndex++)
	{
		if (NULL != m_vcpMachinelist[ulMachineIndex])
		{
			m_vcpMachinelist[ulMachineIndex]->DestroyMachine();
			delete m_vcpMachinelist[ulMachineIndex];
			m_vcpMachinelist[ulMachineIndex] = NULL;
		}
	}

	m_bReqCompletion = FALSE;
	m_bFull = FALSE;
	m_bMachFullRunning = FALSE;
	m_ulCompleteReqNum = 0;

	return lRet;
}

long CQueue::Process()
{
	long lRet = TRUE;
	double dmeanWait = 0;
	double dmeanServe = 0;
	double dmeanResponse = 0;
	double dmeanCompute = 0;
	double dmeanCommunication = 0;
	static int compCount = 0;
	static int ss_CompletedReq;
	if (fflag == 0) {
		ss_CompletedReq = 0;
		fflag = 1;
	}
	


	PlantSeeds(123456789);

	while (1)
	{

		if (FALSE == m_bReqCompletion)
		{
			
			if ((m_vstRequestList.size() < MAXQUEUELONGTH) && (!(g_ReqMaxNum == g_ulReqWholeNum) || (g_ReqMaxNum < g_ulReqWholeNum)))
			{
				stReq stNewReq;
				MakeReq(stNewReq);
				m_vstRequestList.push_back(stNewReq);
				g_ulReqWholeNum++;
				cout << "last_finishTime top" << last_finishTime << endl;
				
				//cout << "g_ulReqWholeNum=" << g_ulReqWholeNum << endl;
			}

			if (m_vstProcessingRequestList.size() > 0)
			{
				if (m_vstRequestList.size() > 0)
				{
					if (FALSE == Cmp(m_vstRequestList[0].stTime.dArrival,
						m_vstProcessingRequestList[0].stTime.dCompletion))
					{
						m_bReqCompletion = TRUE;

						continue;
					}
				}
				else
				{
					m_bReqCompletion = TRUE;

					continue;
				}
			}

			if (m_vstRequestList.size() > 0)
			{

				JudgeFull();

				if (FALSE == m_bMachFullRunning)
				{
					if (1 == g_ulReqWholeNum && cc_count==1 && 1==RL)
					{
						g_stGlobalTime.dCurrentTime = m_vstRequestList[0].stTime.dArrival;
					}
					else if (g_ulReqWholeNum == 1 && 1 == cc_count && 0 == RL) { // for the first request of SC, arrival time should be 0 
						m_vstRequestList[0].stTime.dStartService = m_vstRequestList[0].stTime.dArrival;
					}
					else 
					{
  						g_stGlobalTime.dCurrentTime = max(m_vstRequestList[0].stTime.dArrival, g_stGlobalTime.dCurrentTime);
					}
					/*
					if (240 == m_vstRequestList[0].ulReqid)//for test
					{
						int itest = 0;
					}*/
					
					if(m_ulCompleteReqNum == 0 && cc_count!=1){  //from one time frame to another time frame of same RL or SC process, the wait time must be added on
						m_vstRequestList[0].stTime.dStartService = max( last_finishTime, m_vstRequestList[0].stTime.dArrival);   //for 2000
					}
					else 
					m_vstRequestList[0].stTime.dStartService = g_stGlobalTime.dCurrentTime;
					
					GetAvilableMachine(&(m_vstRequestList[0]));
					lRet = m_vcpMachinelist[0]-> run(&(m_vstRequestList[0]));
					 
					m_vstProcessingRequestList.push_back(m_vstRequestList[0]);
	
					UpServTmForItfparaCng(TRUE);

					m_vstRequestList.erase(m_vstRequestList.begin());
				}
				else
				{
						m_bReqCompletion = TRUE;

						continue;
				}
			}
		}
		else
		{
			if (m_vstProcessingRequestList.size() > 0)
			{
				if (240 == m_vstProcessingRequestList[0].ulReqid)//for test
				{
					int itest = 0;
				}

				//set currenttime as completion time
				g_stGlobalTime.dCurrentTime = m_vstProcessingRequestList[0].stTime.dCompletion;


				UpServTmForItfparaCng(FALSE);
				ULONG ulTempMachineID = 0;
				for (ULONG ulvecLoop = 0; ulvecLoop < m_vstProcessingRequestList[0].vstMTidlist.size(); ulvecLoop++)
				{
					ulTempMachineID = m_vstProcessingRequestList[0].vstMTidlist[ulvecLoop].ulMachineid;

					m_vcpMachinelist[ulTempMachineID]->RleaseOneThread(&(m_vstProcessingRequestList[0]));

				}

				m_bMachFullRunning = FALSE;
				m_bReqCompletion = FALSE;
				//cout << "_______________________" << endl;
				cout << "Request No="<< m_ulCompleteReqNum<< "completed in " << m_vstProcessingRequestList[0].stTime.dCompletion << endl;
				// 
				cout << "Request No=" << m_ulCompleteReqNum << "wait time is " << m_vstProcessingRequestList[0].stTime.dStartService - m_vstProcessingRequestList[0].stTime.dArrival << endl;
				cout << "Request N0=" << m_ulCompleteReqNum << "\t Arrival:" << m_vstProcessingRequestList[0].stTime.dArrival << "\t Start:" << m_vstProcessingRequestList[0].stTime.dStartService << "\t Completion=" << m_vstProcessingRequestList[0].stTime.dCompletion << "\t Wait=" << m_vstProcessingRequestList[0].stTime.dStartService - m_vstProcessingRequestList[0].stTime.dArrival<<"\t Response time="<< m_vstProcessingRequestList[0].stTime.dCompletion - m_vstProcessingRequestList[0].stTime.dArrival << endl;
				//cout << "_________________________" << endl;
				m_ulCompleteReqNum++;


				ss_CompletedReq++;
				//cout << "ss_completedReq=" << ss_CompletedReq << endl;
				m_vstCompletedRequestList.push_back(m_vstProcessingRequestList[0]);
				
					dwait = m_vstProcessingRequestList[0].stTime.dStartService - m_vstProcessingRequestList[0].stTime.dArrival;
					dservice = m_vstProcessingRequestList[0].stTime.dCompletion - m_vstProcessingRequestList[0].stTime.dStartService;
					dresponse = m_vstProcessingRequestList[0].stTime.dCompletion - m_vstProcessingRequestList[0].stTime.dArrival;
				
				dmeanWait = GetAverage( dwait, m_ulCompleteReqNum, dmeanWait);
				dmeanServe = GetAverage( dservice, m_ulCompleteReqNum, dmeanServe);
				dmeanResponse  = GetAverage( dresponse, m_ulCompleteReqNum, dmeanResponse);
				dmeanCompute = GetAverage( m_vstProcessingRequestList[0].stTime.dComputingTime, m_ulCompleteReqNum, dmeanCompute);
				dmeanCommunication = GetAverage( m_vstProcessingRequestList[0].stTime.dCommunicationTime, m_ulCompleteReqNum, dmeanCommunication);
				
				//cout << "dmeanWait=" << dmeanWait << "\t dmeanServe=" << dmeanServe << "\t dmeanResponse=" << dmeanResponse << "\t dmeanComputer=" << dmeanCompute << endl;
				//cout << "m_ulCOmpleteReqNum=" << m_ulCompleteReqNum << endl;
				last_finishTime = m_vstProcessingRequestList[0].stTime.dCompletion;
				//cout << "last_finishtime=" << last_finishTime << endl;

				m_vstProcessingRequestList.erase(m_vstProcessingRequestList.begin());

				//cout << "dmeanResponse=" << dmeanResponse;
				double temp_servTime = dservice;
				double temp_ArrTime = m_vstProcessingRequestList[0].stTime.dStartService;
				double temp_WaitTime = dwait;
				double temp_ResTime = dresponse;
				//cout << "completed Count=" << s_CompletedReq << endl;
				//cout << "\t dresponse=" << dresponse << endl;
			
				//if (ss_CompletedReq > 10000)
				{
					CumEach_latency[cc_count] = CumEach_latency[cc_count-1] + dresponse;
					each_latency[cc_count] = dresponse;
					cout << "cc_count==" << cc_count << endl;
					cc_count++;
				}
				m_vstCompletedRequestList.clear();
				
				if( g_ReqMaxNum == m_ulCompleteReqNum)
				{
					lastRequest_servTime = temp_servTime;
					lastRequest_ArrTime = temp_ArrTime;
					lastRequest_WaitTime = temp_WaitTime;
					lastRequest_ResTime = temp_ResTime;
					
					lastTime_latency = dmeanResponse;

				//	cout << "Serv=" << lastRequest_servTime << "\t Arr=" << lastRequest_ArrTime << "\t Wait=" << lastRequest_WaitTime << "\t Res==" << lastRequest_ResTime << "\t mean response="<< lastTime_latency<<endl;
					break;
				}
			}
		}
	
	}

	ULONG ulNumOfReqProcessAtSameTime = (MACHINENUM/g_machineNum)*(g_servthreadNumForMachine/g_servthreadNum);


	//printf("r%dt%d\t%6.2f\t%6.2f\t%6.2f\t%6.2f\n",ulNumOfReqProcessAtSameTime,g_servthreadNum,g_arrivalmean,dmeanServe,dmeanWait,dmeanResponse );
	return lRet;
}

long CQueue::ReqCompletionCallBack(ULONG ulReqid)
{
//	assert( NULL != m_vstRequestList[ulReqid] );

	long lRet = TRUE;

	stReq* pstReq = &(m_vstRequestList[ulReqid]);
	pstReq->stTime.dCompletion = pstReq->stTime.dStartService + pstReq->stTime.dServiceTimeLength;


	return lRet;
}

void CQueue::JudgeFull()
{
	m_bMachFullRunning = FALSE;
	m_ulIdleMachineNum = 0;

	for(ULONG ulMachine = 0; ulMachine < m_vcpMachinelist.size(); ulMachine++)
	{
		if ( FALSE == m_vcpMachinelist[ulMachine]->IsFullRunning())
		{
			m_ulIdleMachineNum++;
		}
	}

	if( m_ulIdleMachineNum < g_machineNum )
	{
		m_bMachFullRunning = TRUE;
	}

	return ;
}

BOOL CQueue::IsFull()
{

	return m_bMachFullRunning;
}

long CQueue::MakeReq(stReq& stNewReq)
{
	long lRet = TRUE;

	memset(&stNewReq, 0, sizeof(stNewReq));

	stNewReq.ulReqid = g_ulReqWholeNum;
	stNewReq.stTime.dArrival = GetArrival(g_ulReqWholeNum);
	stNewReq.stTime.dServiceTimeLength = INFINITY;
	stNewReq.stTime.dCompletion = INFINITY;

	return lRet;
}

long CQueue::GetAvilableMachine(stReq* pstReq)
{
	long lRet = TRUE;
	int iMachine = g_machineNum;

	for(ULONG ulMachine = 0; ulMachine < m_vcpMachinelist.size(); ulMachine++)
	{
		if ( FALSE == m_vcpMachinelist[ulMachine]->IsFullRunning())
		{
			lRet = m_vcpMachinelist[ulMachine]->GetAvilableThread( pstReq );

			iMachine--;
		}
//		else
//		{
//			assert(0);
//			break;
//		}

		if(0 == iMachine)
		{
			break;
		}

	}

	JudgeFull();
	
	return lRet;
}

void CQueue::UpServTmForItfparaCng(BOOL bThreadNumIncr)
{
	double dServedTime = 0;
	double dRemainedTime = 0;
	ULONG ulPreThreadParaIndex = 0;
	ULONG ulLoopEndCond = 0;
	double dTempCommTime = 0;
	double dTempComputTime = 0;
	double dPercentage = 1;
	ULONG ulProcThreadParaIndex = 0;
	ULONG ulAimMachineID = 0;

	if(TRUE == bThreadNumIncr)
	{
		ulAimMachineID = m_vstRequestList[0].vstMTidlist[0].ulMachineid;
	}
	else
	{
		ulAimMachineID = m_vstProcessingRequestList[0].vstMTidlist[0].ulMachineid;
	}

	for(ULONG uloop = 0; uloop < m_vstProcessingRequestList.size(); uloop++ )
	{
		if( ulAimMachineID == m_vstProcessingRequestList[uloop].vstMTidlist[0].ulMachineid)
		{
			ulProcThreadParaIndex++;
			
		}
	}

	if(ulProcThreadParaIndex == 17)
	{
		for(ULONG uloop = 0; uloop < m_vstProcessingRequestList.size(); uloop++ )
		{
			if( ulAimMachineID == m_vstProcessingRequestList[uloop].vstMTidlist[0].ulMachineid)
			{
				printf("%u\t%u\t%u\n", m_vstProcessingRequestList[uloop].ulReqid, 
					                   m_vstProcessingRequestList[uloop].vstMTidlist[0].ulMachineid,
									   m_vstProcessingRequestList[uloop].vstMTidlist[0].ulThreadid);
			}
		}
	}

	if((0 == ulProcThreadParaIndex) || (1 == ulProcThreadParaIndex) )
	{
		return;
	}


	if(TRUE == bThreadNumIncr)
	{
		ulProcThreadParaIndex-=1;
		ulPreThreadParaIndex = ulProcThreadParaIndex - 1;//-new
		ulLoopEndCond = m_vstProcessingRequestList.size()-1;//not update the new one
	}
	else
	{
		ulProcThreadParaIndex-=2;
		ulPreThreadParaIndex = ulProcThreadParaIndex + 1;	//+ completing one	
		ulLoopEndCond = m_vstProcessingRequestList.size(); // update all remains

	}

	for( ULONG ulLoop = 0; ulLoop < ulLoopEndCond; ulLoop++ )
	{
		if(ulAimMachineID == m_vstProcessingRequestList[ulLoop].vstMTidlist[0].ulMachineid)
		{
	/*		
			dServedTime = g_stGlobalTime.dCurrentTime - m_vstProcessingRequestList[ulLoop].stTime.dStartService;
			dRemainedTime = m_vstProcessingRequestList[ulLoop].stTime.dServiceTimeLength - dServedTime;

			m_vstProcessingRequestList[ulLoop].stTime.dServiceTimeLength 
				= dServedTime 
				+ (dRemainedTime/g_aMultifpleThreadInteferPara[ulPreThreadParaIndex])
				  *g_aMultifpleThreadInteferPara[ulProcThreadParaIndex];
	*/

			if(240==m_vstProcessingRequestList[ulLoop].ulReqid)//for test
			{
				int itest = 0;
			}

			dServedTime = g_stGlobalTime.dCurrentTime - m_vstProcessingRequestList[ulLoop].stTime.dStartService;
			dRemainedTime = m_vstProcessingRequestList[ulLoop].stTime.dServiceTimeLength - dServedTime;

			if(dRemainedTime == 0)
			{
				continue;
			}

			dPercentage = m_vstProcessingRequestList[ulLoop].stTime.dComputingTime/m_vstProcessingRequestList[ulLoop].stTime.dServiceTimeLength;
			
			dTempComputTime = dRemainedTime*dPercentage;
			dTempCommTime = dRemainedTime-dTempComputTime;


			if( 1  == g_machineNum )
			{
				m_vstProcessingRequestList[ulLoop].stTime.dComputingTime
								= dTempComputTime / (g_aMultifpleThreadInteferPara[g_servthreadNum-1][ulPreThreadParaIndex] / g_aMultifpleThreadInteferPara[g_servthreadNum-1][0])
													 //+ (g_aMultifpleThreadInteferPara[ulPreThreadParaIndex]-1) * g_MultifpleThreadSigleEventPara)
												  * (g_aMultifpleThreadInteferPara[g_servthreadNum-1][ulProcThreadParaIndex] / g_aMultifpleThreadInteferPara[g_servthreadNum-1][0]);
													 //+ (g_aMultifpleThreadInteferPara[ulProcThreadParaIndex]-1) * g_MultifpleThreadSigleEventPara);

				m_vstProcessingRequestList[ulLoop].stTime.dCommunicationTime = 0;
			}
			else
			{
				m_vstProcessingRequestList[ulLoop].stTime.dComputingTime
								= dTempComputTime * (g_aMultifpleThreadInteferPara[g_servthreadNum-1][ulPreThreadParaIndex] / g_aMultifpleThreadInteferPara[g_servthreadNum-1][0])
													 //+ (g_aMultifpleThreadInteferPara[ulPreThreadParaIndex]-1) * g_MultifpleThreadSigleEventPara)
												  * (g_aMultifpleThreadInteferPara[g_servthreadNum-1][ulProcThreadParaIndex] / g_aMultifpleThreadInteferPara[g_servthreadNum-1][0]);
													 //+ (g_aMultifpleThreadInteferPara[ulProcThreadParaIndex]-1) * g_MultifpleThreadSigleEventPara);
												  
				if( 0 != m_vstProcessingRequestList[ulLoop].stTime.dCommunicationTime)
				{
					m_vstProcessingRequestList[ulLoop].stTime.dCommunicationTime = dTempCommTime * ( 1 + BANDWITHFACTOR * (g_machineNum * (ulProcThreadParaIndex+1)) )
																								 / ( 1 + BANDWITHFACTOR * (g_machineNum * (ulPreThreadParaIndex+1)) )
																				   + g_aNetworkingLatency[ulProcThreadParaIndex]
																				   - dTempCommTime/m_vstProcessingRequestList[ulLoop].stTime.dCommunicationTime * g_aNetworkingLatency[ulPreThreadParaIndex];
				}

			}

			m_vstProcessingRequestList[ulLoop].stTime.dServiceTimeLength = dServedTime 
																		   + m_vstProcessingRequestList[ulLoop].stTime.dComputingTime 
																		   + m_vstProcessingRequestList[ulLoop].stTime.dCommunicationTime;

			m_vstProcessingRequestList[ulLoop].stTime.dComputingTime = dServedTime * dPercentage 
																		+ m_vstProcessingRequestList[ulLoop].stTime.dComputingTime;

			m_vstProcessingRequestList[ulLoop].stTime.dCommunicationTime = m_vstProcessingRequestList[ulLoop].stTime.dServiceTimeLength
																			- m_vstProcessingRequestList[ulLoop].stTime.dComputingTime;

			m_vstProcessingRequestList[ulLoop].stTime.dCompletion 
				= m_vstProcessingRequestList[ulLoop].stTime.dStartService
				+ m_vstProcessingRequestList[ulLoop].stTime.dServiceTimeLength;
		}
	}

	//sort by completion time
	sort(m_vstProcessingRequestList.begin(),m_vstProcessingRequestList.end(), VecCmpByCmpt);
}

int VecCmpByArr(const stReq& lReq, const stReq& rReq)
{
	return lReq.stTime.dArrival < rReq.stTime.dArrival;
}

int VecCmpBySSV(const stReq& lReq, const stReq& rReq)
{
	return lReq.stTime.dStartService < rReq.stTime.dStartService;
}

int VecCmpBySVL(const stReq& lReq, const stReq& rReq)
{
	return lReq.stTime.dServiceTimeLength < rReq.stTime.dServiceTimeLength;
}

int VecCmpByCmpt(const stReq& lReq, const stReq& rReq)
{
	return lReq.stTime.dCompletion < rReq.stTime.dCompletion;
}

