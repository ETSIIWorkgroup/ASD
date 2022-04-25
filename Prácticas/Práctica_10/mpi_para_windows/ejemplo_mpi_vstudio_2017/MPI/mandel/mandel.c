/*
Ejemplo de c�lculo del conjunto de Mandelbrot usando MPI

(c)2020 Miguel Angel Rodriguez Jodar. Dept. ATC.

Para ver el resultado (Windows), cargar el fichero mandel.raw con
el programa Irfanview (es gratuito: se necesita tener el programa
y los plugins extra). En el cuadro de di�logo de apertura del fichero
indicar que width es 1024, height es 768, y que la imagen es de 8BPP.
El resto de par�metros no se tocan.
Si se cambia ANCHO o ALTO, cambiar tambi�n width o height en Irfanview
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define ANCHO 1024   // Resolucion que usaremos en la
#define ALTO 768     // imagen final

void Maestro (int nprocs);
void Esclavo (int rank, int nprocs);
void CalculaMandel (unsigned char *bitmap, float xmin, float xmax, float ymin, float ymax, int desde, int hasta);

int main (int argc, char *argv[])
{
  int rank, nprocs;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // quien soy yo en el universo MPI
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs); // cuantos procesos hay

  if (rank == 0)
    Maestro(nprocs);                      // el maestro (root) ser� el rank 0
  else
    Esclavo(rank, nprocs);                // el resto, esclavos.

  MPI_Finalize();
  return MPI_SUCCESS;
}

void Maestro (int nprocs)
{
  float xmin, xmax, ymin, ymax;
  unsigned desde, hasta, altura_trozo;
  unsigned char *bitmap;
  int i;
  double tstart, tend;
  FILE *f;

  bitmap = malloc (ANCHO*ALTO*sizeof *bitmap);   // creo espacio en memoria para el bitmap

  xmin = -2.4f;          //
  xmax = 0.8f;           // Parametros iniciales del
  ymin = -1.2f;          // conjunto de Mandelbrot
  ymax = 1.2f;           // (porci�n del plano a usar)

  MPI_Barrier (MPI_COMM_WORLD);  // esperamos a que todos estemos listos, para....
  tstart = MPI_Wtime();          // ...empezar a contar el tiempo!

  for (i=1; i<nprocs; i++)  // equivalente a usar MPI_Bcast sin el bucle
  {
    MPI_Send (&xmin, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);  //
    MPI_Send (&xmax, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);  // Env�o de los par�metros iniciales
    MPI_Send (&ymin, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);  // a cada esclavo. MEJORA: usar MPI_Bcast
    MPI_Send (&ymax, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);  //
  }

  altura_trozo = ALTO/nprocs;   // cada proceso generar� un cacho del bitmap de "altura_trozo" lineas de alto
  desde = 0;                    // El maestro calcula el primer cacho
  hasta = altura_trozo-1;       //
  CalculaMandel (bitmap+desde*ANCHO, xmin, xmax, ymin, ymax, desde, hasta);
  if (ALTO%nprocs != 0)         // si la altura del bitmap no es divisible entre el numero de procesos...
  {
    desde = nprocs*altura_trozo;     // el maestro tambi�n se encargar� de calcular
    hasta = ALTO-1;                  // el restillo de lineas que quedasen sin calcular
    CalculaMandel (bitmap+desde*ANCHO, xmin, xmax, ymin, ymax, desde, hasta);
  }

  for (i=1; i<nprocs; i++)  // MEJORA: usar MPI_Gather y quitar el bucle. Recojo los "cachos" de bitmap de cada esclavo.
  {
    desde = i*altura_trozo;
    MPI_Recv (bitmap+desde*ANCHO, altura_trozo*ANCHO, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  MPI_Barrier (MPI_COMM_WORLD);  // barrera para que ning�n esclavo libere su buffer de
                                 // memoria hasta que el maestro no lo haya recibido
  tend = MPI_Wtime();            // Fin de la medida del tiempo

  printf ("Tiempo: %d ms\n", (int)((tend-tstart)*1000));  // lo muestro en milisegundos

  f = fopen ("mandel.raw", "wb");     //
  fwrite (bitmap, 1, ANCHO*ALTO, f);  // Grabo el contenido del bloque "bitmap" en un fichero, formato RAW
  fclose (f);                         // para Irfanview, o cualquier programa capaz de abrir ficheros gr�ficos sin formato

  free (bitmap);                      // libero el espacio de memoria que asign� para el bitmap inicialmente

  return;
}

void Esclavo (int rank, int nprocs)
{
  float xmin, xmax, ymin, ymax;
  unsigned desde, hasta, altura_trozo;
  unsigned char *bitmap;

  MPI_Barrier (MPI_COMM_WORLD);       // aviso al maesto de que estoy listo

  // Aqui tambi�n se usar�a MPI_Bcast
  MPI_Recv (&xmin, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Recv (&xmax, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Recv (&ymin, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Recv (&ymax, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  altura_trozo = ALTO/nprocs;        // calculo desde y hasta qu� linea debo calcular
  desde = rank*altura_trozo;
  hasta = (rank+1)*altura_trozo-1;
  bitmap = malloc (altura_trozo*ANCHO*sizeof *bitmap);  // creo un minibitmap donde guardar� mi trozo de bitmap calculado

  CalculaMandel (bitmap, xmin, xmax, ymin, ymax, desde, hasta);
  MPI_Send (bitmap, altura_trozo*ANCHO, MPI_CHAR, 0, 0, MPI_COMM_WORLD);  // env�o mi resultado al maestro. MEJORA: usar MPI_Gather
  MPI_Barrier (MPI_COMM_WORLD);       // me espero a que el maestro me diga que ya ha recogido todos los trozos
  free (bitmap);                      // y entonces s�, libero la memoria para mi minibitmap
}

/* Esta funci�n calcula, de forma "novata" un valor de 0 a 255 por cada pixel que le corresponde a un trozo de bitmap
   que va desde "desde" hasta "hasta" lineas. Las dimensiones del plano complejo usado est�n en xmin, ymin, xmax, ymax
   y se corresponden con los valores m�nimo y m�ximo para las componentes real e imaginaria del n�mero complejo C.

   El bucle m�s interno va calculando los t�rminos de la sucesi�n Zi+1 = Zi^2 + C, donde Z0 = 0+0i
   Si la sucesi�n diverge, se para el c�lculo y se devuelve el n�mero de iteraciones que ha tardado en diverger.
   Si la sucesi�n llega a 255 iteraciones y no diverge, se devuelve 255.
   Para que el dibujo quede m�s vistoso, en lugar de devolver el valor tal cual, se devuelve su logaritmo escalado
*/
void CalculaMandel (unsigned char *bitmap, float xmin, float xmax, float ymin, float ymax, int desde, int hasta)
{
  int x, y;
  float zr, zi, cr, ci, tr, ti;
  int niter;
  int i = 0;

  for (y=desde; y<=hasta; y++)
  {
    for (x=0; x<ANCHO; x++)
    {
      cr = xmin + x*1.0f*(xmax-xmin)/(ANCHO-1);  // Valor de C
      ci = ymin + y*1.0f*(ymax-ymin)/(ALTO-1);   //
      zr = zi = 0;                              // Valor inicial de Z
      niter = 0;
      while (niter < 255 && zr*zr+zi*zi<4.0f)  // Suponemos que Z diverge si su norma es mayor de 4
      {
        tr = zr*zr - zi*zi + cr;     // Calculo Z^2 + C
        ti = 2.0f*zr*zi + ci;           // en un numero complejo T (temporal)
        zr = tr;                     // Paso el nuevo valor al
        zi = ti;                     // complejo Z para una nueva iteracion
        niter++;
      }
      bitmap[i++] = (int)(log10(niter)*255/log10(255));   // guardamos en el minibitmap el logaritmo del numero de iteraciones
    }
  }
}
