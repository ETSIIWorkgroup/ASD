#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

  int mi_rango, tamanno;
  
  MPI_Init(&argc, &argv);		/* Inicializa MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);
  MPI_Comm_size(MPI_COMM_WORLD, &tamanno);

  if ( mi_rango == 0 )
  	printf("Soy el proceso %i de %i: !Hola mundo!\n", mi_rango, tamanno);
  else
  	printf("Yo soy el proceso %i de %i.\n", mi_rango, tamanno);

  MPI_Finalize();			/* Cierra el universo MPI */
  return(0);
}
