/* prod_escalar_mpi.c
 * CALCULO DEL PRODUCTO ESCALAR DE DOS VECTORES
 *
 * ENTRADA: NINGUNA.
 * SALIDA:  PRODUCTO ESCALAR
 */
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#define ELEMENTOS   (2*2*2*2*3*3*5*7*11*13*200) // vale  720720*200
/* ATENCION: EL Nº DE ELEMENTOS DEBE SER DIVISIBLE POR EL Nº DE PROCESOS (p)! */

float       x[ELEMENTOS], y[ELEMENTOS];		// VECTORES (VARIABLES GLOBALES)

/* FUNCION QUE CALCULA EL PRODUCTO ESCALAR LOCAL (DE UN TROZO DE LOS VECTORES) */
float prod_escalar_serie(
	float a[], // ENTRADA
	float b[], // ENTRADA
	float n   /* ENTRADA: NUMERO DE ELEMENTOS */ );

main(int argc, char** argv) {
    int         mi_rango;  				// RANGO DE MI PROCESO
    int         p;        				// NUMERO DE PROCESOS
    int		n = ELEMENTOS;				// NUMERO DE ELEMENTOS DE LOS VECTORES
    int		n_local;				// NUMERO DE ELEMENTOS DE CADA FRAGMENTO
    int 	inicio_vector_local;			// INDICE DE INICIO DE CADA FRAGMENTO
    float       suma_local;	 			// PRODUCTO ESCALAR SOBRE MI INTERVALO
    float       suma_total;     			// PRODUCTO ESCALAR TOTAL
    int         fuente;   				// PROCESO QUE ENVIA RESULTADO DE SUMA
    int         dest = 0; 				// DESTINATARIO: TODOS LOS MENSAJES VAN A 0
    int         etiqueta = 0;
    int		i;
    MPI_Status  status;

    MPI_Init(&argc, &argv);				// INICIALIZA MPI

    /* INICIALIZA LOS VECTORES */
    for (i = 0; i < n; i++)
          x[i] = y[i] = i%5;

	// ARRANCAR EL CRONOMETRO AQUI
	
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango); 		// AVERIGUA EL RANGO DE MI PROCESO
    MPI_Comm_size(MPI_COMM_WORLD, &p);			// AVERIGUA CUANTOS PROCESOS HAY
    n_local = n/p;
    inicio_vector_local = mi_rango * n_local;
    suma_local = prod_escalar_serie(&x[inicio_vector_local], &y[inicio_vector_local], n_local);
    printf("MI RANGO = %d , SUMA LOCAL = %f\n", mi_rango, suma_local);

    /* SUMA LAS CONTRIBUCIONES CALCULADAS POR CADA PROCESO */
    if (mi_rango == 0) {
        suma_total = suma_local;
        for (fuente = 1; fuente < p; fuente++) {
            MPI_Recv(&suma_local, 1, MPI_FLOAT, fuente, etiqueta, MPI_COMM_WORLD, &status);
	    suma_total = suma_total + suma_local;
        }
    } else {  
        MPI_Send(&suma_local, 1, MPI_FLOAT, dest, etiqueta, MPI_COMM_WORLD);
    }

    /* MUESTRA EL RESULTADO POR PANTALLA */
    if (mi_rango == 0) {
	printf("PRODUCTO ESCALAR USANDO p=%d TROZOS DE LOS VECTORES X E Y = %f\n", p, suma_total);
    }

	// PARAR EL CRONOMETRO AQUI
	/*
	PARA IMPRIMIR TIEMPOS USE ALGO COMO ESTO:
	if (mi_rango == 0)
		printf("TIEMPO DE EJECUCION = %lf s\n", tiempo_total);
	*/
	
    MPI_Finalize();					// CIERRA EL UNIVERSO MPI */
} /* MAIN  */

/* FUNCION QUE CALCULA EL PRODUCTO ESCALAR LOCAL (DE UN TROZO DE LOS VECTORES) */
float prod_escalar_serie(
		float a[], // ENTRADA
		float b[], // ENTRADA
		float n   /* ENTRADA: NUMERO DE ELEMENTOS */ ){
	int i;
	float suma = 0.0;
	for (i = 0; i < n; i++)
		suma = suma + a[i] * b[i];
	return suma;
} /*  PROD_ESCALAR_SERIE  */
