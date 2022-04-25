#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_TARGET_OPENCL_VERSION 120

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <CL/cl.h>

void CifradoOpenCL (cl_device_id device);
const char *getErrorString (cl_int error);

int main()
{
	int i, j;
	cl_platform_id platform_ids[16];
	cl_device_id device_ids[16];
	int platform_count;
	int device_count;

	// Esta enumeración es idéntica a la del primer ejercicio, salvo que no nos
	// hemos entretenido en obtener información sobre cada plataforma o dispositivo
	// por lo que hay menos código.
	clGetPlatformIDs(0, NULL, &platform_count);
	clGetPlatformIDs(platform_count, platform_ids, NULL);

	for (i = 0; i < platform_count; i++)  // enumeramos plataformas
	{
		cl_platform_id platform_id = platform_ids[i];

		device_count = 0;
		clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &device_count);
		clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, device_count, device_ids, NULL);

		for (j = 0; j < device_count; j++)  // enumeramos dispositivos dentro de cada plataforma
		{
			printf("Plataforma: %d,  dispositivo %d\n", i, j);
			CifradoOpenCL (device_ids[j]);  // vamos a usar este dispositivo para ejecutar nuestro kérnel
		}
	}
	return 0;
}

void CifradoOpenCL(cl_device_id device)
{
	cl_int ret;
	size_t global_work_size = 1024;  // cuantos work items tiene el grupo global de trabajo
	size_t local_work_size = 64;     // cuantos work items tiene cada grupo local de trabajo que vamos a enviar a OpenCL (debe ser divisor del primero)

	// Construir contexto para OpenCL
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &ret);

	// Establecer una via de comunicacion (cola de comandos) con el dispositivo
	cl_command_queue command_queue = clCreateCommandQueue(context, device, 0, &ret);

	// Código fuente de las funciones que queremos que se ejecuten en el dispositivo
	// En este caso, usamos un puntero a cadena que inicializamos en la misma declaración
	const char *program_code = ""
		"kernel void cifrado (global char src[], global char dst[], int lcadena)\n"
		"{                                                                      \n"
		"	size_t i = get_global_id(0);                                        \n"
		"                                                                       \n"
		"   // rellena el resto de este codigo                                  \n"
		"                                                                       \n"
		"                                                                       \n"
		"                                                                       \n"
		"                                                                       \n"
		"                                                                       \n"
		"                                                                       \n"
		"                                                                       \n"
		"                                                                       \n"
		"}                                                                      \n"
		;
	// Compilar el programa que irá en el kérnel
	cl_program program = clCreateProgramWithSource(context, 1, &program_code, NULL, NULL);
	ret = clBuildProgram(program, 1, &device, "", NULL, NULL);
	//                                        ^^
	//                                         +---- en esta cadena vacía podemos poner flags para el compilador de CL si nos hiciera falta
	if (ret)  // si la compilación dio algún error, recogerlo y mostrarlo en pantalla
	{
		char compiler_log[4096];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof compiler_log, compiler_log, NULL);
		printf("La compilacion de OpenCL dio error:\n%s", compiler_log);
		return;
	}
	
	// Creamos el objeto kernel e indicamos qué función de las compiladas en "program" se va a usar
	cl_kernel kernel = clCreateKernel(program, "cifrado", &ret);

	// Preparar la cadena de entrada y la de salida
	char cadena_origen[] = "Hvwr#hv#RshqFO=#fdgd#fdudfwhu#gh#hvwd#fdghqd#kd#vlgr#ghvfliudgr#sru#xq#hohphqwr#gh#surfhvdplhqwr#gh#xqd#FSX/#JSX#r#wdumhwd#dfhohudgrud1";
	char cadena_destino[1024];  // suficiente espacio en el destino como para albergar la cadena descifrada.
	int lcadena = (int)strlen(cadena_origen);

	// Crear buffers para la GPU
	cl_mem buffer_origen = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, lcadena, cadena_origen, &ret);
	cl_mem buffer_destino = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, lcadena, cadena_destino, &ret);

	// Establecer argumentos del kernel
	clSetKernelArg(kernel, 0, sizeof buffer_origen, &buffer_origen);
	clSetKernelArg(kernel, 1, sizeof buffer_destino, &buffer_destino);
	clSetKernelArg(kernel, 2, sizeof lcadena, &lcadena);
	
    // Ejecutar el kernel
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
	// If the buffer object is created with CL_MEM_USE_HOST_PTR set in mem_flags, the host_ptr specified in clCreateBuffer 
	// is guaranteed to contain the latest bits in the region being mapped when the clEnqueueMapBuffer command has 
	// completed; and the pointer value returned by clEnqueueMapBuffer will be derived from the host_ptr specified when the buffer object is created.
	char* result = clEnqueueMapBuffer(command_queue, buffer_destino, CL_TRUE, CL_MAP_READ, 0, lcadena, 0, NULL, NULL, &ret);
	clEnqueueUnmapMemObject(command_queue, buffer_destino, result, 0, NULL, NULL);

	if (ret)
		printf("  ERROR: %s\n\n", getErrorString(ret));
	else
	{
		printf("Cadena descifrada:\n%s\n\n", cadena_destino);
	}

	// Limpiar todo lo que hemos usado de OpenCL
	clReleaseMemObject(buffer_origen);
	clReleaseMemObject(buffer_destino);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);
}

const char *getErrorString(cl_int error)
{
	switch (error) {
		// run-time and JIT compiler errors
	case 0: return "CL_SUCCESS";
	case -1: return "CL_DEVICE_NOT_FOUND";
	case -2: return "CL_DEVICE_NOT_AVAILABLE";
	case -3: return "CL_COMPILER_NOT_AVAILABLE";
	case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case -5: return "CL_OUT_OF_RESOURCES";
	case -6: return "CL_OUT_OF_HOST_MEMORY";
	case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case -8: return "CL_MEM_COPY_OVERLAP";
	case -9: return "CL_IMAGE_FORMAT_MISMATCH";
	case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case -11: return "CL_BUILD_PROGRAM_FAILURE";
	case -12: return "CL_MAP_FAILURE";
	case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case -15: return "CL_COMPILE_PROGRAM_FAILURE";
	case -16: return "CL_LINKER_NOT_AVAILABLE";
	case -17: return "CL_LINK_PROGRAM_FAILURE";
	case -18: return "CL_DEVICE_PARTITION_FAILED";
	case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

		// compile-time errors
	case -30: return "CL_INVALID_VALUE";
	case -31: return "CL_INVALID_DEVICE_TYPE";
	case -32: return "CL_INVALID_PLATFORM";
	case -33: return "CL_INVALID_DEVICE";
	case -34: return "CL_INVALID_CONTEXT";
	case -35: return "CL_INVALID_QUEUE_PROPERTIES";
	case -36: return "CL_INVALID_COMMAND_QUEUE";
	case -37: return "CL_INVALID_HOST_PTR";
	case -38: return "CL_INVALID_MEM_OBJECT";
	case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case -40: return "CL_INVALID_IMAGE_SIZE";
	case -41: return "CL_INVALID_SAMPLER";
	case -42: return "CL_INVALID_BINARY";
	case -43: return "CL_INVALID_BUILD_OPTIONS";
	case -44: return "CL_INVALID_PROGRAM";
	case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
	case -46: return "CL_INVALID_KERNEL_NAME";
	case -47: return "CL_INVALID_KERNEL_DEFINITION";
	case -48: return "CL_INVALID_KERNEL";
	case -49: return "CL_INVALID_ARG_INDEX";
	case -50: return "CL_INVALID_ARG_VALUE";
	case -51: return "CL_INVALID_ARG_SIZE";
	case -52: return "CL_INVALID_KERNEL_ARGS";
	case -53: return "CL_INVALID_WORK_DIMENSION";
	case -54: return "CL_INVALID_WORK_GROUP_SIZE";
	case -55: return "CL_INVALID_WORK_ITEM_SIZE";
	case -56: return "CL_INVALID_GLOBAL_OFFSET";
	case -57: return "CL_INVALID_EVENT_WAIT_LIST";
	case -58: return "CL_INVALID_EVENT";
	case -59: return "CL_INVALID_OPERATION";
	case -60: return "CL_INVALID_GL_OBJECT";
	case -61: return "CL_INVALID_BUFFER_SIZE";
	case -62: return "CL_INVALID_MIP_LEVEL";
	case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
	case -64: return "CL_INVALID_PROPERTY";
	case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
	case -66: return "CL_INVALID_COMPILER_OPTIONS";
	case -67: return "CL_INVALID_LINKER_OPTIONS";
	case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

		// extension errors
	case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
	case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
	case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
	case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
	case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
	default: return "Unknown OpenCL error";
	}
}