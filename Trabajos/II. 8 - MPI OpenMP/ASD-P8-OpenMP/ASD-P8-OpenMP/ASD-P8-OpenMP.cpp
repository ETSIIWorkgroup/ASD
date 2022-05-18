//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  version 1. 2021. Fernando Diaz del Rio 
//  
//  - parallelizing scientific programs with OpenMP/MPI
//  - Parallelizing the generated histogram by a pseudorandom number generator
//
//-----------------------------------------------------

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//	Práctica 8 - Arquitectura de Sistemas Distribuidos - Universidad de Sevilla Curso 2021/2022
// 
//	Alumnos:
//	- Pedro Escobar Rubio
//	- Alejandro Fernández Trigo
// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

using namespace std;

#include <iostream>
#include <iomanip>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//------------------------------------------------------------------
// project def and includes

// this test uses measure CPU cycles more precisely using rdtsc asm instr. (see previous practices)
//#include "querypmrdtsc.h"

#define  MAX_N_THREADS_OR_TASKS 4

#define  N_REPETITIONS 13
// repeat several times each test to extract the minimum time
// and also to check if different executions return the same values

// RAND_MAX		
// IF THE STUDENT used another pseudorandom generator, please check that the constant RAND_MAX is defined and which value it has.
#define  MAX_N_SLICES ((RAND_MAX+1)/32) // max number of slices in which the original random numbers are divided 
#define  MIN_N_SLICES ((RAND_MAX+1)/512) // min number of slices in which the original random numbers are divided 

#define  N_ITERATIONS  (1000*MAX_N_SLICES) //NUMBER of tested random numbers //big number to execute a considerable time . 
// IT MUST BE  a mulitple of MAX_N_SLICES (so that if the random generation were perfect, the histogram would be uniform)

#define  N_TESTS 4
// repeat several times the test with different seeds for the random generation 

typedef double  ElementType;

int hist[N_TESTS][MAX_N_SLICES]; //it must contain the number of times that a certain slice is generated (here slice is the reduced number from the original number returned by rand())
// the first indexes is to save different tests, each with a different seed 

int prime_numbers[29] = { 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887 , 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997 };
int seeds[N_TESTS]; //A seed for each thread or process (task) is preferable 

ElementType seq_std_deviation[N_TESTS];
ElementType par_std_deviation[N_TESTS];

//protoype functions to initialize matrixes and do tests

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// matrixes and vectors  init
void matrix_vector_init(int current_n_slices)
{
	int t, col;
	for (t = 0; t < N_TESTS; t++) {
		for (col = 0; col < current_n_slices; col++) {
			hist[t][col] = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pointers must be aligned to frontier of 16 (or 32) bytes
void  par_histogram(int current_n_slices, int n_iter) {
	int iter, slice, test;

	int n_threads = omp_get_num_threads();
	int thread_num = omp_get_thread_num();
	int n_tests_per_threads = N_TESTS / n_threads;

	#pragma omp parallel for default(none) shared(n_threads,n_tests_per_threads, test) private(thread_num)
	for (test = 0; test < N_TESTS; test++)
	{
		srand(seeds[test + n_tests_per_threads * thread_num]);
		for (iter = 0; iter < n_iter; iter++) {
			slice = (rand() * current_n_slices) / (RAND_MAX + 1);
			hist[test][slice] ++;
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void seq_histogram(int current_n_slices, int n_iter) {
	int iter, slice, test;

	for (test = 0; test < N_TESTS; test++)
	{
		srand(seeds[test]);
		for (iter = 0; iter < n_iter; iter++) {
			slice = (rand() * current_n_slices) / (RAND_MAX + 1);
			hist[test][slice] ++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  accumulative_deviation(int current_n_slices, ElementType vec[]) {
	int k, test;
	for (test = 0; test < N_TESTS; test++)
	{
		ElementType acum = 0.0;
		for (k = 0; k < current_n_slices; k++) {
			acum += (hist[test][k] - (N_ITERATIONS / current_n_slices)) * (hist[test][k] - (N_ITERATIONS / current_n_slices));
		}
		vec[test] = acum / current_n_slices;
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

	double par_initial_time, par_elapsed_time, par_minimum_time = 1e+36;
	double seq_initial_time, seq_elapsed_time, seq_minimum_time = 1e+36;
	int range, slice_number = 0;
	int i;
	int test;

	omp_set_num_threads(MAX_N_THREADS_OR_TASKS);

	/*@ for (th = 0; th < MAX_N_THREADS_OR_TASKS; th++) {
		seeds[th] = prime_numbers[th]; // big prime numbers usually generate better random numbers.
	}*/
	for (i = 0; i < N_TESTS; i++) {
		seeds[i] = prime_numbers[i]; // big prime numbers usually generate better random numbers.
	}

	for (range = MIN_N_SLICES; range <= MAX_N_SLICES; range = range * 2, slice_number++)
	{
		// measuring tests
		par_minimum_time = 1e+36;
		seq_minimum_time = 1e+36;

		// FIRST EXECUTING THE SEQUENTIAL VERSION TO GET THE CORRECT RESULTS AND THE SEQ TIMING 
		//------------------------------------------------
		for (i = 0; i < N_REPETITIONS; i++)
		{
			matrix_vector_init(range);

			seq_initial_time = omp_get_wtime();
			// omp_get_wtime returns a value in seconds of the time elapsed from some point.
			// the problem is that the frequency is usually variable in current CPUs
			//------------------------------------------------

			seq_histogram(range, N_ITERATIONS); // Do the tests
 //------------------------------------------------
			seq_elapsed_time = omp_get_wtime() - seq_initial_time;
			if ((seq_elapsed_time) < seq_minimum_time)    seq_minimum_time = seq_elapsed_time;
		}

		accumulative_deviation(range, seq_std_deviation);

		// SECOND EXECUTING THE PARALLEL VERSION AND ITS PAR TIMING 
		//------------------------------------------------
		for (i = 0; i < N_REPETITIONS; i++)
		{
			matrix_vector_init(range);

			par_initial_time = omp_get_wtime();
			// omp_get_wtime returns a value in seconds of the time elapsed from some point.
			// the problem is that the frequency is usually variable in current CPUs
			//------------------------------------------------

			par_histogram(range, N_ITERATIONS); // Do the tests

			//------------------------------------------------
			par_elapsed_time = omp_get_wtime() - par_initial_time;
			if ((par_elapsed_time) < par_minimum_time)    par_minimum_time = par_elapsed_time;
		}
		accumulative_deviation(range, par_std_deviation);

		printf("# PAR and SEQ standard deviation for the N_SLICES: %8d\n", range);
		for (test = 0; test < N_TESTS; test++)
		{
			printf("%6d : %8lf: %8lf . Diff: %8lf \n", test, par_std_deviation[test], seq_std_deviation[test], par_std_deviation[test] - seq_std_deviation[test]);
			if (par_std_deviation[test] - seq_std_deviation[test] > 1e-2) {
				printf("** ERROR IN PARALLEL VERSION\n");
				return(-1);
			}
		}
		printf("- Size: %d . SEQ Minimum seconds for all the tests: %.6lf\n", range, seq_minimum_time);
		printf("- Size: %d . PAR Minimum seconds for all the tests: %.6lf\n", range, par_minimum_time);


	} //end of  'range' loop

	return(0);
}
