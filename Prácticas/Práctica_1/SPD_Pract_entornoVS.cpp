//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  SPD subject 3º GIC
//  version 1. Sept. 2019. Fernando Diaz del Rio
//  
//  - Using VS enviroment and AC review 
//
//		PRÁCTICAS ASD - ALEJANDRO FERNÁNDEZ TRIGO
//
//-----------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <iostream>

using namespace std;
//------------------------------------------------------------------
// project def and includes

// this test uses measure CPU cycles more precisely using rdtsc asm instr.
#include "QueryPerformanceTiming_rdtsc.h"

#define  N_REPETITIONS 50
// repeat several times each test to extract the minimum and mean time
// (In subsequent repetitions, caches would be filled)

#define  N_ROWS (512)  //big number to execute a considerable time 
#define  N_COLS (2*1024)  //big number to execute a considerable time 

float a;
float m1[N_ROWS][N_COLS], m2[N_ROWS][N_COLS];
float m3[N_ROWS][N_COLS];

//protoype functions to initialize matrixes and do tests
void matrix_init(void);
void pp_a(void);
void pp_b(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

	QPTimer crono_a, crono_b;
	crono_a.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	crono_b.Calibrate(); //calibrates timer overhead and set cronometer to zero	

	matrix_init();
	// measuring tests
	for (int i = 0; i < N_REPETITIONS; i++) {
		crono_a.Start();  // start timer
		pp_a(); // Do the test
		crono_a.Stop();  // stop timer
		crono_a.Reset();
	}

	matrix_init();
	for (int i = 0; i < N_REPETITIONS; i++) {
		crono_b.Start();  // start timer
		pp_b(); // Do the test
		crono_b.Stop();  // stop timer
		crono_b.Reset();
	}
	// end of    testing

	cout << endl << "-Num. de iteraciones : " << N_ROWS*N_COLS << endl;
	cout << endl << "-Num. de mediciones: " << crono_a.NumberOfMeasures() << endl;

	crono_a.PrintMinimumCycles			(" Tiempo minimo en ciclos para las repeticiones de pp_a()  es:   ");
	crono_b.PrintMinimumCycles			(" Tiempo minimo en ciclos para las repeticiones de pp_b()  es:   ");

	crono_a.PrintMeanCycles  (" Tiempo medio  en ciclos de pp_a() es: ");
	crono_b.PrintMeanCycles  (" Tiempo medio  en ciclos de pp_b() es: ");

	crono_b.PrintMinimumCyclesByIteration(" Tiempo minimo en ciclos por instruccion dentro del bucle de pp_b() es: ", 4*N_ROWS*N_COLS);

	cout << endl;

	/* English version
	cout << endl << "-Number of iterations N_ROWS: " << N_ROWS*N_COLS << endl;
	cout << endl << "-Number of measures: " << crono_a.NumberOfMeasures() << endl;

	crono_a.PrintMinimumCycles(" Minimum time in cycles for an iteration of pp_a() is:   ", N_ROWS);
	crono_b.PrintMinimumCycles(" Minimum time in cycles for an iteration of pp_b() is:   ", N_ROWS);

	   crono_a.PrintMeanCyclesByIteration(" Mean time in cycles for an iteration of pp_a()    is:   ", N_REPETITIONS);
	   crono_b.PrintMeanCyclesByIteration(" Mean time in cycles for an iteration of pp_b()    is:   ", N_REPETITIONS);

	cout << endl;
	*/

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// tests
void pp_a() {
	for (int c = 0; c < N_COLS; c++) {
		for (int r = 0; r < N_ROWS; r++) {
			m3[r][c] = a * m1[r][c] + m2[r][N_COLS - c - 1];
		}
	}
}
void  pp_b() {
	for (int r = 0; r < N_ROWS; r++) {
		for (int c = 0; c < N_COLS; c++) {
			m3[r][c] = a * m1[r][c] + m2[r][N_COLS - c - 1];
		}
	}
}

///////////////////////////////////////////////////////
// initialization 
void matrix_init() {
	// any initiallization of the matrix elements is valid
	
	for (int r = 0; r < N_ROWS; r++) {
		for (int c = 0; c < N_COLS; c++) {
			m1[r][c] = a + r + c;
			m2[r][c] = (float)3.1415926 + r*r - c;
		}
	}
}
