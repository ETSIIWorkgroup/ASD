//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  ASD subject 3º GTI
//  - Understanding basic concepts for OpenMP
//-----------------------------------------------------

using namespace std;

#include <iostream>
#include <iomanip>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "querypmrdtsc.h"

#define NUMREP 56
#define MINSIZE 64
#define MAXSIZE (8*1024*1024)

__declspec(align(16)) float x[MAXSIZE], y[MAXSIZE], y2[MAXSIZE], y3[MAXSIZE];
float a = 3.141592654;
__declspec(align(16)) unsigned char z[MAXSIZE];

double times[18];
int indxtime = 0;

void init_vectors (int);
void saxpy (int n);
float dotproduct (int n);
int countzeros (int n);

int main()
{
	int repe, n;
	QPTimer c1;
	float res = 0;
	unsigned nz = 0;
	FILE *fres;

	setlocale(LC_NUMERIC, "es");

	c1.Calibrate();
	for (n = MINSIZE; n <= MAXSIZE; n = n * 2)
	{
		init_vectors(n);
	
		for (repe = 0; repe < NUMREP; repe++)
		{
			c1.Start();

			// Uncomment the call to the function under test.
			//------------------------------------------------
			saxpy(n);
			//res = dotproduct(n);
			//nz = countzeros(n);
			//------------------------------------------------

			c1.Stop();
			c1.Reset();
		}
		cout << "Size: " << setw(9) << n
			<< ". Minimum cycles per array element: " << setiosflags(ios::fixed) << setw(8) << setprecision(4) << c1.GetMinimumCyclesByIteration(n)
			<< endl;
		times[indxtime++] = c1.GetMinimumCyclesByIteration(n);
		c1.ResetAll();
	}
	cout << endl << "Ignore this... " << nz << " " << res << endl;

	fres = fopen("results.csv", "w");
	if (!fres)
	{
		cout << "Please, close results.csv file if you have it opened in another program, such as Excel" << endl;
		cout << "Results file NOT created." << endl;
		exit(0);
	}

	for (n = 0; n < 18; n++)
	{
		fprintf(fres, "%.8lf", times[n]);
		if (n != 17)
			fprintf(fres, ";");
		else
			fprintf(fres, "\n");
	}
	fclose(fres);
	cout << "Results file created. Import it into Excel" << endl;
}


void init_vectors (int n)
{
    int i;

    srand(1);
    for (i=0;i<n;i++)
    {
        x[i] = (float)i/n;
        y[i] = 1-x[i];
        z[i] = (rand()>(RAND_MAX/2))? 1 : 0;
    }
}


// Computes: Y = a*X + Y
void saxpy (int n)
{

}


// Computes and return the value X*Y (dot product)
float dotproduct (int n)
{
	float sum_dotp=0.0;




	return sum_dotp; // just a filler
}

// Counts and returns the number of zero-elements in Z
int countzeros (int n)
{
	int nz = 0;
	
	
	
	return nz; // just a filler
}
