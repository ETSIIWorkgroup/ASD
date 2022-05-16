//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  version 2. 2021. Fernando Diaz del Rio 
//  THIS VERSION FULFILLS C++ STANDARD, SO, IT CAN BE USED IN VISUAL STUDIO
//   STUTDENTS MUST CHANGE THE VALUE OF CONSTANT "RANGE" , THEN COMPILE AND EXECUTE FOR DIFFERENT VALUES
//  - parallelizing scientific programs with OpenMP/MPI
//  - Parallelizing the generated histogram by a pseudorandom number generator
//
//-----------------------------------------------------

using namespace std;

#include <iostream>
#include <iomanip>
//#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//------------------------------------------------------------------
// project def and includes
// If the constant DEBUG_PRINT is defined you can do some  debugging by printing many steps and values below
// comment this line to do the final timing measures (and no printing is done): 
#define  DEBUG_PRINT_no

#ifdef DEBUG_PRINT
int debug_step = 0;
#endif

// this test uses measure CPU cycles more precisely using rdtsc asm instr. (see previous practices)
//#include "querypmrdtsc.h"

#define  N_REPETITIONS 13
// repeat several times each test to extract the minimum time
// and also to check if different executions return the same values

#define  RANGE ((unsigned long)256) // FIXED number of slices in which the original random numbers are divided 

#define  N_ITERATIONS  (100*(long)RANGE) //NUMBER of tested random numbers //big number to execute a considerable time . 
// IT MUST BE  a mulitple of MAX_N_SLICES (so that if the random generation were perfect, the histogram would be uniform)

#define  N_TESTS 2
// repeat several times the test with different seeds for the random generation 

typedef unsigned long   ElementType;

ElementType hist[N_TESTS][RANGE]; //FIXED SIZE
//it must contain the number of times that a certain slice is generated (here slice is the reduced number from the original number returned by rand())
// the first indexes is to save different tests, each with a different seed 


int prime_numbers[29] = { 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887 , 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997 };
int seeds[N_TESTS]; //A seed for each thread or process (task) is preferable 

double seq_std_deviation[N_TESTS];
double par_std_deviation[N_TESTS];

//protoype functions to initialize matrixes and do tests

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// matrixes and vectors  init
void matrix_vector_init(int n_tests_part, void *mat)
{
	ElementType(*h_part)[RANGE]   ;
	h_part = (ElementType(*)[RANGE] ) mat;
	int t, col;
	for (t = 0; t < n_tests_part; t++) {
		for (col = 0; col < RANGE; col++) {
			h_part[t][col] = 0;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pointers must be aligned to frontier of 16 (or 32) bytes
void  par_histogram(long int n_iter,  void *mat)
{
	ElementType(*h_part)[RANGE]   ;
	h_part = (ElementType(*)[RANGE] ) mat;
	int iter, test;
	unsigned long  slice;
	int        p = 444, my_rank = 333; //GIVE VALUE TO THESE VARIABLES 

	//@  STUDENTS MUST WRITE HERE THE PARALLEL VERSION” 
	 //@  STUDENTS MUST WRITE HERE THE PARALLEL VERSION” 
	 //@  STUDENTS MUST WRITE HERE THE PARALLEL VERSION” 

//	MPI_Comm_size(MPI_COMM_WORLD, &p);
	//MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	int n_test_per_process  = N_TESTS / p;

#ifdef DEBUG_PRINT
	printf("      Step: %3d . MPI computation begins . rank:%d out a total of :%d \n", debug_step++, my_rank, p);
#endif

#ifdef DEBUG_PRINT
	printf("      Step: %3d . MPI computation   ended   . rank:%d out a total of :%d \n", debug_step++, my_rank, p);
#endif

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void seq_histogram(long int n_iter) {
	int iter, test;
	unsigned long  slice;

	// IF THE STUDENT uses another pseudorandom generator, this checking is needed
	if (((unsigned long)RAND_MAX+1)%RANGE != 0) {
		/* if the RANGE is not a multiple of the number of random numbers that can be generated (given by (RAND_MAX+1))
		this test is not fair: some slices may have more opportunities than others
		*/
		printf("*** ERROR : This number of slices: %lu is not valid because \n     it is not a multiple of the number of random numbers that can be generated \n", RANGE);

		exit(-1);
	}

	for (test = 0; test < N_TESTS; test++)
	{
		srand(seeds[test]);
		for (iter = 0; iter < n_iter; iter++) {
			slice = (rand() * RANGE) / ((unsigned long)RAND_MAX + 1);
			hist[test][slice] ++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void  accumulative_deviation(double  vec[]) {
	int k, test;
	for (test = 0; test < N_TESTS; test++)
	{
		double acum = 0.0;
		for (k = 0; k < RANGE; k++) {
			acum += (hist[test][k] - (N_ITERATIONS / RANGE)) * (hist[test][k] - (N_ITERATIONS / RANGE));
		}
		vec[test] = acum / RANGE;
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

	double par_initial_time, par_elapsed_time, par_minimum_time = 1e+36;
	double seq_initial_time, seq_elapsed_time, seq_minimum_time = 1e+36;
	unsigned long slice_number = 0;
	int test;
	int  i, p, my_rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	for (i = 0; i < N_TESTS; i++) {
		seeds[i] = prime_numbers[i]; // big prime numbers usually generate better random numbers.
	}

#ifdef DEBUG_PRINT
		printf("      Step: %3d .RANGE : %lu \n", debug_step++, RANGE);
#endif

		if (my_rank == 0) {
			// measuring tests
			par_minimum_time = 1e+36;
			seq_minimum_time = 1e+36;

			// FIRST EXECUTING THE SEQUENTIAL VERSION TO GET THE CORRECT RESULTS AND THE SEQ TIMING 
			//------------------------------------------------
			for (i = 0; i < N_REPETITIONS; i++)
			{
				matrix_vector_init(N_TESTS , hist);
				
				seq_initial_time = MPI_Wtime();
				// omp_get_wtime returns a value in seconds of the time elapsed from some point.
				// the problem is that the frequency is usually variable in current CPUs
				//------------------------------------------------

				seq_histogram(N_ITERATIONS); // Do the tests
	 //------------------------------------------------
				seq_elapsed_time = MPI_Wtime() - seq_initial_time;
				if ((seq_elapsed_time) < seq_minimum_time)    seq_minimum_time = seq_elapsed_time;
			}
		} //end of if(my_rank==0) {

#ifdef DEBUG_PRINT
			printf("      Step: %3d .SEQ version Finished. \n", debug_step++);
#endif

		accumulative_deviation(seq_std_deviation);

		// SECOND EXECUTING THE PARALLEL VERSION AND ITS PAR TIMING 
		
		int n_test_per_process  = N_TESTS / p;

		//malloc:  dinamically allocate memory for each slave:
// This is a pointer to dinamically allocate memory for each slave:
ElementType(*hist_partial)[RANGE];

		hist_partial = (ElementType(*)[RANGE]) malloc(n_test_per_process*RANGE * sizeof(ElementType));
		
		//------------------------------------------------
		for (i = 0; i < N_REPETITIONS; i++)
		{
			//matrix_vector_init(N_TESTS , hist);
			matrix_vector_init(n_test_per_process, hist_partial);

			if (my_rank == 0) {				
				par_initial_time = MPI_Wtime();
			}
			MPI_Barrier(MPI_COMM_WORLD);


			//------------------------------------------------
#ifdef DEBUG_PRINT
			printf("      Step: %3d . Calling to par_histogram. \n", debug_step++);
#endif
			//------------------------------------------------

			par_histogram( N_ITERATIONS, hist_partial); // Do the tests

#ifdef DEBUG_PRINT
			printf("      Step: %3d . Finished par_histogram. \n", debug_step++);
#endif
			//------------------------------------------------

#ifdef DEBUG_PRINT
	printf("      Step: %3d . MPI is going to collect results  . rank:%d out a total of :%d \n", debug_step++, my_rank, p);
#endif



	//@  STUDENTS MUST WRITE HERE THE MESSAGES TO COLLECT DATA  
	//@  STUDENTS MUST WRITE HERE THE MESSAGES TO COLLECT DATA  
	//@  STUDENTS MUST WRITE HERE THE MESSAGES TO COLLECT DATA  
	



#ifdef DEBUG_PRINT
	printf("      Step: %3d . MPI Barrier  . rank:%d out a total of :%d \n", debug_step++, my_rank, p);
#endif
			MPI_Barrier(MPI_COMM_WORLD);
			if (my_rank == 0) {
				par_elapsed_time = MPI_Wtime() - par_initial_time;
				if ((par_elapsed_time) < par_minimum_time)    par_minimum_time = par_elapsed_time;
			}
		}  //END OF  N_REPETITIONS

		if (my_rank == 0) {
			accumulative_deviation(par_std_deviation);

			printf("# PAR and SEQ standard deviation for the N_SLICES: %8lu\n", RANGE);
			for (test = 0; test < N_TESTS; test++)
			{
				printf("%6d : %8lf: %8lf . Diff: %8lf \n", test, par_std_deviation[test], seq_std_deviation[test], par_std_deviation[test] - seq_std_deviation[test]);
				if (par_std_deviation[test] - seq_std_deviation[test] > 1e-2) {
					printf("** ERROR IN PARALLEL VERSION\n");
					MPI_Finalize();
					return(-1);
				}
			}
			printf("- Size: %lu . SEQ Minimum seconds for all the tests: %.6lf\n", RANGE, seq_minimum_time);
			printf("- Size: %lu . PAR Minimum seconds for all the tests: %.6lf\n", RANGE, par_minimum_time);
			}

		free(hist_partial);

	MPI_Finalize();

}
