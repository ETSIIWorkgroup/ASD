using namespace std;

#include <iostream>
#include "QueryPerformanceTiming_rdtsc.h"

#define N_REPETITIONS 200  // repeticiones (solo para quedarse con el menor tiempo )
#define VSIZE 512 //número de elementos para los que ambos vectores ocupen 4 KB */
#define N 8192   // multiplicador máximo para probar con tamaños desde VSIZE hasta VSIZE*N 
#define MAX_TAM (VSIZE*N)  // maximo tamaño reservado para los vectores 

void codigo_c (int tam);
bool codigo_sse_escalar (int tam);
bool codigo_sse_vectorial (int tam);

//inicialización con instr. SSE: x[] = {1, 2, 3, 4, 1, 2, 3, 4,.. }, y[i] = sqrt(x[i])
void init(int tam); 

//Funciones para comprobar si los resultados son correctos 
bool checkCode(int tam);

// alinea los vectores en frontera de 16 bytes.
__declspec(align(16)) float x[MAX_TAM], y[MAX_TAM], m[4] = {1.0, 2.0, 3.0, 4.0};

float results[MAX_TAM]; //vector para  comprobar resultados 

float a = (float)3.14159; // Constante para los bucles

QPTimer c1,c2,c3;  // objeto tipo cronometros

//***********************************************************************
int main(void) {
    int i;

	c1.Calibrate();
	c2.Calibrate();
	c3.Calibrate();

    for (i=1; i<=N; i*=2) {
      	
		cout << "*** C Code for " << i*VSIZE << " elements ***" << endl;
		codigo_c (i*VSIZE); 
		//cout <<  "   First element: " << y[0] << " ; Last element: " << y[i*VSIZE-1] << endl ;
		c1.PrintMinimumCycles ("   Min. #cycles for all elements: "); cout << endl  ;
		c1.PrintMeanCycles    ("   Mean #cycles for all elements: ");
		c1.PrintMinimumCyclesByIteration("   Minimum time in cycles for an array element: ",i*VSIZE); cout << endl << endl;


		cout << "*** SSE Scalar for " << i*VSIZE << " elements ***" << endl;
		if (codigo_sse_escalar(i*VSIZE)) {
			c2.PrintMinimumCycles ("   Min. #cycles for all elements: "); cout << endl;
			c2.PrintMeanCycles    ("   Mean #cycles for all elements: ");
			c2.PrintMinimumCyclesByIteration("   Minimum time in cycles for an array element: ",i*VSIZE); cout << endl << endl ;
		}


		cout << "*** SSE Packet (vectorial)  for " << i*VSIZE << " elements ***" << endl ;
		if (codigo_sse_vectorial(i*VSIZE))
		{	c3.PrintMinimumCycles ("   Min. #cycles for all elements: "); cout << endl  ;
			c3.PrintMeanCycles    ("   Mean #cycles for all elements: ");
			c3.PrintMinimumCyclesByIteration("   Minimum time in cycles for an array element: ",i*VSIZE); cout << endl << endl ;
		}

		c1.ResetAll();
		c2.ResetAll();
		c3.ResetAll();
		cout << "----------------------------------------------------------" << endl;
	}
    return 0;
}

//***********************************************************************
//          FUNCIONES 
//***********************************************************************

//Código C en otra función para que no influyan las optimizaciones del compilador
void codigo_c (int tam) {
    int i, j;

    init (tam);
    for (j=0; j<N_REPETITIONS; j++) {
		c1.Start(); 
        for (i=0; i<tam; i++) 
		{	// Código a probar
			y[i] = a*x[i] + y[i];
		}
		c1.Stop();
		c1.Reset();
    }        

	//Guardo resultados correctos en vector results[]
    for(i=0;i<tam;i++)
		results[i] = y[i];
}
//***********************************************************************
//Código SSE escalar en ensamblador
bool codigo_sse_escalar (int tam) {
    int i;

    init (tam);
    for (i=0; i<N_REPETITIONS; i++) {
		c2.Start();
        __asm {
			// Código a probar
			lea eax, a
			lea ebx, x
			lea edx, y
			mov edi, tam
			lea ecx, [ebx+edi*4]
		    lea edi, y
			movss xmm2, [eax]  // escalar a parte menos significativa
	etiq_esc:
			// XXXXXXX   EL alumno debe escribir aquí el Código SSE escalar del SAXPY (debe ser un bucle)
			
		}

		c2.Stop();
		c2.Reset();
	}
    return checkCode(tam);
}

//***********************************************************************
// Código SSE vectorial en ensamblador
bool codigo_sse_vectorial (int tam) {
    int i;
	
    init (tam);
    for (i=0; i<N_REPETITIONS; i++) {
		c3.Start();
		__asm {
            	// Código a probar
				lea eax, a
    			lea ebx, x
	    		lea edx, y
            	mov edi, tam
				lea ecx, [ebx+edi*4]
            	lea edi, y
				movss xmm2, [eax]    //escalar a parte menos significativa
				shufps xmm2, xmm2, 0 //el elemento 0 se replica en todo xmm2
		etiq_vec:
				//XXXXXXX   EL alumno debe escribir aquí el código SSE vectorial del SAXPY (debe ser un bucle)

		}
		c3.Stop();
		c3.Reset();
	}
    return checkCode(tam);
}

//***********************************************************************
//inicialización con instr. SSE: x[]={1,2,3,4,1,2,3,4...}, y[i]=sqrt(x[i])
void init (int tam) {
	__asm {
		lea eax, m
		lea ebx, x
		lea edx, y
		mov edi, tam
        shl edi, 2
        mov ecx, ebx
        add ecx, edi
		movaps xmm0, [eax]
		sqrtps xmm1, xmm0
lp:
		movaps [ebx], xmm0
 		movaps [edx], xmm1
		add	ebx, 16
		add edx, 16
		cmp ebx, ecx
		jne  lp
	}
}
//***********************************************************************
bool checkCode (int tam) {
    int i = 0;

    while (fabs (results[i] - y[i]) < 1e-6 && i<tam)    
		i++;
	
    if (i != tam) {
		cout << "   -> WRONG RESULTS: check your code\n" << endl;
		return false;
	}
	return true;
}
