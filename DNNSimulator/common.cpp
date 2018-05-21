#include "common.h"
#include "CQueue.h"

BOOL Cmp(double a, double c)
{
	if (a < c)
		return TRUE;
	else
		return FALSE;
}

double Min(double a, double c)
/* ------------------------------
 * return the smaller of a, b
 * ------------------------------
 */
{
  if (a < c)
    return (a);
  else
    return (c);
}

double Max(double a, double c)
{
  if (a < c)
    return (c);
  else
    return (a);
}

//   double Exponential(double m)
///* ---------------------------------------------------
// * generate an Exponential random variate, use m > 0.0
// * ---------------------------------------------------
// */
//{
//  return (-m * log(1.0 - Random()));
//}


//   double Uniform(double a, double b)
///* --------------------------------------------
// * generate a Uniform random variate, use a < b
// * --------------------------------------------
// */
//{
//  return (a + (b - a) * Random());
//}



  double GetArrival(int g_ulReqWholeNum)
/* ---------------------------------------------
 * generate the next arrival time, with rate 1/2
 * ---------------------------------------------
 */
{
  static double arrival = START;
		
	  static double loop = 1;   //to make sure that when RL finishes SC also starts from time=0
	//  double x = 0;				//to print out the values of arrival times
	  static int Local_countForMixArrival_RL = 0;
	  static int Local_countForMixArrival_SC = 0;
  SelectStream(1);//(0)
  /*
  if (RL == 1) 
  {
	  if (g_arrivalmean < 70) {
		  x = Normal(g_arrivalmean, 1);

	  }
	  else if (g_arrivalmean < 200) {
		  x = Exponential(g_arrivalmean);

	  }
	  else {
		  x = Erlang(g_arrivalmean,2);
	  }
	//  cout << "count1_ForMix=" << count1_ForMix << endl;
	  mix_arrival[Local_countForMixArrival_RL] = x;
	  Local_countForMixArrival_RL++;
	  //cout << "mix+arrival=" << mix_arrival[count_ForMix] << "\t count=" << count_ForMix << endl;
	  
  }
 else {
	 x = mix_arrival[Local_countForMixArrival_SC];
	 Local_countForMixArrival_SC++;
	 
	  
	  //cout << "x=" << x <<"\t count="<<count_ForMix<< endl;
  
  }
  */
 // count1_ForMix++;
  
 //double x = Chisquare(g_arrivalmean);
 //double x = Normal(g_arrivalmean, 1);
  //double x = Erlang(g_arrivalmean,2);
 // arrival += Erlang(g_arrivalmean, 1);
   //arrival += Exponential(g_arrivalmean);
  //double x = Uniform(g_arrivalmean-20, g_arrivalmean+20);
  {
	  static int Run_count = 0;
	  static double inter_arrival[numTotalTimeFrame*numReqinUnitTimeFrame] = { 0.0 };
	  
	  //to make sure RL and SC has same arrival rates for corresponding requests
	  if (RL == 1 && Run_count == 0) {
		  for (int i = 0; i < numTotalTimeFrame*numReqinUnitTimeFrame; i++) {
			  inter_arrival[i] = Uniform(g_arrivalmean - 40, g_arrivalmean + 40);
			 // inter_arrival[i] = Exponential(g_arrivalmean);
		  }
		  arrival = START;
		  
		//  cout << "Inter-arrival***********" << endl;
		 // printMatrix((double *)inter_arrival, 1, numTotalTimeFrame*numReqinUnitTimeFrame);
	  }
	  if (RL == 0 && Run_count - numTotalTimeFrame*numReqinUnitTimeFrame == 0) {
		  arrival = START;
	  }
	  double x = 0;
	  if (RL == 1)
		  x = inter_arrival[Run_count];
	  else
		  x = inter_arrival[Run_count - numTotalTimeFrame*numReqinUnitTimeFrame];

	  arrival += x;
	  Run_count++;
	  cout << "arrival time of " << g_ulReqWholeNum << "==" << arrival << "inter-arrival==" << x << endl;

  }
  /*
  double x = Uniform(g_arrivalmean - 20, g_arrivalmean + 20);
  arrival += x;
  */
  //arrival+=25;
  //arrival += Exponential(0.00176);//0.12280,0.00176
  if(g_ulReqWholeNum==0 && loop==1)
	last_finishTime=arrival;
  loop++;
  return (arrival);
}


  double GetService()
/* --------------------------------------------
 * generate the next service time with rate 2/3
 * --------------------------------------------
 */
{
  static double service = 0;
  SelectStream(0);
  service = Exponential(MEANSERTIME);
  //printf("service %f\n",temp);
   //system("pause");
  return (service);

}

  double GetAverage(double number, double numcount, double mean)
{ 
  mean= mean + (number - mean)/numcount;
  return (mean);
}

  
  void init(void)
  {
	  //_____________ added by biplav_______//


	  g_servthreadNum = 1;
	  g_servthreadNumForMachine = 1;
	  g_ReqMaxNum = 10;
	  g_arrivalmean = 25;
	  g_machineNum = 4;

	 
	  int i_ActionValues[][12] = { {2,4,6,8,3,6,4,8,5,6,7,8},{1,2,3,4,1,2,1,2,1,1,1,1} };


	  //initializing the parameters in the Action(configuration)
	  
	/*  stAction[0].i_machineNum = 4;
	  stAction[0].i_numThPerReq = 1;		//=g_servthreadNum
	  stAction[0].i_tot_th = 8;*/

	  for (int i = 0; i < num_Action; i++) {
		  stAction[i].i_machineNum = 4;
		  stAction[i].i_numThPerReq = i+1;		//=g_servthreadNum
		  stAction[i].i_tot_th = i+1;		//=g_servthreadNumForMachine
		 
	  }
	  for (int i = 8; i < num_Action; i++) {
		     stAction[i].i_machineNum = 4;
			  stAction[i].i_numThPerReq = i_ActionValues[1][i-8];
			  stAction[i].i_tot_th = i_ActionValues[0][i-8];
		 
	  }
	  for (int i = 0; i < num_Action; i++) {
		  cout << "Config" << i << "\t NumThPerReq=" << stAction[i].i_numThPerReq << "\t Total Thread=" << stAction[i].i_tot_th << endl;
	  }
	  
	  //-------------finished assigning the parameters in the Action
/*
	  for (int i = 0; i < 8; i++) {
		  cout << stAction[i].i_machineNum << "\t";
		  cout << stAction[i].i_numThPerReq << "\t";
		  cout << stAction[i].i_tot_th << endl;
	  }  */
	  //initializing state(arrival rate) parameters
	  for (int i = 0; i < num_ArrStates; i++) {
		  for (int j = 0; j < num_eachArrRateRepeats; j++) {//num_eachArrRateRepeats
			  stStates[i].i_sel[j] = 10;
			  stStates[i].d_rand[j] = 10;
			  stStates[i].i_stateId = arrRates[i];
			  stStates[i].i_state_pos = i;
			  stStates[i].i_loopCount = 0;
		  }
		  for (int j = 0; j <num_Action; j++) {
			  for (int k = 0; k < num_eachArrRateRepeats; k++) {//num_eachArrRateRepeats
				  stStates[i].d_PI[j][k] = 0.0;
				  stStates[i].d_Vs[j][k] = 0.0;
				  stStates[i].d_cumTable[j][k] = 0.0;
			  }
		  }
	  }
	 // printMatrix((double *)stStates[0].d_PI, num_Action, num_eachArrRateRepeats);
	  //stStates[0].d_PI[0][0] = 8;
	 long double init_val = 1 / num_Action;
	  //make the initial values of d_PI and d_VS to 1/8 for all the arrival states
	  for (int i = 0; i < num_ArrStates; i++) {
		  for (int j = 0; j < num_Action; j++) {
			  //cout << "i=" << i << "j=" << j << endl;
			  stStates[i].d_PI[j][0] = 0.05;
			  stStates[i].d_Vs[j][0] = 0.05;
	//		  	cout << "stStates["<<i<<"].d_PI["<<j<<"][0]=" << stStates[i].d_PI[j][0] << endl;
		//	  cout << "stStates["<<i<<"].d_Vs["<<j<<"][0]=" << stStates[i].d_Vs[j][0] << endl;
		  }
	  }



	  //stStates[1].d_PI[1][0] = 1;
	  //	cout << "stStates[1].d_PI[2][0] " << stStates[1].d_PI[2][0] << endl;



	  //checking correctness upto current point	
	 // printMatrix((double *)stStates[0].d_PI, num_Action, num_eachArrRateRepeats+1);
	  //printMatrix((double *)stStates[0].d_Vs, num_Action, num_eachArrRateRepeats+1);
	/*
	//________upto _____________//
	  for (int i = 0; i < num_ArrStates; i++) {
	  for (int j = 0; j < num_eachArrRateRepeats; j++) {
	  cout << stStates[i].i_sel[j]<<"\t";
	  cout << stStates[i].d_rand[j]<<"\t";
	  cout<<stStates[i].i_stateId<<"\t";
	  cout << stStates[i].i_state_pos << "\t";
	  cout << stStates[i].i_loopCount << endl;
	  }
	  }
	 */
	// cin.get(); 
  }

  void printMatrix(double *matrix, int r, int c)
  {
	  for (int i = 0; i<r; i++)
	  {
		  for (int j = 0; j<c; j++)
		  {
			  cout << matrix[i*c + j] << " ";
		  }
		  cout << endl;
	  }
  }
  void printMatrix_int(int *matrix, int r, int c)
  {
	  for (int i = 0; i<r; i++)
	  {
		  for (int j = 0; j<c; j++)
		  {
			  cout << matrix[i*c + j] << " ";
		  }
		  cout << endl;
	  }
  }


  void Rl_algo(stState *stStates, int arrPos, long double cost)
  {
		  static int req_count = 0;
		  static double current_arrival = arrRates[arrPos];  //to know the state in which we are in
		 
		//  cout << "Arrival Rate===================" << arrRates[arrPos] << endl;
		  {
			  int temploopCount = stStates[arrPos].i_loopCount;
			  int tempsel = stStates[arrPos].i_sel[temploopCount-1];
			  long double tempPI[num_Action] = { 0.0 };
			  long double tempVs[num_Action] = { 0.0 };
			  int stateId = stStates[arrPos].i_stateId;
			  int statePos = arrPos;
			  long double temp_cumTable[num_Action] = { 0.0 };
			  long double denom = 0;
			  double tempRand = 0;

			 // cout << "Latency=" << cost << endl;
			  //cout << "*********Parameters**************" << endl;
			  //cout << "Temploopcount" << temploopCount << "\t tempsel=" << tempsel << "\t stateid=" << stateId << "\t tempRand=" << tempRand << endl;

			  //calculate the Vs function of just executed time frame
			  //copy all the elements of previous time period to this time
			  for (int i = 0; i < num_Action; i++) {
				  tempVs[i] = stStates[arrPos].d_Vs[i][temploopCount-1];
			  }
			//  printMatrix((double *)tempVs, num_Action, 1);
			 // cout << "TempVs above &&&&&&&&&" << endl;
			  //update the value of Vs for the 'one' selected configuration
			 // cout << "Global Selected=" << global_selected << endl;
			  tempVs[tempsel] = (1 - beta)*tempVs[tempsel] + beta* cost;   //cost is taken just for arr 25, randomly
			//  printMatrix((double *)tempVs, num_Action, 1);
			 // cout << "tempVe[tempsel]=" << tempVs[tempsel] << endl;
			  //now we have the final Vs table ready


			  //calculate PI values
			  //calculate the denominator of PI expression
			  for (int i = 0; i < num_Action; i++) {

				  denom += exp(-beta*tempVs[i]);
			  }
			//  cout << "Denom=" << denom << endl;
			  for (int i = 0; i < num_Action; i++) {
				  tempPI[i] = exp(-beta*tempVs[i]);
			  }
			//  cout << "Temp PI below" << endl;
			 // printMatrix((double *)tempPI, 8, 1);
			  for (int i = 0; i < num_Action; i++) {
				  tempPI[i] = tempPI[i] / denom;
			  }
			//  cout << "Temp PI below $$$$$$$$$$$$$$$$$$$$$$$" << endl;
			// printMatrix((double *)tempPI, num_Action, 1);

			  //form the cummulative table of PI
			  temp_cumTable[0] = tempPI[0];
			  for (int i = 1; i < num_Action; i++) {
				  temp_cumTable[i] = temp_cumTable[i - 1] + tempPI[i];
			  }
			//  cout << "__________________tem Cum Table below" << endl;
			// printMatrix((double *)temp_cumTable, 8, 1);



			  //copy temporary values in the actual stStates structure
			  //copy Values of Vs and PI 
			  for (int i = 0; i < num_Action; i++) {
				  stStates[arrPos].d_Vs[i][temploopCount] = tempVs[i];
				  stStates[arrPos].d_PI[i][temploopCount] = tempPI[i];
				  stStates[arrPos].d_cumTable[i][temploopCount] = temp_cumTable[i];
			  }
			  //cout << "____________________________________________________________" << endl;
			//  printMatrix((double *)stStates[arrPos].d_Vs, num_Action, num_eachArrRateRepeats+1);
			  cout << "____________________________________________________________" << endl;
			  //printMatrix((double *)stStates[arrPos].d_PI, num_Action, num_eachArrRateRepeats+1);
			  cout << "____________________________________________________________" << endl;
			

			

		  }

			
		  req_count++;
		  cout << "*************Time Frame No" << req_count << "is processed" << endl;
		
  }