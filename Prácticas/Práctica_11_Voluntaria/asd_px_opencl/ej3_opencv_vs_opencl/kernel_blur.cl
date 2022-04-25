kernel void blurring (int height, int width, int step, global uchar src[], global uchar dst[])
{
  size_t y = get_global_id(0);
  size_t x = get_global_id(1);

  // Si el work-item no se corresponde a un pixel de la imagen, salir y no hacer nada
  if (x >= width || y >= height)
    return;

  // caso límite: no procesamos los pixeles de los bordes. Tan solo los copiamos
  if (x==0 || y==0 || x==width-1 || y==height-1)
  {
    dst[y*step+x*3+0] = src[y*step+x*3+0];
    dst[y*step+x*3+1] = src[y*step+x*3+2];
    dst[y*step+x*3+2] = src[y*step+x*3+1];
  }
  // caso general. Aquí es donde hacemos el blurring.
  else
  {
    global uchar *ptrsrc = src + y*step;  // puntero a la fila de pixeles de la imagen origen
	global uchar *ptrdst = dst + y*step;  // puntero a la fila de pixeles de la imagen destino

	// Copia de la practica 1, apendice 4, el triple bucle que
	// procesa la componente de color de cada pixel.
	// Los cambios son que la imagen se lee desde ptrsrc en lugar de desde ptr
	// y que el pixel promediado se escribe a ptrdst en lugar de a ptr.
	// El valor de kernel_radius es 1. La anchura y altura en pixeles de la
	// imagen se encuentra en los parámetros width y height respectivamente
	// El número de vecinos es 9.
    for (int color=0; color<3; color++)
    {
	  // .... ETC ......
    }
  }
}
