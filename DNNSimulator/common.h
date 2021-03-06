#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <math.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "rngs.h"  
#include "rvgs.h"
using namespace std;

#define START			0.0              /* initial time                   */
#define STOP			122926.25//1761.8095              /* terminal (close the door) time */
#define INFINITY		(1000000.0 * STOP)      /* must be much larger than STOP  */

/* PARA SET */
#define FINAL			10000

//#define BURSTFILE		"1burstMMAPp08q030"

#define SERVISEINFO		"M-M-M-FCFS.txt"
#define OP_REQ_ID		"Req_id"
#define OP_REQ_MID		"Req_Machine_id"
#define OP_REQ_TID		"Req_Thread_id"
#define OP_REQ_TAR		"Req_Time_Arrival"
#define OP_REQ_TSS		"Req_Time_StartServe"
#define OP_REQ_TSL		"Req_Time_ServeLength"
#define OP_REQ_TCP		"Req_Time_Complete"
#define OP_REQ_TCMP		"Req_Time_Computing time"
#define OP_REQ_TCMC		"Req_Time_Communication time"

extern double g_arrivalmean;
extern int global_selected;
#define MEANARRTIME		800.0 //1.111111  //6  1.2   0.666667
#define MEANSERTIME		94.45  //0.00088 0.001056 0.001232 0.001408 0.001584 MEANSERTIME/thread num/arr~0.6~0.7

#define INVALIDE			0xFFFFFFFF


#define TRUE			0x00000000
#define FALSE			0xFFFFFFFF

#define BOOL			long
#define ULONG			unsigned long

//#define THREADNUM			16	// # thread serving in single machine
#define WHOLETHREADNUM		8	// # thread in single machine
#define MACHINENUM			4	// # available machines 
//#define SRVTHREADNUM		5	// # thread serving for one request(replica) in single machine
extern ULONG g_machineNum;
extern ULONG g_servthreadNumForMachine;
extern ULONG g_servthreadNum;
extern ULONG g_ReqMaxNum;
extern ULONG lRet;

#define OUTPUTTHRESHHOLD	100	// When the size of completed list come up to this threshold, write data to file and clear the list 
#define MAXQUEUELONGTH		100
// max lentgh of the queue
//#define REQWHOLENUM			100000	// # request to be proceeded.
#define BANDWITHFACTOR		0.02



// current time
struct GlobalTime{
	double dCurrentTime;
};

extern double dwait;
extern double dservice;
extern double dresponse;

extern double lastRequest_servTime;
extern double lastRequest_ArrTime;
extern double lastRequest_WaitTime;
extern double lastRequest_ResTime;
extern double lastTime_latency;



extern double beta;
extern int arrRates[];
struct stAct {   //for different actions
	int i_machineNum;
	int i_tot_th;			//g_servtheadNumforMachine
	int i_numThPerReq;	//g_servthreadNum
};

#define num_eachArrRateRepeats 300  // number of time frames each arrival_rate should repeat
#define num_ArrStates 10	//number of arrival_rates
#define num_Action 20		//number of configuration
#define numTotalTimeFrame 3000	//number of times arrival_state repeats
#define numReqinUnitTimeFrame 30//number of requests in one time interval


struct stState {			//for different arrival (states)
	int i_sel[num_eachArrRateRepeats+1];//num_eachArrRateRepeats (yo ra following cumtable samma)
	long double d_rand[num_eachArrRateRepeats+1];
	long double d_PI[num_Action][num_eachArrRateRepeats+1];
	long double d_Vs[num_Action][num_eachArrRateRepeats+1];
	long double d_cumTable[num_Action][num_eachArrRateRepeats+1];
	int i_stateId;
	int i_state_pos;
	int i_loopCount;
};

extern double beta;
extern int arrRates[];
extern struct stAct stAction[];
extern struct stState stStates[];
extern long double average_latencies_array[];
extern long double CumEach_latency[];
extern long double each_latency[];
extern int countt_ForMix;
extern int count1_ForMix;


extern int fflag;
extern int arr_num[];
extern int timed_loop;
extern int tarr_pos;
extern long double cost;

extern int arr_num[];
extern int timed_loop;
extern int tarr_pos;
extern long double cost;
extern long double series_arr[];
extern double last_finishTime;
extern long int compCount;
extern long double mix_arrival[];
extern long double basic_serviceTime[];
extern long double service[];
extern long int sel_activity[];

// record kinds of times of processing certain request
 struct stReqTime{
    double dArrival;                 /* arrival time                   */
    double dStartService;                 /* starting-service time                   */
	double dBasicServiceTimeLength;                   // job service time
	double dServiceTimeLength;                   // job service time
	double dComputingTime;
	double dCommunicationTime;
    double dCompletion;              /* completion time                */
 //   double dCurrent;                 /* current time                        */
 //   double dNext;                    /* next (most imminent) event time     */
 //   double dLast;                    /* last arrival time                   */
  };

  struct stArea{
    double node;                    /* time integrated number in the node  */
    double queue;                   /* time integrated number in the queue */
    double service;                 /* time integrated number in service   */
  };

  // machines and threads in service for certain request.
  struct stMTids{
     ULONG ulMachineid;                    
	 ULONG ulThreadid;                   
  };

  // request
  struct stReq{
//	long    lSource;
//	ULONG   ulMachineid;						//YF: the index in the queue
//	ULONG   ulThreadid;						//YF: the index in the queue
//	double  dRemain;
//	long    lOrder;
//	long D;							//YF: limit delay time
	ULONG   ulReqid;						//YF: the index in the queue
	ULONG	ulRunningThreadNumInTheMachine;
	stReqTime stTime;
 	vector <stMTids> vstMTidlist;
 };

  extern double g_MultifpleThreadSigleEventPara;
/*#if (SRVTHREADNUM == 1)
  static double			g_MultifpleThreadSigleEventPara = 0.0; //multiple thread in a machine serving one event.
#else
  static double			g_MultifpleThreadSigleEventPara = 0.25; //multiple thread in a machine serving one event.
#endif
*/
  static int			g_iDefMachineNum;
  static int			g_iDefThreadNum;
  static ULONG			g_ulReqWholeNum; // # requests generated until current time 
  static GlobalTime		g_stGlobalTime;  // Time
  static ULONG			g_ulProcThreadNum = 0; // # thread in serving in single machine
  extern int cc_count;
  extern int RL; // for knowing that the system is working at RL algorithm and not SC; for reusing the same arrival rates at getArrival function
  /*
  const double			g_aMultifpleThreadInteferPara[8][8] = {{94.45, 52.35, 38.62, 32.18, 27.96, 25.74, 23.94, 22.51},  
																{95.46, 52.28, 39.45, 34.34, 0.0, 0.0, 0.0, 0.0},
																{97.17, 54.35, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
																{95.89, 55.83, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
																{96.62, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
																{98.09, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
																{99.72, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
																{102.19, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
															  }; //thread interference para
															  
															  
  const double			g_aMultifpleThreadInteferPara[8][8] = { { 18.64, 18.79, 19.1, 19.54, 19.97, 20.54, 21.24, 21.97 },
																  { 11.71, 11.9, 12.1, 12.32, 0.0, 0.0, 0.0, 0.0 },
																  { 9.71, 9.94, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																  { 8.69,8.92, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																  { 8.24,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																  { 7.84, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																  { 7.68, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																  { 7.35, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
																  }; //untransposed 
																  */
			/*													  
  const double			g_aMultifpleThreadInteferPara[8][8] = { { 777.94, 804.94, 1629.55, 2365.52, 3008.2, 3602.38, 4186.57, 4804.04 },
															  { 426.84, 504.07, 969.53, 1328.28, 0.0, 0.0, 0.0, 0.0 },
															  { 314.98, 390.28, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
															  { 259.76, 347.49, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
															  { 225.88,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
															  { 205.16, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
															  { 191.27, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
															  { 177.69, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
															  }; 
															  
  */
	  const double			g_aMultifpleThreadInteferPara[8][8] = { { 236.76, 240.4, 242.9, 241.3, 235.05, 236.89, 239.25, 243.88 },
																	  { 141.69, 146.72, 149.68, 148.34, 0.0, 0.0, 0.0, 0.0 },
																	  { 111.46, 117.45, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																	  { 97.66, 105.73, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																	  { 90.34,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																	  { 83.64, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																	  { 81.62, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
																	  { 76.94, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
																															  }; //new two not_trans 
														 
														 
//  const double			g_aMultifpleThreadInteferPara[16] = {1,1,1,1,1064.200746/848.558618,1170.918359/848.558618,1334.443593/848.558618,1449.305691/848.558618,1617.662343/848.558618,1729.033354/848.558618,1907.093680/848.558618,2027.695473/848.558618,2195.909546/848.558618,2342.152850/848.558618,2524.747003/848.558618,2685.351988/848.558618}; //thread interference para
//  const double			g_aNetworkingInterferPara[20] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; // comunication interference para
  const double			g_aNetworkingPercentange[20] = {0, 120.0/360, 105.0/270, 90.0/180, 82.0/144,76.0/120,71.0/103,68.0/90,63.0/84,58.0/78,53.0/72,48.0/66,44.0/60,39.0/54,34.0/48,40.0/45,41.0/45,42.0/45,43.0/45,44.0/45}; // comunication  interference para
  const double			g_aNetworkingLatency[16] = {0, 2.0*2.5/16 ,3.0*2.5/16,4.0*2.5/16,5.0*2.5/16,6.0*2.5/16,7.0*2.5/16,8.0*2.5/16,9.0*2.5/16,10.0*2.5/16,11.0*2.5/16,12.0*2.5/16,13.0*2.5/16,14.0*2.5/16,15.0*2.5/16.0,16.0*2.5/16}; // comunication interference para


extern "C" {

	BOOL Cmp(double a, double c);

	double Min(double a, double c);

    double Max(double a, double c);

	double GetAverage(double number, double numcount, double mean);

//   double Exponential(double m);


//   double Uniform(double a, double b);
	void init(void);

  double GetArrival(int g_UlReqWholeNum);

  double GetService();
  void work(void);

  void printMatrix(double *matrix, int r, int c);
  void Rl_algo(stState *stStates,int arrPos, long double cost);
  void Rl_init(stState *stStates, int arrPos);
  void Rl_run(stState *stStates, int arrPos, int ranPos);
  void printMatrix_int(int *matrix, int r, int c);
}

#endif