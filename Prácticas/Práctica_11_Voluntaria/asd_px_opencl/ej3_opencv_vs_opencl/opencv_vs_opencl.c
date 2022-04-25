#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_TARGET_OPENCL_VERSION 120
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <CL/cl.h>
#include <intrin.h>
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>

#define VECES 64

const char *getErrorString(cl_int error);
void BlurConOpenCV(CvMat *img);
void BlurConOpenCL(cl_device_id device, CvMat *img, char *windowtitle);

int main()
{
	int i, j;
	cl_platform_id platform_ids[16];
	cl_device_id device_ids[16];
	char cadena[256];
	int platform_count;
	int device_count;
	size_t lencadena;

	// objeto Mat de OpenCV que contiene la imagen original cargada
	CvMat *img;

	// Descomentar una de las cuatro llamadas a cvLoadImageM según la imagen que se quiere cargar
	//img = cvLoadImageM("spike.png", CV_LOAD_IMAGE_COLOR);
	img = cvLoadImageM("carta_ajuste.png", CV_LOAD_IMAGE_COLOR);
	//img = cvLoadImageM("monsters_inc.png", CV_LOAD_IMAGE_COLOR);
	//img = cvLoadImageM("textura_1024x1024.jpg", CV_LOAD_IMAGE_COLOR);

	// Mostramos la imagen original en una ventana
	cvNamedWindow("Imagen original", CV_WINDOW_AUTOSIZE);	
	cvShowImage("Imagen original", img);

	// Medimos tiempo que tarda OpenCV en hacerle un blurring a esta imagen
	BlurConOpenCV(img);

	// Enumeración OpenCL
	clGetPlatformIDs(0, NULL, &platform_count);
	clGetPlatformIDs(platform_count, platform_ids, &platform_count);

	for (i = 0; i < platform_count; i++)
	{
		cl_platform_id platform_id = platform_ids[i];
		clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, 256, cadena, NULL);
		printf("\nPlatform: %s\n", cadena);

		device_count = 0;
		clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 0, NULL, &device_count);
		clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, device_count, device_ids, &device_count);

		for (j = 0; j < device_count; j++)
		{
			printf("  Device %d: ", j);

			clGetDeviceInfo(device_ids[j], CL_DEVICE_NAME, 256, cadena, &lencadena);
			cadena[lencadena] = '\0';
			printf("%-30.30s : ", cadena);

			BlurConOpenCL(device_ids[j], img, cadena);
		}
	}

	cvWaitKey(0);
	cvReleaseMat(&img);
	return 0;
}

void BlurConOpenCV(CvMat *img)
{
	CvMat *origen, *destino;
	int i;
	// estas variables de 64 bits contienen el numero de ciclos medidos
	unsigned long long tstart, tstop, tminimo = 0xffffffffffffffff;

	// Hacemos una copia de la imagen pasada como parámetro, en origen
	origen = cvCloneMat(img);
	// Creamos una imagen vacía con las mismas dimensiones y profundidad de color que origen
	// En destino hay una imagen vacía donde se guardará el resultado del blurring.
	destino = cvCreateMat(origen->rows, origen->cols, CV_8UC3);

	// Repetimos varias veces la operación...
	for (i = 0; i < VECES; i++)
	{
		tstart = __rdtsc();  // comenzamos a medir el tiempo aquí
		/////////////////////////////////////////////////////////
		cvSmooth(origen, destino, CV_BLUR, 3, 3, 0, 0);
		/////////////////////////////////////////////////////////
		tstop = __rdtsc();   // terminamos de medir el tiempo aquí
		if (tstop - tstart < tminimo)  // nos quedamos con el tiempo mínimo hasta ahora
			tminimo = tstop - tstart;
	}

	// Mostramos la imagen con blurring hecha con OpenCV
	cvNamedWindow("Blur con OpenCV", CV_WINDOW_AUTOSIZE);
	cvShowImage("Blur con OpenCV", destino);

	// Y mostramos el tiempo mínimo calculado, por pixel.
	printf("OpenCV: %.6f ciclos/pixel\n", tminimo*1.0f / (origen->rows*origen->cols));

	// Descartamos origen y destino. La imagen original (img) no queda afectada
	cvReleaseMat(&origen);
	cvReleaseMat(&destino);
}


void BlurConOpenCL(cl_device_id device, CvMat *img, char *windowtitle)
{
	// estas variables de 64 bits contienen el numero de ciclos medidos
	unsigned long long tstart, tstop, tminimo = 0xffffffffffffffff;

	cl_int ret;
	int veces;
	CvMat *origen, *destino;
	size_t global_work_size[2], local_work_size[2];

	// Ahora nuestro grupo de trabajo global tiene dos dimensiones. Establecemos que como maximo
	// procesaremos imagenes de 1024x1024 pixeles
	global_work_size[0] = 1024;
	global_work_size[1] = 1024;

	// Al dispositivo enviaremos bloques de 1x1 pixeles (ouch! esto no está aprovechando el potencial de OpenCL)
	local_work_size[0] = 1;
	local_work_size[1] = 1;

	// Construir contexto para OpenCL
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &ret);
	cl_command_queue command_queue = clCreateCommandQueue(context, device, 0, &ret);

	// Cargar el codigo fuente del kernel en una cadena y enviarsela al compilador
	char *program_code = malloc(4096);
	FILE *f;
	f = fopen("kernel_blur.cl", "rb");
	size_t leido = fread(program_code, 1, 4096, f);
	fclose(f);
	program_code[leido] = '\0';

	cl_program program = clCreateProgramWithSource(context, 1, &program_code, NULL, NULL);
	ret = clBuildProgram(program, 1, &device, "", NULL, NULL);
	if (ret)
	{
		char compiler_log[4096];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof compiler_log, compiler_log, NULL);
		printf("La compilacion de OpenCL dio error:\n%s", compiler_log);
		return;
	}
	cl_kernel kernel = clCreateKernel(program, "blurring", &ret);

	// Preparar la imagen de entrada y el buffer de salida
	origen = cvCloneMat(img);  // clonamos la imagen original en origen
	destino = cvCreateMat(origen->rows, origen->cols, CV_8UC3);  // creamos en destino una imagen vacía

	// Crear buffers para la GPU
	cl_mem buffer_origen = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, origen->rows*origen->step, origen->data.ptr, &ret);
	cl_mem buffer_destino = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, destino->rows*destino->step, destino->data.ptr, &ret);

	// Establecer argumentos del kernel
	clSetKernelArg(kernel, 0, sizeof origen->rows, &origen->rows);
	clSetKernelArg(kernel, 1, sizeof origen->cols, &origen->cols);
	clSetKernelArg(kernel, 2, sizeof origen->step, &origen->step);
	clSetKernelArg(kernel, 3, sizeof buffer_origen, &buffer_origen);
	clSetKernelArg(kernel, 4, sizeof buffer_destino, &buffer_destino);

	for (veces = 0; veces < VECES; veces++)
	{
        // Ejecutar el kernel
		tstart = __rdtsc();  // comenzamos a medir el tiempo aquí
		////////////////////////////////////////////////////////////////////////////////
		ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, NULL);
		// If the buffer object is created with CL_MEM_USE_HOST_PTR set in mem_flags, the host_ptr specified in clCreateBuffer 
		// is guaranteed to contain the latest bits in the region being mapped when the clEnqueueMapBuffer command has 
		// completed; and the pointer value returned by clEnqueueMapBuffer will be derived from the host_ptr specified when the buffer object is created.
		char* puntero_a_datos = clEnqueueMapBuffer(command_queue, buffer_destino, CL_TRUE, CL_MAP_READ, 0, destino->rows*destino->step, 0, NULL, NULL, NULL);
		clEnqueueUnmapMemObject(command_queue, buffer_destino, puntero_a_datos, 0, NULL, NULL);
		////////////////////////////////////////////////////////////////////////////////
		tstop = __rdtsc();  // terminamos de medir el tiempo aquí

		if (tstop - tstart < tminimo)  // y nos quedamos con el tiempo mínimo
			tminimo = tstop - tstart;
		if (ret)
		{
			printf("  ERROR: %s\n", getErrorString(ret));
			break;
		}
	}

	if (!ret)
	{
		cvNamedWindow(windowtitle, CV_WINDOW_AUTOSIZE);
		cvShowImage(windowtitle, destino);

		printf("%.6f ciclos/pixel\n", tminimo*1.0f / (origen->rows*origen->cols));
	}

	// Recolección de basura
	clReleaseMemObject(buffer_origen);
	clReleaseMemObject(buffer_destino);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);
	free(program_code);

	cvReleaseMat(&destino);
	cvReleaseMat(&origen);
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