#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <string.h>
#include <CL/cl.h>

int main()
{
	int i, j;
	cl_platform_id platform_ids[16];  // aqui guardaremos los IDs de las plataformas encontradas en este host
	cl_device_id device_ids[16];      // aqui guardaremos los IDs de los dispositivos encontrados para la plataforma actual
	char cadena[256];                 // una cadena, para usarla cuando nos haga falta
	int platform_count;               // numero de entradas validas en el array platform_ids
	int device_count;                 // numero de entradas validas en el array device_ids
	cl_uint max_units;                //
	size_t max_work_item_sizes[3];    // Variables para guardar ciertos parametros del dispositivo sobre los que preguntaremos a OpenCL
	size_t max_work_group_size;       //

	clGetPlatformIDs(0, NULL, &platform_count);  // cuantas plataformas hay en este host?
	clGetPlatformIDs(platform_count, platform_ids, NULL);  // dame la lista de todas las plataformas de este host

	for (i = 0; i < platform_count; i++)  // para cada plataforma, hacer...
	{
		clGetPlatformInfo(platform_ids[i], CL_PLATFORM_NAME, sizeof cadena, cadena, NULL);  // cual es el nombre de esta plataforma?
		printf("\nPlataforma: %s\n", cadena);

		device_count = 0;
		clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &device_count);  // para esta plataforma, dime cuantos dispositivos tiene
		clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL , device_count, device_ids, NULL);  // dame una lista con todos esos dispositivos

		for (j = 0; j < device_count; j++)  // para cada dispositivo de la actual plataforma, hacer...
		{
			// Usando la función clGetDeviceInfo, obten la información sobre el nombre de dispositivo, el número de unidades de computación,
			// el tamaño máximo de workgroup, y los tamaños máximos de workgroup para las tres dimensiones.
			// Como ejemplo puedes mirar cómo se ha usado clGetPlatformInfo más arriba, que funciona de forma muy similar.
			// salvo que ahora usamos device_ids[j] para referirnos a un ID de dispositivo


			printf("  Dispositivo: %-30.30s ", cadena);


			printf("%3u unidades de computacion.\n", max_units);


			printf("    Workgroup size: %-5zd  ", max_work_group_size);


			printf("Workgroup item sizes: %zd/%zd/%zd\n", max_work_item_sizes[0], max_work_item_sizes[1], max_work_item_sizes[2]);
		}
	}
	return 0;
}
