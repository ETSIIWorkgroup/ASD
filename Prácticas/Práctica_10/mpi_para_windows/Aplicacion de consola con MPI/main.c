#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]) 
{
  int		mi_rango;			// rango de mi proceso
  int		num_proc;			// numero de procesos
  int		fuente;				// rango del emisor
  char		mensaje[100];		// almacenamiento para el mensaje
  char      buffer_mpi[1024];   // Buffer MPI para alojar localmente los mensajes a enviar
  int       tambuffer_mpi = sizeof buffer_mpi;  // Tamaño de este buffer

  MPI_Init(&argc, &argv);		// Inicializa MPI
  MPI_Buffer_attach(buffer_mpi, tambuffer_mpi);  // Asignamos un buffer para que los envios no se bloqueen
  MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);      // Quien soy?
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);      // Cuántos como yo hay?  
  
  /* Crea mensaje */
  sprintf (mensaje, "Saludos desde el proceso %d!", mi_rango);
  /* Usa strlen+1 para que '\0' sea transmitido */
  MPI_Bsend(mensaje, strlen(mensaje)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);  
  //OJO! Uso MPI_Bsend en lugar de MPI_Send para que el envío se guarde en el buffer y de esa forma no se bloquee cuando el envío es local (de proceso 0 a proceso 0)

  if (mi_rango == 0)  // si soy el primer proceso...
  {
	  for (fuente = 0; fuente < num_proc; fuente++)  // ... espero el mensaje de todos los demás (incluído yo)
	  {
		  MPI_Recv(mensaje, 100, MPI_CHAR, fuente, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // lo recibo
		  puts(mensaje);                                                                   // y lo imprimo en pantalla
	  }
  }
  
  MPI_Buffer_detach(buffer_mpi, &tambuffer_mpi);  // ya podemos deasignar el buffer para los envíos en MPI
  MPI_Finalize();			// Cierra el universo MPI
  return MPI_SUCCESS;
}
