#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {

  int mi_rango, tamanno, numero;
  char nombre[100];
  
  MPI_Init(&argc, &argv);		/* Inicializa MPI */
  
  MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);
  
  MPI_Comm_size(MPI_COMM_WORLD, &tamanno);
  
  MPI_Get_processor_name(nombre, &numero);

  if ( mi_rango == 0 )
  	printf("Soy el proceso %i de %i corriendo en  %s !Hola mundo!\n", mi_rango, tamanno, nombre);
  else
  	printf("Yo soy el proceso %i de %i, corriendo en  %s \n", mi_rango, tamanno, nombre);

  MPI_Finalize();			/* Cierra el universo MPI */
  return(0);
}
