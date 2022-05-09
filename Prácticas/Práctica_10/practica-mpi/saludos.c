#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

	int mi_rango, p, fuente, dest;
	int etiqueta = 0;
	char mensaje[100];
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	if (mi_rango != 0) {
	
		sprintf(mensaje, "Saludos desde el proceso %d!", mi_rango);
		dest = 0;
		MPI_Send(mensaje, strlen(mensaje)+1, MPI_CHAR, dest, etiqueta, MPI_COMM_WORLD);
		
	} else {
	
		for (fuente = 1; fuente < p; fuente++) {
		
			MPI_Recv(mensaje, 100, MPI_CHAR, fuente, etiqueta, MPI_COMM_WORLD, &status);
			printf("%s\n", mensaje);
			
		}
		
	}
	
	MPI_Finalize();
	
}

