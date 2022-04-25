//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  ASD subject 3º GTI
//  - Understanding basic concepts for OpenMP
//-----------------------------------------------------

using namespace std;

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <omp.h>

void part_a_b_c (int n);
void part_d (unsigned n);

int main()
{
	// Uncoment the function to work with
	//-----------------------------------
	//part_a_b_c(8);
	part_d(1000000);
	//-----------------------------------
}

void part_a_b_c (int n)
{
	//#pragma omp parallel
	{
		int this_thread = omp_get_thread_num();
		for (int i=0;i<n;i++)
		{
			cout << "Iteration " << i << " is handled by thread " << this_thread << endl;
		}
	}
}

void part_d (unsigned n)
{
	unsigned suma=0;

	for (int i=0; i<n; i++)
	{
		suma = suma + i;
	}
	cout << "Result: " << suma << ". Check: " << ((n-1)*n)/2 << endl;
}

