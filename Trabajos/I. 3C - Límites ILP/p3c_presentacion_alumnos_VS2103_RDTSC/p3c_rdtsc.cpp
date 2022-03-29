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
// Repetir múltiples veces para sacar el valor mínimo y llenar las cachés

//#define  N_ELEM (1024*2)
// Tamaño de vectores suficientemente grandes para tener un tiempo medio pero NO tener fallos de caché

#define N_ELEM (1024*1024)
// Tamaño de vectores mucho más grande para pruebas
/*
 Un float son 4 bytes; entonces, 1024 · 2 · 4 = 8192 bytes. Si las cachés son de:
	
	PC I
	- L1 6x32KB (196608B) > Habría que saturarlo con 1024*60*4
	- L2 6x256KB (1572864B) > Habría que saturarlo con 1024*500*4
	- L3 12MB (12*10^6B) > Habría que saturarlo con 1024*4096*4
	- RAM 16GB (16*10^9B)

	PC II
	- L1 2x32KB (65536B) > Habría que saturarlo con 1024*25*4
	- L2 2x256KB (524288B) > Habría que saturarlo con 1024*500*4
	- L3 4MB (4*10^6B) > Habría que saturarlo con 1024*1024*4
	- RAM 4GB (4*10^9B) 
*/

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
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- Vectores grandes (/O2 /SSE): 
				-> L1:
				-> L2:
				-> L3:
	PC II
		- /O2 
			* Vectores normales
				-> Instrucciones /SSE: 3.46094 ciclos - 2.53143e-06 segundos
				-> Instrucciones /IA32: 3.45996 ciclos - 2.53071e-06 segundos
		- /Od
			* Vectores normales
				-> Instrucciones /SSE: 3.45898 ciclos - 2.53e-06 segundos
				-> Instrucciones /IA32: 3.45801 ciclos - 2.52929e-06 segundos
		- Vectores grandes (/O2 /SSE):
				-> L1: 3.86992 ciclos - 3.53821e-05 segundos
				-> L2: 3.7221 ciclos - 0.000680613 segundos
				-> L3: 4.1539 ciclos - 0.0015556 segundos
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
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- Vectores grandes (/O2 /SSE):
				-> L1:
				-> L2:
				-> L3:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 3.46484 ciclos - 2.53429e-06 segundos
				-> Instrucciones /IA32: 3.46191 ciclos - 2.53214e-06 segundos
		- /Od
			* Vectores normales
				-> Instrucciones /SSE: 3.46387 ciclos - 2.53357e-06 segundos
				-> Instrucciones /IA32: 3.46191 ciclos - 2.53214e-06 segundos
		- Vectores grandes (/O2 /SSE):
				-> L1: 3.35359 ciclos - 3.06614e-05 segundos
				-> L2: 3.37059 ciclos - 0.000616337 segundos
				-> L3: 3.38062 ciclos - 0.00126601 segundos
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
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- Vectores grandes (/O2 /SSE):
				-> L1:
				-> L2:
				-> L3:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 3.47461 ciclos - 2.54143e-06 segundos 
				-> Instrucciones /IA32: 3.40039 ciclos - 2.48714e-06 segundos
		- /Od
			* Vectores normales
				-> Instrucciones /SSE: 3.40234 ciclos - 2.48857e-06 segundos
				-> Instrucciones /IA32: 3.47266 ciclos - 2.54e-06 segundos
		- Vectores grandes (/O2 /SSE): 
				-> L1: 3.3618 ciclos - 3.07364e-05 segundos
				-> L2: 4.25164 ciclos - 0.000777444 segundos
				-> L3: 4.23906 ciclos - 0.00158749 segundos
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
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- Vectores grandes (/O2 /SSE):
				-> L1:
				-> L2:
				-> L3:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 3.41309 ciclos - 2.49643e-06 segundos
				-> Instrucciones /IA32: 3.38184 ciclos - 2.47357e-06 segundos
		- /Od
			* Vectores normales
				-> Instrucciones /SSE: 3.41504 ciclos - 2.49786e-06 segundos
				-> Instrucciones /IA32: 3.47168 ciclos - 2.53929e-06 segundos
		- Vectores grandes (/O2 /SSE):
				-> L1: 3.36023 ciclos - 3.07221e-05 segundos
				-> L2: 3.89562 ciclos - 0.000712343 segundos
				-> L3: 3.91309 ciclos - 0.00146542 segundos
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
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- Vectores grandes (/O2 /SSE):
				-> L1:
				-> L2:
				-> L3:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 3.36035 ciclos - 2.45786e-06 segundos
				-> Instrucciones /IA32: 3.3916 ciclos - 2.48071e-06 segundos
		- /Od
			* Vectores normales
				-> Instrucciones /SSE: 3.47266 ciclos - 2.54e-06 segundos
				-> Instrucciones /IA32: 3.46973 ciclos - 2.53786e-06 segundos
		- Vectores grandes (/O2 /SSE):
				-> L1: 3.36141 ciclos - 3.07329e-05 segundos
				-> L2: 4.06821 ciclos - 0.000743902 segundos
				-> L3: 4.16774 ciclos - 0.00156078 segundos
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
		- /Od
			* Vectores normales
				-> Instrucciones /SSE:
				-> Instrucciones /IA32:
		- Vectores grandes (/O2 /SSE):
				-> L1:
				-> L2:
				-> L3:
	PC II
		- /O2
			* Vectores normales
				-> Instrucciones /SSE: 1.01465 ciclos - 7.42143e-07 segundos
				-> Instrucciones /IA32: 1.02539 ciclos - 7.5e-07 segundos
		- /Od
			* Vectores normales
				-> Instrucciones /SSE: 1.00391 ciclos - 7.34286e-07 segundos
				-> Instrucciones /IA32: 0.998047 ciclos - 7.3e-07 segundos
		- Vectores grandes (/O2 /SSE): 
				-> L1: 1.12234 ciclos - 1.02614e-05 segundos
				-> L2: 2.69023 ciclos - 0.000491927 segundos
				-> L3: 2.68793 ciclos - 0.00100661 segundos
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

	// Otras mediciones posibles:
	/*
	c2.PrintMinimumCycles (" Minimum time in cycles for problem  is:   ");
	c2.PrintMeanCycles    (" Mean time in cycles for problem is    :   ");
	*/

	c1.PrintMinimumCyclesByIteration  (" Minimum time in cycles for an element of 'example' is: ", N_ELEM);
	c1.PrintMinimumTime  (" Minimum time in seconds for example is:   ");
	c1.PrintMeanTime  (" Mean time in seconds for example is:   ");
	cout << endl;
	cout << endl;

	c2.PrintMinimumCyclesByIteration  (" Minimum time in cycles for an element of 'problem A' is: ", N_ELEM);
	c2.PrintMinimumTime(" Minimum time in seconds for problem A is:   ");
	c2.PrintMeanTime(" Mean time in seconds for problem A is:   ");
	cout << endl;
	cout << endl;

	c3.PrintMinimumCyclesByIteration(" Minimum time in cycles for an element of 'problem B' is: ", N_ELEM);
	c3.PrintMinimumTime(" Minimum time in seconds for problem B is:   ");
	c3.PrintMeanTime(" Mean time in seconds for problem B is:   ");
	cout << endl;
	cout << endl;

	c4.PrintMinimumCyclesByIteration(" Minimum time in cycles for an element of 'problem C' is: ", N_ELEM);
	c4.PrintMinimumTime(" Minimum time in seconds for problem C is:   ");
	c4.PrintMeanTime(" Mean time in seconds for problem C is:   ");
	cout << endl;
	cout << endl;

	c5.PrintMinimumCyclesByIteration(" Minimum time in cycles for an element of 'problem D1' is: ", N_ELEM);
	c5.PrintMinimumTime(" Minimum time in seconds for problem D1 is:   ");
	c5.PrintMeanTime(" Mean time in seconds for problem D1 is:   ");
	cout << endl;
	cout << endl;

	c6.PrintMinimumCyclesByIteration(" Minimum time in cycles for an element of 'problem D2' is: ", N_ELEM);
	c6.PrintMinimumTime(" Minimum time in seconds for problem D2  is:   ");
	c6.PrintMeanTime(" Mean time in seconds for problem D2 is:   ");
	cout << endl;
	cout << endl;

}
