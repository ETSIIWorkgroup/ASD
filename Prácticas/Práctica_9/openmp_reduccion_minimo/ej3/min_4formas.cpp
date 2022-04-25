//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  ASD subject 3º GTI
//  - computing mimimum with 4 manners
//-----------------------------------------------------

using namespace std;

#include <iostream>
#include <iomanip>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>
#include "querypmrdtsc.h"

#define NUMREP 64
#define MINSIZE  (128)
#define MAXSIZE  (16*1024*1024)

__declspec(align(16)) float x[MAXSIZE], y[MAXSIZE];

double times[4][18];
int indxtime = 0;

void init_vectors (int);
float  min_seq(int n, float *vec);
float  min_a(int n, float *vec);
float  min_b(int n, float *vec);
float  min_c(int n, float *vec);
float  min_d(int n, float *vec);

int main()
{
	int repe, n;
	QPTimer c1;
	float 	var_min_seq, var_min_par;
	FILE *fres;

	// pointing the vector to be tested to a pointer.
	float *tested_vector = y;

	setlocale(LC_NUMERIC, "es");

	c1.Calibrate();
	for (n = MINSIZE; n <= MAXSIZE; n = n * 2)
	{
		init_vectors(n);
		for (repe = 0; repe < NUMREP; repe++)
		{
			c1.Start();
			// Sequential execution is correct 
			var_min_seq = min_seq(n, tested_vector );
			c1.Stop();
			c1.Reset();
		}
		cout << "Size: " << setw(9) << n
			<< ". SEQ  Minimum cycles per array element: " << setiosflags(ios::fixed) << setw(8) << setprecision(4) << c1.GetMinimumCyclesByIteration(n)
			<< endl;
		times[0][indxtime] = c1.GetMinimumCyclesByIteration(n);
		c1.ResetAll();
		//cout << "SEQ: Minimum is: " << var_min_seq << endl;

		//init_vectors(n);
		////////////////////////////////////////////////////////////////////
		for (repe = 0; repe < NUMREP; repe++)
		{
			c1.Start();
			// Uncomment the call to the PARALLEL function under test.
			//------------------------------------------------
			// var_min_par = min_a(n, tested_vector );
			 var_min_par = min_b(n, tested_vector );
			// var_min_par = min_c(n, tested_vector );
			// var_min_par = min_d(n, tested_vector );
			//------------------------------------------------
			c1.Stop();
			c1.Reset();
		}
		cout << "Size: " << setw(9) << n
			<< ". PARB Minimum cycles per array element: " << setiosflags(ios::fixed) << setw(8) << setprecision(4) << c1.GetMinimumCyclesByIteration(n)
			<< endl;
		times[1][indxtime] = c1.GetMinimumCyclesByIteration(n);
		c1.ResetAll();
		//cout << "PAR: Minimum is: " << setprecision(8) << var_min_seq << endl;
		if (var_min_seq != var_min_par)
			cout << "*** PARB Minimum is BAD " << endl;
		////////////////////////////////////////////////////////////////////
		for (repe = 0; repe < NUMREP; repe++)
		{
			c1.Start();
			// Uncomment the call to the PARALLEL function under test.
			//------------------------------------------------
			// var_min_par = min_a(n, tested_vector );
			// var_min_par = min_b(n, tested_vector );
			var_min_par = min_c(n, tested_vector );
			// var_min_par = min_d(n, tested_vector);
			//------------------------------------------------
			c1.Stop();
			c1.Reset();
		}
		cout << "Size: " << setw(9) << n
			<< ". PARC Minimum cycles per array element: " << setiosflags(ios::fixed) << setw(8) << setprecision(4) << c1.GetMinimumCyclesByIteration(n)
			<< endl;
		times[2][indxtime] = c1.GetMinimumCyclesByIteration(n);
		c1.ResetAll();
		//cout << "PAR: Minimum is: " << setprecision(8) << var_min_seq << endl;
		if (var_min_seq != var_min_par)
			cout << "*** PARC Minimum is BAD " << endl;
		////////////////////////////////////////////////////////////////////
		for (repe = 0; repe < NUMREP; repe++)
		{
			c1.Start();
			// Uncomment the call to the PARALLEL function under test.
			//------------------------------------------------
			// var_min_par = min_a(n, tested_vector );
			// var_min_par = min_b(n, tested_vector );
			// var_min_par = min_c(n, tested_vector );
			var_min_par = min_d(n, tested_vector);
			//------------------------------------------------
			c1.Stop();
			c1.Reset();
		}
		cout << "Size: " << setw(9) << n
			<< ". PARD Minimum cycles per array element: " << setiosflags(ios::fixed) << setw(8) << setprecision(4) << c1.GetMinimumCyclesByIteration(n)
			<< endl;
		times[3][indxtime] = c1.GetMinimumCyclesByIteration(n);
		c1.ResetAll();
		//cout << "PAR: Minimum is: " << setprecision(8) << var_min_seq << endl;
		if (var_min_seq != var_min_par)
			cout << "*** PARD Minimum is BAD " << endl;

		indxtime++;
	}

	fres = fopen("results.csv", "w");
	if (!fres)
	{
		cout << "Please, close results.csv file if you have it opened in another program, such as Excel" << endl;
		cout << "Results file NOT created." << endl;
		exit(0);
	}

	for (int j = 1; j <= 3; j++)
	{
		for (n = 0; n < 18; n++)
		{
			fprintf(fres, "%.8lf", times[0][n]);
			if (n != 17)
				fprintf(fres, ";");
			else
				fprintf(fres, "\n");
		}
		for (n = 0; n < 18; n++)
		{
			fprintf(fres, "%.8lf", times[j][n]);
			if (n != 17)
				fprintf(fres, ";");
			else
				fprintf(fres, "\n");
		}
		fprintf(fres, "\n");
	}
	fclose(fres);
	cout << "Results file created. Import it into Excel" << endl;
}

/////////////////////////////////////////////////////////////////
void init_vectors (int n)
{
    int i;
    srand(1);
	for (i = 0; i < n; i++)
	{
		y[i] = n - i;
	}
	int cont = 0;
	for (i = 0; i < n / 16; i++)
	{
		//x[cont] = (n / 16) - i + j; // (float)rand();
		x[cont] = (float)rand() ; // ;
		cont++;
	}
	for (int j = 1; j < 16; j++)
		for (i = 0; i < n / 16; i++)
		{
			//x[cont] = (n / 16) - i + j; // (float)rand();
			x[cont] = x[i] - (float)rand() / ((float)RAND_MAX) ; // ;
			cont++;
		}
}

float min_seq (int n, float *vec)
{
	float min = vec[0];
	for (int i = 0; i < n; i++)
	{
		if (vec[i] < min) min = vec[i];
	}
	return min;
}

float min_a (int n, float *vec)
{
	float min = vec[0];
	
    #pragma omp parallel for default(none) shared(vec,n,min)
	for (int i = 0; i < n; i++)
	{
		if (vec[i] < min) 
			min = vec[i]; // Ouch!!!
	}
	return min;
}

float min_b (int n, float *vec)
{
	float min_total,  min[16];
	int nth;
	for (int i = 0; i < 16; i++) 
		min[i] = vec[0];

    #pragma omp parallel default(none) shared(nth,vec,n,min)
	{
		int tid = omp_get_thread_num();
		nth = omp_get_num_threads();
		for (int i = tid*(n/nth); i < (tid+1)*(n / nth); i++)
		{
			if (vec[i] < min[tid])
				min[tid] = vec[i];
		}
	}

	min_total = min[0];
	for (int i = 1; i < nth; i++) {
		if (min_total > min[i]) 
			min_total = min[i];
	}
	return min_total;
}

float min_c (int n, float *vec)
{
	float min_total, min[16];
	int nth;

	for (int i = 0; i < 16; i++) 
		min[i] = vec[0];

	// Esto puede mejorarse.... ahora mismo es igual que min_b
    #pragma omp parallel default(none) shared(nth,vec,n,min)
	{
		int tid = omp_get_thread_num();
		nth = omp_get_num_threads();
		for (int i = tid * (n / nth); i < (tid + 1)*(n / nth); i++)
		{
			if (vec[i] < min[tid])
				min[tid] = vec[i];
		}
	}

	min_total = min[0];
	for (int i = 1; i < nth; i++) {
		if (min_total > min[i])
			min_total = min[i];
	}
	return min_total;
}

float min_d(int n, float *vec)
{
	float min = vec[0];

	// Esto puede mejorarse bastante....
    #pragma omp parallel for default(none) shared(vec,n,min)
	for (int i = 0; i < n; i++) 
	{
        #pragma omp critical
		{
			if (vec[i] < min)
				min = vec[i];
		}
	}
	return min;
}
