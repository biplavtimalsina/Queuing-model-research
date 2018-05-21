#include "common.h"
#include "CQueue.h"
//change the processing time by using random generator (rather than using the parameters) so that the optimum configuration changes
	//how many number of total time frame 

extern ULONG g_machineNum = 2;
extern ULONG g_servthreadNum = 2;
extern ULONG g_servthreadNumForMachine = 8;
extern double g_MultifpleThreadSigleEventPara = 0.0;
extern double g_arrivalmean = 200.0;
extern ULONG g_ReqMaxNum = 20;


extern int arrRates[] = { 70,80,90,100,150,200, 400,500,1000,2000 };
//extern int arrRates[] = { 100,150,200,250,260,270,290,295,300,500 };
//extern int arrRates[] = { 20,20,20,20,20,20,20,20,20,20 };

//extern int arrRates[]=  {5,5,5,5,5,5,5,5,5,5};
//extern int arrRates[] = { 25,25,25,25,25,25,25,25,25,25 };
//extern int arrRates[] = { 35,35,35,35,35,35,35,35,35,35 };
//extern int arrRates[] = { 40,40,40,40,40,40,40,40,40,40 };
//extern int arrRates[]=  {50,50,50,50,50,50,50,50,50,50};
//extern int arrRates[] = { 60,60,60,60,60,60,60,60,60,60 };
//extern int arrRates[] = { 70,70,70,70,70,70,70,70,70,70 };
//extern int arrRates[] = { 75,75,75,75,75,75,75,75,75,75 };
//extern int arrRates[] = { 80,80,80,80,80,80,80,80,80,80 };
//extern int arrRates[] = { 90,90,90,90,90,90,90,90,90,90 };
//extern int arrRates[] = { 100,100,100,100,100,100,100,100,100,100 };
//extern int arrRates[]={110,110,110,110,110,110,110,110,110,110};
//extern int arrRates[] = {120,120,120,120,120,120,120,120,120,120};
//extern int arrRates[] = {130,130,130,130,130,130,130,130,130,130,130};
//extern int arrRates[] = {140,140,140,140,140,140,140,140,140,140};
//extern int arrRates[] = { 150,150,150,150,150,150,150,150,150,150 };
//extern int arrRates[] = { 200,200,200,200,200,200,200,200,200,200 };
//extern int arrRates[] = {250,250,250,250,250,250,250,250,250,250};
//extern int arrRates[] = {270,270,270,270,270,270,270,270,270,270};
//extern int arrRates[] = { 400,400,400,400,400,400,400,400,400,400 };
//extern int arrRates[] = { 500,500,500,500,500,500,500,500,500,500 };
//extern int arrRates[] = { 1000,1000,1000,1000,1000,1000,1000,1000,1000,1000 };
//extern int arrRates[] = { 2000,2000,2000,2000,2000,2000,2000,2000,2000,2000 };







extern double last_finishTime=0;
struct stAct stAction[num_Action];
struct stState stStates[num_ArrStates];
extern double beta = 0.5;
extern int fflag = 0;
//extern int arr_num[] = { 80,60,57,50,40,25,20,10,7,5 };
//extern int arr_num[] = { 5,5,5,5,5,5,5,5,5,5,5 };
//extern int arr_num[] = { 8,8,8,8,8,8,8,8,8,8,8 };
//extern int arr_num[] = { 9,9,9,9,9,9,9,9,9,9 };
//extern int arr_num[] = { 10,10,10,10,10,10,10,10,10,10 };
//extern int arr_num[] = { 11,11,11,11,11,11,11,11,11,11 };
//extern int arr_num[]={12,12,12,12,12,12,12,12,12,12};
//extern int arr_num[] = { 13,13,13,13,13,13,13,13,13,13 };
//extern int arr_num[] = { 14,14,14,14,14,14,14,14,14,14 };
//extern int arr_num[] = { 15,15,15,15,15,15,15,15,15,15 };
//extern int arr_num[] = { 16,16,16,16,16,16,16,16,16,16 };
//extern int arr_num[] = { 17,17,17,17,17,17,17,17,17,17 };
//extern int arr_num[] = { 18,18,18,18,18,18,18,18,18,18 };
//extern int arr_num[] = { 19,19,19,19,19,19,19,19,19,19 };
//extern int arr_num[] = { 20,20,20,20,20,20,20,20,20,20 };
//extern int arr_num[] = { 21,21,21,21,21,21,21,21,21,21 };
//extern int arr_num[] = { 22,22,22,22,22,22,22,22,22,22 };
//extern int arr_num[] = { 23,23,23,23,23,23,23,23,23,23 };
//extern int arr_num[] = { 24,24,24,24,24,24,24,24,24,24 };
extern int arr_num[] = { 30,30,30,30,30,30,30,30,30,30 };
//extern int arr_num[] = { 35,35,35,35,35,35,35,35,35,35 };
//extern int arr_num[] = { 40,40,40,40,40,40,40,40,40,40 };
//extern int arr_num[] = { 50,50,50,50,50,50,50,50,50,50 };
//extern int arr_num[] = { 75,75,75,75,75,75,75,75,75,75 };
//extern int arr_num[] = { 100,100,100,100,100,100,100,100,100,100 };
//extern int arr_num[] = { 200,200,200,200,200,200,200,200,200,200 };
//extern int arr_num[]={300,300,300,300,300,300,300,300,300,300};
//extern int arr_num[] = { 500,500,500,500,500,500,500,500,500,500 };

extern int timed_loop = 0;
extern int tarr_pos = 0;
extern long double cost = 0;
extern long double series_arr[numTotalTimeFrame] = { 0 };
extern int global_selected = 0;
extern long double mix_arrival[numTotalTimeFrame*numReqinUnitTimeFrame] = {0.0}; // for recording the arrival rates in a mix situation and reusing it for SC paper
extern long double basic_serviceTime[numTotalTimeFrame*numReqinUnitTimeFrame*2] = { 0.0 };		//to randomly generate basic service time for operations

extern double dwait = 0;
extern double dservice = 0;
extern double dresponse = 0;

extern double lastRequest_servTime=0;
extern double lastRequest_ArrTime=0;
extern double lastRequest_WaitTime=0;
extern double lastRequest_ResTime=0;
extern double lastTime_latency=0;
extern int cc_count = 1;
double tempRande = 0;
extern long int compCount = 0;
extern int RL = 1;
extern int countt_ForMix = 0;
extern int count1_ForMix = 0;

double ran_number[numTotalTimeFrame] = { 0 };
extern long double average_latencies_array[numTotalTimeFrame] = { 0.0 };	//average latency of each time frame processed
extern long double CumEach_latency[numTotalTimeFrame*numReqinUnitTimeFrame] = { 0.0 };    //cummulative latency of each request processed; numTotalTimeFrame*50
extern long double each_latency[numTotalTimeFrame*numReqinUnitTimeFrame] = { 0.0 };	//latency of each request processed
extern long double service[numTotalTimeFrame*numReqinUnitTimeFrame] = { 0.0 };
extern long int sel_activity[numTotalTimeFrame] = { 0 };




void Rl_run(stState *stStates, int arrPos, int ranPos) {
	long double tempPI[num_Action] = { 0.0 };
	long double tempVs[num_Action] = { 0.0 };
	long double temp_cumTable[num_Action] = { 0.0 };
	
	int temploopCount = stStates[arrPos].i_loopCount;
	int  tempsel = stStates[arrPos].i_sel[temploopCount - 1];


	//		select a random number
	tempRande = ran_number[ranPos];
	//cout << "Random Number" << tempRande << endl;

	//copy all the elements of previous time period to this time
	for (int i = 0; i < num_Action; i++) {
		tempVs[i] = stStates[arrPos].d_Vs[i][temploopCount ];
	}
	//printMatrix((double *)tempVs, 8, 1);
	//cout << "_____________________________________________" << endl;
	for (int i = 0; i < num_Action; i++) {
		temp_cumTable[i] = stStates[arrPos].d_cumTable[i][temploopCount ];
	}
	//cout << "temporary copied PI from previous setting&&&&&&&&&&&&" << endl;
	//printMatrix((double *)temp_cumTable, 8, 1);

	//find out which interval does the random value lies in
	for (int i = 0; i < num_Action; i++) {
		if (tempRande < temp_cumTable[i]) {
			tempsel = i;
			global_selected = i;
			break;
		}
	}
	//cout << "Activity Selected=" << tempsel << endl;
	sel_activity[cc_count] = tempsel;
	stStates[arrPos].i_sel[temploopCount] = tempsel;
	stStates[arrPos].d_rand[temploopCount] = tempRande;
	stStates[arrPos].i_loopCount = temploopCount;


	g_servthreadNumForMachine = stAction[stStates[arrPos].i_sel[stStates[arrPos].i_loopCount]].i_tot_th;   //+1 because the action (4,1,1) position 0, so have to add 1 and likewise
	g_servthreadNum = stAction[stStates[arrPos].i_sel[stStates[arrPos].i_loopCount]].i_numThPerReq;
	g_machineNum = stAction[stStates[arrPos].i_sel[stStates[arrPos].i_loopCount]].i_machineNum;
	//cout << "* arrival mean" <<g_arrivalmean<< endl;
	//cout << "State pos" << stStates[arrPos].i_state_pos << endl;
	g_arrivalmean = arrRates[stStates[arrPos].i_state_pos];
	//cout << "*" << endl;

	//cout << "g_servthreadNumForMachine" << g_servthreadNumForMachine << endl;
	//cout << "g_servthreadNum" << g_servthreadNum << endl;
	//cout << "HI" << endl;
	stStates[arrPos].i_loopCount++;
	
}

void Rl_init(stState *stStates, int arrPos)
{
	//select a random number
	(stStates + arrPos)->d_rand[0] = Random();
	//(stStates + arrPos)->d_rand[0] = 0;
	//cout << "random no==" << (stStates + arrPos)->d_rand[0] << endl;

	//form the cummulative table of PI
	(stStates+arrPos)->d_cumTable[0][0] = (stStates + arrPos)->d_PI[0][0];
	for (int i = 1; i < num_Action; i++) {
		(stStates + arrPos)->d_cumTable[i][0] = (stStates + arrPos)->d_cumTable[i - 1][0] + (stStates + arrPos)->d_PI[i][0];
	}
	//printMatrix((double *)stStates[arrPos].d_cumTable, num_Action, num_eachArrRateRepeats+1);

	//find out which interval does the random value lies in
	for (int i = 0; i < num_Action; i++) {
		if (stStates[arrPos].d_rand[0] < stStates[arrPos].d_cumTable[i][0]) {
			stStates[arrPos].i_sel[0] = i;
			global_selected = i;
			break;
		}
	}
	//cout << "sel activity=" << stStates[arrPos].i_sel[stStates[arrPos].i_loopCount] << endl;



	//update the system parameters according to newly selected activity
	g_servthreadNumForMachine = stAction[stStates[arrPos].i_sel[0]].i_tot_th;   //+1 because the action (4,1,1) position 0, so have to add 1 and likewise
	g_servthreadNum = stAction[stStates[arrPos].i_sel[0]].i_numThPerReq;
	g_machineNum = stAction[stStates[arrPos].i_sel[0]].i_machineNum;
	g_arrivalmean = arrRates[stStates[arrPos].i_state_pos];


	stStates[arrPos].i_loopCount++ ;
	//cout << "g_servthreadNumForMachine" << g_servthreadNumForMachine << endl;
	//cout << "g_servthreadNum" << g_servthreadNum << endl;
	//cout << "g_arrivalmean" << g_arrivalmean << endl;

 	stStates[arrPos].i_sel[0] = global_selected;


}

void find_convergenceTime(long double *temp_matrix,int nrow, int ncol) {
	cout << "nrow=" << nrow << endl;
	cout << "ncol=" << ncol << endl;
	long double conver[numTotalTimeFrame][2] = { 0 }; // stores the 10 converged values
	//int count = 0;
	//int temp_matrix[30] = { 1,4, 7,7,10,10,10,8,10,10,2,5,8,8,10,9,9,8,8,5,3,6,9,9,10,8,7,9,7,6};
	//int *p = temp_matrix;
	//ncol = 10;
	//nrow = 3;



		//find max position of 1st column (0th column for initilization)

		long double max = temp_matrix[0];
		int max_pos = 0;
		for (int j = 1; j < nrow; j++) {
			if (temp_matrix[j*ncol + 0] > max) {
				max = temp_matrix[j*ncol + 0];
				max_pos = j;
			}
		}
		conver[0][0] = max;
		conver[0][1] = max_pos;

		//cout << "max=" << conver[0][0] << "\t position=" << conver[0][1] << endl;
		//cout << "*******" << endl;



		//___________found max value of first column

		for (int i = 1; i < ncol; i++)
		{
			//find max position for each column
			long double max = temp_matrix[i];
			long double max_pos = 0;
			for (int j = 1; j < nrow; j++) {
				if (temp_matrix[j*ncol + i] > max) {
					max = temp_matrix[j*ncol + i];
					max_pos = j;
				}
			}
			//cout << "max==" << max << "\t max_pos=" << max_pos << endl;
			conver[i][0] = max;
			conver[i][1] = max_pos;
		}
		ofstream myfile1;
		myfile1.open("converged_data.txt");
		for (int i = 0; i < numTotalTimeFrame; i++) {
			myfile1 << conver[i][0]<<"\t"<<conver[i][1] << endl;
		}
		//printMatrix((double *)average_latencies_array, 1, numTotalTimeFrame);
		myfile1.close();
		//printMatrix((double *)conver, ncol, 2);
  		int act_convergedLocation = 0;
		//cout << "ncol=" << ncol << endl;
		int i=0;
		long double temp_max = 0;
		for (i = 0; i < ncol-5; i++) {
		//	cout << conver[i][0] << "\t" << conver[i][1] << "\t" << conver[i+1][1] << "\t" << conver[i+2][1] << "\t" << conver[i+3][1] << "\t" << conver[i+3][1] << "\t" << endl;
			float x = 0.5;
			if (conver[i][0] > x && conver[i+1][0] > x && conver[i+2][0] > x && conver[i+3][0] > x && conver[i+4 ][0] > x && conver[i+5][0] > x && conver[i][1] == conver[i + 1][1] && conver[i][1] == conver[i + 2][1] && conver[i][1] == conver[i + 3][1] && conver[i][1] == conver[i + 4][1] && conver[i][1] == conver[i + 5][1]) {
				act_convergedLocation = i;
				temp_max = conver[i][0];
				break;
			}
		}
		cout << "*******Converged Location =*****"<<act_convergedLocation<<endl;
		cout << "***" << endl;
		cout << "Converged value=" << conver[act_convergedLocation][0] << endl;

		long int num_selected = 0;
		for (int j = 0; j < ncol; j++) {
			cout << "conver[j][1]" << conver[j][1] << endl;
		}
		/*
		for (int j = 0; j < ncol; j++) {
			if (conver[j][1] == conver[act_convergedLocation][1]) {
				cout << "conver[j][1]" << conver[j][1] << endl;
				num_selected++;
  				cout << "num_selected=" << num_selected << endl;
			}
			else {
				cout << "******************" << endl;
			}
		}
 		cout << "num_time selected=" << num_selected << endl;
		cout << "***" << endl;*/
}

void work(void) {
	ULONG lRet = TRUE;
	CQueue* pcQueue = new CQueue(MAXQUEUELONGTH);

	do {

		if (NULL == pcQueue)
		{
			lRet = FALSE;
			break;
		}

		lRet = pcQueue->CreateQueue();

		if (FALSE == lRet)
		{
			break;
		}


		lRet = pcQueue->Process();
		if (FALSE == lRet)
		{
			break;
		}

	} while (false);

	if (NULL != pcQueue)
	{
		pcQueue->DestroyQueue();

		delete pcQueue;
		pcQueue = NULL;
	}
}

int main(int argc,char *argv[]) 
{
	ULONG lRet = TRUE;
	memset(&g_stGlobalTime, 0, sizeof(GlobalTime));
	fflag = 0;
	count1_ForMix = 0;
	//generating the basic service time randomly using some distribution
	
	//printMatrix((double *)basic_serviceTime, 1, numTotalTimeFrame*numReqinUnitTimeFrame);
	cout << "*" << endl;
	
	/*
 //testing the distribution
	double exp[100] = { 0 };
	int countt[20] = { 0 };
	
	
	for (int i = 1; i < 5000; i++) {
		exp[i] =Uniform(80,120);
		//exp[i] = Exponential(2000);
		for (int j = 0; j < 20; j++) {
			if (exp[i] < j * 10) {
				countt[j]++;
				break;
			}
		}
	}
	printMatrix_int(countt, 20, 1);
	
	
	//testing for Erlang data
	double exp[100] = { 0 };
	int countt[100] = { 0 };
	for (int i = 1; i < 5000; i++) {
		exp[i] = Erlang(50, 2);
		//exp[i] = Exponential(2000);
		for (int j = 1; j < 100; j++) {
			if (exp[i] < j * 10) {
				countt[j]++;
				break;
			}
		}
	}
	printMatrix_int(countt, 30, 1);
	
	//testing for Normal data
	double exp[100] = { 0 };
	int countt[100] = { 0 };
	for (int i = 1; i < 100; i++) {
		exp[i] = Normal(50, 1);
		for (int j = 1; j < 10; j++) {
			if (exp[i] < j * 10) {
				countt[j]++;
				break;
			}
		}
	}
	printMatrix_int(countt, 30, 1);
	
	//testing for ChiSquare data
	double exp[100] = { 0 };
	int countt[100] = { 0 };
	for (int i = 1; i < 100; i++) {
		exp[i] = Chisquare(50);
		for (int j = 1; j < 10; j++) {
			if (exp[i] < j * 10) {
				countt[j]++;
				break;
			}
		}
	}
	printMatrix_int(countt, 30, 1);
	
	double exp[100] = { 0 };
	int countt[100] = { 0 };
	for (int i = 1; i < 100; i++) {
		exp[i] = Lognormal(5,1);
		cout << "exp[i]" << exp[i] << endl;
		for (int j = 1; j < 10; j++) {
			if (exp[i] < j * 10) {
				countt[j]++;
				break;
			}
		}
	}
	printMatrix_int(countt, 30, 1);
	*/
	//cout << "compCount=" << compCount << endl;
	//cout << "comp Count=" << numTotalTimeFrame*num_eachArrRateRepeats << endl;
		init();		//initialize the status and actions

		for (int i = 0; i < numTotalTimeFrame*numReqinUnitTimeFrame; i++) {
			service[i] = Uniform(80, 100);
		}
		//cout << "_________________________________________________________________" << endl;
		//generate the arrival rates randomly and assign them to an array
		int count = 0;
		for (int i = 0; i < num_ArrStates; i++) {
			for (int j = 0; j < num_eachArrRateRepeats; j++) {
				series_arr[count] = arrRates[i];
				count++; 
			}
		}
		//printMatrix((double *)series_arr, count, 1);

		
		//shuffle the arrival rates
		//random_shuffle(&series_arr[0], &series_arr[count]);

	/*	//cout << "Arival Rates>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
		for (int i = 0; i < numTotalTimeFrame; i++) {
			cout << "Arrival rate no " << i <<"=="<< series_arr[i]<<endl;
		}*/
		

		for (int i = 0; i < numTotalTimeFrame; i++) {
			ran_number[i] = Random();
		//	cout << "ran_number[" << i<<"]=" << ran_number[i] << endl;
		}
		

	

		//
		for (int i = 0; i < numTotalTimeFrame; i++) {
			//find out what is the arrival rate to be worked at in this loop
			//cout << "******************************LOOP*********************************************************************" << i << endl;
			double temp_arr = series_arr[i];
			//cout << "Temp arr" << series_arr[i] << endl;
			int temp_pos = 0;
		//	cout << "Num of arrival states=" << num_ArrStates << endl;
			for (int j = 0; j < num_ArrStates; j++) {
				if (temp_arr == arrRates[j]) {
					temp_pos = j;
			//		cout << "Arr Pos==" << temp_pos << endl;
					break;
				}
			}
			
			//j is the arrRate[j] that has been selected or the position where it is located
			if (stStates[temp_pos].i_loopCount == 0) {
				Rl_init(stStates, temp_pos);
			}
			else
				Rl_run(stStates, temp_pos ,i);


			g_ReqMaxNum = arr_num[stStates[temp_pos].i_state_pos];
	

			work();//return average response time(cost), start time of last request,

			//cout << "In Main,,,Serv=" << lastRequest_servTime << "\t Arr=" << lastRequest_ArrTime << "\t Wait=" << lastRequest_WaitTime << "\t Res==" << lastRequest_ResTime << endl;
			average_latencies_array[i] = lastTime_latency;    //average latency in each time frame
			//update the parameters in the corresponding stStates[] array
			Rl_algo(stStates, temp_pos, lastTime_latency);
		}
		cout << "*****************************************RL Algorithm************************************************" << endl;
		//printMatrix((double *)stStates[0].d_PI, num_Action, num_eachArrRateRepeats+1);
		cout << "*****************************************************************************************************" << endl;
		ofstream myfile;
		myfile.open("RLaverage_latencies.txt");
		for (int i = 0; i < numTotalTimeFrame; i++) {
			myfile << average_latencies_array[i] << endl;
		}
		//printMatrix((double *)average_latencies_array, 1, numTotalTimeFrame);
		myfile.close();

		cout << "*****************************************************************************************************" << endl;
		myfile.open("RLCumEach_latencies.txt");
		for (int i = 0; i < numTotalTimeFrame*numReqinUnitTimeFrame; i++) {
			myfile << CumEach_latency[i] << endl;
		}
		
		//printMatrix((double *)CumEach_latency, 1, numTotalTimeFrame * 10);
		myfile.close();

		cout << "*****************************************************************************************************" << endl;
		myfile.open("RlEach_latencies.txt");
		for (int i = 0; i < numTotalTimeFrame * numReqinUnitTimeFrame; i++) {
			myfile << each_latency[i] << endl;
		}
		//printMatrix((double *)each_latency, 1, numTotalTimeFrame * 10);
		myfile.close();

		cout << "********************************************************************************************************" << endl;
		myfile.open("RL_Selected_Config.txt");
		for (int i = 0; i < numTotalTimeFrame; i++) {
			myfile << stStates[0].i_sel[i] << endl;
		}
		myfile.close();

		cout << "************************************************************************************************************" << endl;
		myfile.open("RL_all_selectedConfig.txt");
		for (int i = 0; i < numTotalTimeFrame; i++) {
			myfile << sel_activity[i] << endl;
		}
		myfile.close();
		//measuring convergence time
		/*	for (int i = 0; i < 10; i++) {
			cout << "*************** " << arrRates[i] << "**** states" << endl;
			find_convergenceTime((long double *)stStates[i].d_PI, num_Action, num_eachArrRateRepeats + 1);
		}*/

		//find_convergenceTime((long double *)stStates[0].d_PI, num_Action, num_eachArrRateRepeats + 1);
		//finished measuring convergence time


		for (int i = 0; i < numTotalTimeFrame; i++) {
			//cout << "&" << endl;
			//cout << "hi" << endl;
			average_latencies_array[i] =  0.0 ;
		}
		for (int i = 0; i < numTotalTimeFrame * numReqinUnitTimeFrame; i++) {
			CumEach_latency[i] = { 0.0 };
			each_latency[i] = { 0.0 };
		}
		fflag = 0;
		cc_count = 1;  //counting the number of reqeusts and recording the values of average latencies, cumEach_latencies
		RL = 0;
		countt_ForMix = 0;
		dresponse = 0;
		dservice = 0;
		dwait = 0;
		

//		printMatrix((double *)mix_arrival,1, numTotalTimeFrame * 10);
 		cout << "*******************SC Started*********************************"<<endl;
		g_servthreadNum = 8;
		
		//printMatrix((double *)service, 1, numTotalTimeFrame*numReqinUnitTimeFrame);

		///////////////////////////////////////////////////////////////////////////////////////////////SC Algorithm
		for (int i = 0; i < numTotalTimeFrame; i++) {
			//Baseline algorithm
			double temp_arr = series_arr[i];
			g_arrivalmean = series_arr[i];
			//cout << "Temp arr" << series_arr[i] << endl;
			int temp_pos = 0;
			//cout << "Num of arrival states=" << num_ArrStates << endl;
			for (int j = 0; j < num_ArrStates; j++) {
				if (temp_arr == arrRates[j]) {
					temp_pos = j;
					//cout << "Arr Pos==" << temp_pos << endl;
					break;
				}
			}
			//if (g_arrivalmean < 70) {
				//for Exponential implementation and Uniform Distribution
				if (temp_arr <= 20) { //r7t1
					g_servthreadNumForMachine = 7;
					g_servthreadNum = 1;
					g_machineNum = 4;
				}
				else if (temp_arr <= 40) {  //r2t4

					g_servthreadNumForMachine = 8;
					g_servthreadNum = 4;
					g_machineNum = 4;

				}
				else if (temp_arr >= 40) {//r1t8
					g_servthreadNumForMachine = 8;
					g_servthreadNum = 8;
					g_machineNum = 4;

				}
				else
				{
					cout << "Unauthorized arrival rate" << endl;
				}

		//	}
		/*
			else if (g_arrivalmean < 200) {
				//for erlang distribution
				if (temp_arr <= 30) { //r8t1
					g_servthreadNumForMachine = 8;
					g_servthreadNum = 1;
					g_machineNum = 4;
				}

				else {  //r1t8

					g_servthreadNumForMachine = 8;
					g_servthreadNum = 8;
					g_machineNum = 4;

				}
			}
			else //for normal   //r1t8
			{
				g_servthreadNumForMachine = 8;
				g_servthreadNum = 8;
				g_machineNum = 4;
			}
			
		/*	if (g_arrivalmean <= 20) {
				g_servthreadNumForMachine = 8;
				g_servthreadNum = 4;
				g_machineNum = 4;
			}
			else {
				g_servthreadNumForMachine = 8;
				g_servthreadNum = 8;
				g_machineNum = 4;
			}*/
			
			
			work();
			average_latencies_array[i] = lastTime_latency;
			cout << "*************************************" << endl;
			cout << "time Frame no:" << i << "is processed" << endl;
			cout << "****************************************" << endl; 
		}
	
	
	
	
	//printMatrix_int((int *)stStates[0].i_sel, num_eachArrRateRepeats+1, 1);
	//cout << "____________________________________________________________" << endl;


//	printMatrix((double *)stStates[0].d_rand, num_eachArrRateRepeats+1, 1);
	//cout << "____________________________________________________________" << endl;

	//printMatrix((double*)stStates[4].d_PI, num_Action, num_eachArrRateRepeats+1);
	//printMatrix((double*))
	
	/*
	for (int j = 0; j < 10; j++) {//arrival states
		cout << "Arrival Rate=" << arrRates[j] << endl;
		for (int i = 0; i < 20; i++) {
			cout << "Config" << i << "==\t" << stStates[j].d_PI[i][num_eachArrRateRepeats] << "\t";
			cout << endl;
		}
	}*/

	
	myfile.open("SCaverage_latencies.txt");
	for (int i = 0; i < numTotalTimeFrame; i++) {
		myfile << average_latencies_array[i] << endl;
	}
	//printMatrix((double *)average_latencies_array, 1, numTotalTimeFrame);
	myfile.close();

	//cout << "*****************************************************************************************************" << endl;
	myfile.open("SCCumEach_latencies.txt");
	for (int i = 0; i < numTotalTimeFrame * numReqinUnitTimeFrame; i++) {
		myfile << CumEach_latency[i] << endl;
	}

	//printMatrix((double *)CumEach_latency, 1, numTotalTimeFrame * 10);
	myfile.close();

	//cout << "*****************************************************************************************************" << endl;
	myfile.open("SCEach_latencies.txt");
	for (int i = 0; i < numTotalTimeFrame * numReqinUnitTimeFrame; i++) {
		myfile << each_latency[i] << endl;
	}
	//printMatrix((double *)each_latency, 1, numTotalTimeFrame * 10);
	myfile.close();
	

	// for baseline display
	//cout << "******************Baseline SC Paper****************************************************************************" << endl;
	//printMatrix((double *)average_latencies_array, 1, numTotalTimeFrame);
	//cout << "*****************************************************************************************************"<<endl;
	//printMatrix((double *)CumEach_latency, 1, numTotalTimeFrame * 10);//numTotalTimeFrame*50
	//cout << "*****************************************************************************************************" << endl;
	//printMatrix((double *)each_latency, 1, numTotalTimeFrame * 10);
	/*
	//find how many times optimal config is selected
	static int total_repeat = 0;
	for (int i = 0; i < numTotalTimeFrame; i++) {
		
		if (stStates[0].i_sel[i] == 7) {
			total_repeat++;
		}
		
	}
	cout << "total repeat" << total_repeat << endl;
	cout << "numTotaltime farme" << numTotalTimeFrame << endl;
	double pert;
	pert=	(float)total_repeat / (float)numTotalTimeFrame;
	cout << "Percentage of selected" << pert << endl;
	
	*/

	cin.get();
	return (int)lRet;
}