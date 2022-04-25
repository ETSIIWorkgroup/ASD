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

void part_a_b_c_1 (int n);
void part_a_b_c_2 (int n);
void part_d (unsigned n);

int main()
{
	// Uncoment the function to work with
	//-----------------------------------
	//part_a_b_c_1(16);
	//part_a_b_c_2(16);
	part_d(1000000);
	//-----------------------------------
}

void part_a_b_c_1 (int n)
{
	#pragma omp parallel
	{
		// Nº de hilos:
		int T = omp_get_num_threads();
		// Hilo que está ejecutando:
		int t = omp_get_thread_num();
		// Contenido del for:
		int trozo = n / T;
		int inicio = trozo * t;
		int fin = trozo * (t + 1);

		#pragma omp critical
		{
			for (int i = inicio; i < fin; i++)
			{
				cout << "Iteration " << i << " is handled by thread " << t << endl;
			}
		}

	}
}

void part_a_b_c_2 (int n)
{
	#pragma omp parallel for
	
		for (int i = 0; i < n; i++)
		{
			cout << "Iteration " << i << " is handled by thread " << omp_get_thread_num() << endl;
		}
	
	
}

void part_d (unsigned n)
{
	unsigned suma=0;
	#pragma omp parallel

	{
		#pragma omp critical
		{

			// Nº de hilos:
			int T = omp_get_num_threads();
			// Hilo que está ejecutando:
			int t = omp_get_thread_num();
			// Contenido del for:
			int trozo = n / T;
			int inicio = trozo * t;
			int fin = trozo * (t + 1);

			for (int i = inicio; i < fin; i++)
			{
				suma = suma + i;
			}
			
		}

	}

	cout << "Result: " << suma << ". Check: " << ((n - 1) * n) / 2 << endl;

}

