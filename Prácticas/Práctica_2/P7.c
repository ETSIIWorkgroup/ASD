#define VECTOR_SIZE (1024*1024)
double a=3, x[VECTOR_SIZE*10],y[VECTOR_SIZE*10];
…
// Assume that vectors are initialized
…

int Daxpy_no_opt(void){
	int i;
	for(i=0;i<VECTOR_SIZE;i++)
		x[i]=a*x[i]+y[i];
	for(i=VECTOR_SIZE;i<VECTOR_SIZE*10;i++)
		x[i]=a*x[i]+y[i];
	return i;
}

int Daxpy_opt_10(void){
	int i;
	for (i=0;i<VECTOR_SIZE;i+=4){
		x[i]=a*x[i]+y[i];
		x[i+1]=a*x[i+1]+y[i+1];
		x[i+2]=a*x[i+2]+y[i+2];
		x[i+3]=a*x[i+3]+y[i+3];
	}
	for (i=VECTOR_SIZE;i<VECTOR_SIZE*10;i++)
		x[i]=a*x[i]+y[i];
	return i;
}

int Daxpy_opt_90(void){
	int i;
	for (i=0;i<VECTOR_SIZE;i++)
		
	for (i=VECTOR_SIZE;i<VECTOR_SIZE*10;i+=4){
		x[i]=a*x[i]+y[i];
		x[i+1]=a*x[i+1]+y[i+1];
		x[i+2]=a*x[i+2]+y[i+2];
		x[i+3]=a*x[i+3]+y[i+3];
	}
	return i;
}
