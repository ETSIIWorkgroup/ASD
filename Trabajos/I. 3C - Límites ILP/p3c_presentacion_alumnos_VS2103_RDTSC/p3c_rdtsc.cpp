//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  ASD subject 3º GTI
//  version 1. Nov. 2012. Fernando Diaz del Rio
//  version 2a. March. 2017. Fernando Diaz del Rio
//  
//  - Doing several test to reach ILP limits in superscalar processor 
//  - TEST TIMING OF simple routines 
//  - this test uses a new version of QueryPerformanceTiming , to measure CPU cycles more precisely using RDTSC asm instr.
//		Sometimes RDTSC fails and returns values smaller than expected. In this case try the other version of this test (p3c_proy_QPC_alumnos.zip)
//
//  Debug mode of VS is very slow (it only serves to check bugs in the code)
//  Uses Release mode of VS to measure times 
//-----------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//	Práctica 3C - Arquitectura de Sistemas Distribuidos - Universidad de Sevilla Curso 2021/2022
// 
//	Alumnos:
//	- Pedro Escobar Rubio
//	- Alejandro Fernández Trigo
// 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

#define  N_REPETIC 256
// Repeat several times each test to extract the minimum time
// and to have the caches filled

#define  N_ELEM (1024*2)
// Big number to have a mean time (but not very big to avoid cache misses) 

//#define N_ELEM (1024*512)
// Tamaño de vectores mucho más grande para pruebas

__declspec(align(64)) int a_int[N_ELEM ], b_int[N_ELEM ];

__declspec(align(64)) float a[N_ELEM ], b[N_ELEM ];
__declspec(align(64)) float c[N_ELEM ], d[N_ELEM ];
__declspec(align(64)) float e[N_ELEM ], f[N_ELEM ];
__declspec(align(64)) float g[N_ELEM ], h[N_ELEM ];
float z;

__declspec(align(64)) int cond1[N_ELEM ], cond2[N_ELEM ], cond3[N_ELEM ];

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vectors_init ()
{
	int i;
	for (i=0;i<N_ELEM;i++)
	{
		// a[i] must have little values to avoid overflow in pp1()
		a[i]=(float)rand()/(float)RAND_MAX;
		b[i]=(float)i/5;
		c[i]=(float)i/6;
		d[i]=(float)i/7;
		e[i]=(float)i/8;
		f[i]=(float)i/9;
		g[i]=(float)i/10;
		h[i]=(float)i/11;

		a_int[i]=i;

		cond1[i]=1;
		cond3[i]=rand()%2;
	}

	for (i=0;i<N_ELEM;i+=8)
	{
		// put here the pattern behaviour  you want; BTB is very clever !!
		cond2[i+0]=1;
		cond2[i+1]=0;
		cond2[i+2]=1;
		cond2[i+3]=1;
		cond2[i+4]=0;
		cond2[i+5]=1;
		cond2[i+6]=1;
		cond2[i+7]=1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// a. Una suma por iteración
/*
	PC I
		- /O2
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
	PC II
		- /O2 
			* Vectores normales
				-> Instrucciones /SSE: 3.45996 ciclos
				-> Instrucciones /IA32: 3.45996 ciclos
			* Vectores grandes:
				-> Instrucciones /SSE: ciclos
				-> Instrucciones /IA32: 
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
*/
float problemA ()
{
	int  i;
	float z = 0.0; 

	for (i=0;i<N_ELEM;i++)
	{
		z = z + a[i];
	}

	return z;
}

// b. Dos sumas por iteración
/*
	PC I
		- /O2
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 3.46484 ciclos
				-> Instrucciones /IA32: 3.46191 ciclos
			* Vectores grandes:
				-> Instrucciones /SSE: ciclos
				-> Instrucciones /IA32:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
*/
float problemB ()
{
	int  i;
	float z = 0.0;

	for (i=0;i<N_ELEM;i++)
	{
		// Suma de dos vectores
		z = z + (a[i] + b[i]);

		// Otra forma: Suma de un vector y una constante
		// z = z + (a[i] + 3.0);
	}

	return z;
}

// c. Suma condicional - Dificilmente Predecible 
/*
	PC I
		- /O2
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 3.47266 ciclos
				-> Instrucciones /IA32: 3.47168 ciclos
			* Vectores grandes:
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
*/
float problemC ()
{
	int  i;
	float z = 0.0;

	for (i=0;i<N_ELEM;i++)
	{
		// cond3 contiene valores aleatorios (entre 0 y 1) dificilmente predecibles
		if (cond3[i] == 1) {
			z = z + a[i];
		}
		else {
			z = z + b[i];
		}
		
	}

	return z; 
}

// c. Suma condicional - Muy Fácilmente Predecible 
/*
	Para valores MUY fácilmente predecibles:
	PC I
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 
				-> Instrucciones /IA32:
			* Vectores grandes:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 3.4707 ciclos
				-> Instrucciones /IA32: 3.36133 ciclos
			* Vectores grandes:
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
*/
float problemD1 ()
{
	int  i;
	float z = 0.0;

	for (i=0;i<N_ELEM;i++)
	{
		// Para valores MUY facilmente predecibles (todos son 1)
		if (cond1[i] == 1) {
			z = z + a[i];
		}
		else {
			z = z + b[i];
		}
		
	}

	return z;
}

// c. Suma condicional - Fácilmente Predecible 
/*
	Para valores fácilmente predecibles:
	PC I
		- /O2
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 3.35938 ciclos
				-> Instrucciones /IA32: 3.35645 ciclos
			* Vectores grandes:
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
*/
float problemD2()
{
	int  i;
	float z = 0.0;

	for (i = 0;i < N_ELEM;i++)
	{
		// Para valores predecibles (entre 0 y 1)
		if (cond2[i] == 1) {
			z = z + a[i];
		}
		else {
			z = z + b[i];
		}

	}

	return z;
}

// Ejemplo provisto en el proyecto
/*
	PC I
		- /O2
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 1.02344 ciclos
				-> Instrucciones /IA32: 1.27148 ciclos
			* Vectores grandes:
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
			* Vectores grandes:
*/
float example ()
{
	int i;

	for (i=0;i<N_ELEM;i++)
	{
		a[i]=b[i]+8;
	}
	
	return a[N_ELEM-1 ];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char** argv ) {

	float var1, var2, var3, var4, var5, var6;
	int i;

	QPTimer c1, c2, c3, c4, c5, c6;

	c1.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	// measuring tests
	for (i=0;i<N_REPETIC;i++)
	{
		vectors_init ();
		c1.Start();  // start timer
		var1=example(); // Do the test
		c1.Stop();  // stop timer
		c1.Reset();
	} 
	// end of    testing

	c2.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	// measuring tests
	for (i=0;i<N_REPETIC;i++)
	{
		vectors_init ();
		c2.Start();  // start timer
		var2=problemA(); // Do the test
		c2.Stop();  // stop timer
		c2.Reset();
	} 
	// end of    testing

	c3.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	// measuring tests
	for (i = 0;i < N_REPETIC;i++)
	{
		vectors_init();
		c3.Start();  // start timer
		var3 = problemB(); // Do the test
		c3.Stop();  // stop timer
		c3.Reset();
	}
	// end of    testing

	c4.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	// measuring tests
	for (i = 0;i < N_REPETIC;i++)
	{
		vectors_init();
		c4.Start();  // start timer
		var4 = problemC(); // Do the test
		c4.Stop();  // stop timer
		c4.Reset();
	}
	// end of    testing

	c5.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	// measuring tests
	for (i = 0;i < N_REPETIC;i++)
	{
		vectors_init();
		c5.Start();  // start timer
		var5 = problemD1(); // Do the test
		c5.Stop();  // stop timer
		c5.Reset();
	}
	// end of    testing

	c6.Calibrate(); //calibrates timer overhead and set cronometer to zero	
	// measuring tests
	for (i = 0;i < N_REPETIC;i++)
	{
		vectors_init();
		c6.Start();  // start timer
		var6 = problemD2(); // Do the test
		c6.Stop();  // stop timer
		c6.Reset();
	}
	// end of    testing

	cout << endl << "ONLY PRINTING OUTPUT VARIABLE TO PREVENT THAT THE COMPILER ELIMINATES FUNCTION CALLS: " 
		<< var1 << ", " << var2 << ", " << var3 << ", " << var4 << ", " << var5 << ", " << var6 << ", " << endl;

	cout << "-Number of elements N_ELEM: " << N_ELEM << endl;
	cout << "-Number of measures example: " << c1.NumberOfMeasures() << endl;
	cout << "-Number of measures problemA: " << c2.NumberOfMeasures() << endl;
	cout << "-Number of measures problemB: " << c3.NumberOfMeasures() << endl;
	cout << "-Number of measures problemC: " << c4.NumberOfMeasures() << endl;
	cout << "-Number of measures problemD1: " << c5.NumberOfMeasures() << endl;
	cout << "-Number of measures problemD2: " << c6.NumberOfMeasures() << endl;
	cout << endl;

	// uncomment this if more timing results were needed:
	/*c1.PrintMinimumTime   (" Minimum time in seconds for example is:   ");
	c1.PrintMeanTime      (" Mean time in seconds    for example is:   ");

	c2.PrintMinimumCycles (" Minimum time in cycles for problem  is:   ");
	c2.PrintMeanCycles    (" Mean time in cycles for problem is    :   ");
	*/

	c1.PrintMinimumCyclesByIteration  (" Minimum time in cycles for an element of 'example' is: ", N_ELEM);
	c1.PrintMinimumTime  (" Minimum time in seconds for example is:   ");
	c1.PrintMeanTime  (" Mean time in seconds for example is:   ");
	cout << endl;
	cout << endl;

	c2.PrintMinimumCyclesByIteration  (" Minimum time in cycles for an element of 'problem A' is: ", N_ELEM);
	c2.PrintMinimumTime(" Minimum time in seconds for example is:   ");
	c2.PrintMeanTime(" Mean time in seconds for example is:   ");
	cout << endl;
	cout << endl;

	c3.PrintMinimumCyclesByIteration(" Minimum time in cycles for an element of 'problem B' is: ", N_ELEM);
	c3.PrintMinimumTime(" Minimum time in seconds for example is:   ");
	c3.PrintMeanTime(" Mean time in seconds for example is:   ");
	cout << endl;
	cout << endl;

	c4.PrintMinimumCyclesByIteration(" Minimum time in cycles for an element of 'problem C' is: ", N_ELEM);
	c4.PrintMinimumTime(" Minimum time in seconds for example is:   ");
	c4.PrintMeanTime(" Mean time in seconds for example is:   ");
	cout << endl;
	cout << endl;

	c5.PrintMinimumCyclesByIteration(" Minimum time in cycles for an element of 'problem D1' is: ", N_ELEM);
	c5.PrintMinimumTime(" Minimum time in seconds for example is:   ");
	c5.PrintMeanTime(" Mean time in seconds for example is:   ");
	cout << endl;
	cout << endl;

	c6.PrintMinimumCyclesByIteration(" Minimum time in cycles for an element of 'problem D2' is: ", N_ELEM);
	c6.PrintMinimumTime(" Minimum time in seconds for example is:   ");
	c6.PrintMeanTime(" Mean time in seconds for example is:   ");
	cout << endl;
	cout << endl;

}
