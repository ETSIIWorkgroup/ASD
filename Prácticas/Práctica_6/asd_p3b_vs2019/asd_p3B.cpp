//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  ASD subject 3º GTI
//  version 1. Nov. 2012. Fernando Diaz del Rio
//  version 2. Mar. 2014. Jesus Rodriguez Leal
//  
//
//  - Doing several test to reach ILP limits in superscalar processor 
//  - TEST TIMING OF simple routine
//-----------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <iostream>

using namespace std;
//------------------------------------------------------------------
// project def and includes

// this test uses a new version of QueryPerformanceTiming , to measure CPU cycles more precisely using rdtsc asm instr.
#include "QueryPerformanceTiming_rdtsc.h"
// CHANGE THE CPU FREQUENCY  IN THIS LINE OF THIS FILE .h: // #define CPU_FREQ_HZ (2.660e+9)

#define  N_REPETIC 64
// repeat several times each test to extract the minimum time
// and to have the caches filled

#define  MAX_NUM_ELEM (16*1024*1024)

double a[MAX_NUM_ELEM];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vectors  init
void vectors_init ()
{
	int i;
	for (i=0;i<MAX_NUM_ELEM;i++)
	{
		// a[i] must have near to 1 values to avoid overflow/underflow in tests
		//a[i] = 1.0f;
		a[i] = 1.0 + (((double)rand() - RAND_MAX / 2) / (double)(1.0e+7));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// tests
double test1 (int niter)
{
	int i;
	double acc=1.0;
	for (i=0;i<niter;i++)
	{
		acc=acc*a[i];
	}
	return acc;
}

double test2 (int niter)
{
	int i;
	double acc = 1.0;
	double acc1=1.0;
	double acc2=1.0; // initialization of accumulator variables
	for (i=0;i<niter%2;i++) //DO NOT TOUCH THIS LOOP: this is a prolog code to check if niter is not multiple of 2
	{
		acc1=acc1*a[i];  //DO NOT TOUCH THIS LOOP: 
	}

	// Write here the same algorithm using two accumulators
	// for ( ; i<niter ; i+=2) //this must be the central loop
	

	return ;  //RETURN HERE THE TOTAL ACCUMULATOR
}

double test3 (int niter)
{
	int i;
	double acc = 1.0;
	double acc1=1.0;
	double acc2=1.0;
	double acc3=1.0;
	for (i=0;i<niter%3;i++) //DO NOT TOUCH THIS LOOP: this is a prolog code to check if niter is not multiple of 3
	{
		acc1=acc1*a[i]; //DO NOT TOUCH THIS LOOP: 
	}

	// Write here the same algorithm using 3 accumulators
	// for ( ; i<niter ; i+=3) //this must be the central loop
	for (; i < niter; i += 3)
	{
		
	}



	return ;  //RETURN HERE THE TOTAL ACCUMULATOR
}

double test4 (int niter)
{
	int i;
	double acc = 1.0;
	double acc1=1.0;
	double acc2=1.0;
	double acc3=1.0;
	double acc4=1.0;
	for (i=0;i<niter%4;i++) //DO NOT TOUCH THIS LOOP: this is a prolog code to check if niter is not multiple of 4
	{
		acc1=acc1*a[i];  //DO NOT TOUCH THIS LOOP: 
	}

	// Write here the same algorithm using 4 accumulators
	// for ( ; i<niter ; i+=4) //this must be the central loop
	for (; i < niter; i += 4)
	{
		
	}




		return ;  //RETURN HERE THE TOTAL ACCUMULATOR
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv ) {

	double var1,var2,var3,var4;
	int i,div,niter;

	SetProcessAffinityMask(GetCurrentProcess(), 1);

	vectors_init();

	for (div=MAX_NUM_ELEM/512;div>=1;div/=2)
	{
		niter = MAX_NUM_ELEM / div;

		QPTimer c1,c2,c3,c4;
		c1.Calibrate(); //calibrates timer overhead and set cronometer to zero	
		c2.Calibrate(); //calibrates timer overhead and set cronometer to zero	
		c3.Calibrate(); //calibrates timer overhead and set cronometer to zero	
		c4.Calibrate(); //calibrates timer overhead and set cronometer to zero	

		// measuring tests
		for (i=0;i<N_REPETIC;i++)
		{
			c1.Start();  // start timer
			var1=test1(niter); // Do the test
			c1.Stop();  // stop timer
			c1.Reset();

			c2.Start();  // start timer
			var2=test2(niter); // Do the test
			c2.Stop();  // stop timer
			c2.Reset();

			c3.Start();  // start timer
			var3=test3(niter); // Do the test
			c3.Stop();  // stop timer
			c3.Reset();

			c4.Start();  // start timer
			var4=test4(niter); // Do the test
			c4.Stop();  // stop timer
			c4.Reset();
		} 
		// end of    testing

		cout << endl << "ONLY PRINTING OUTPUT VARIABLE TO PREVENT THAT THE COMPILER ELIMINATES FUNCTION CALLS: " 
			<< var1 << ", " << var2 << ", " << var3 << ", " << var4 << endl;

		cout << endl << "-Number of iterations: " << niter << endl;
		cout << endl << "-Number of measures: " << c1.NumberOfMeasures() << endl;
		
		c1.PrintMinimumCyclesByIteration  (" Minimum time in cycles for a vector element of test1()   is:   ", niter);
		c2.PrintMinimumCyclesByIteration  (" Minimum time in cycles for a vector element of test2()   is:   ", niter);
		c3.PrintMinimumCyclesByIteration  (" Minimum time in cycles for a vector element of test3()   is:   ", niter);
		c4.PrintMinimumCyclesByIteration  (" Minimum time in cycles for a vector element of test4()   is:   ", niter);

		if (abs(var1 - var2) / var1 > 1.0e-3){
			cout << "!!!  ERROR IN test2() CODE . TIMING RESULTS NOT VALID FOR test2()." << endl;
			cout << "!!!  ERROR IN test2() CODE . TIMING RESULTS NOT VALID FOR test2()." << endl;
		}
		if (abs(var1 - var3) / var1 > 1.0e-3){
			cout << "!!!  ERROR IN test3() CODE . TIMING RESULTS NOT VALID FOR test3()." << endl;
			cout << "!!!  ERROR IN test3() CODE . TIMING RESULTS NOT VALID FOR test3()." << endl;
		}
		if (abs(var1 - var4) / var1 > 1.0e-3){
			cout << "!!!  ERROR IN test4() CODE . TIMING RESULTS NOT VALID FOR test4()." << endl;
			cout << "!!!  ERROR IN test4() CODE . TIMING RESULTS NOT VALID FOR test4()." << endl;
		}
		cout << endl;
	}
}
