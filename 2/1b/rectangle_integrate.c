/* Integration using Rectangle rule */
/* Tyler Simon */

#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* function of interest */
double f(double x){
   return pow(sin(2*x),4);
}
 
double rectangle_integrate(double a, double b, int subintervals, double (*function)(double) ){
   double result;
   double interval;
   int i;
 
   interval=(b-a)/subintervals;
   result=0;
   for(i=1;i<=subintervals;i++){
      result+=function(a+interval*(i-0.5));
   }
   result*=interval;
   return result;
}
 
int main(int argc, char **argv){
   double integral;
   int a,b,n,myid,numprocs;

  if (argc!=4){
	 printf("Usage:%s <a> <b> <num_intervals>\n", argv[0]);
	 exit(0);
	 }

   a=atoi(argv[1]);	
   b=atoi(argv[2]);
   n=atoi(argv[3]);

	
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);


   int i = 0;
   double diff = (b-a)/(double)numprocs;
   
   integral=rectangle_integrate(a+myid*diff,b+(myid+1)*diff,n/diff,f);

   if(myid == 0){
   	for(i=1;i<numprocs;i++){
		double temp = 0;
		MPI_Recv(&temp, 1, MPI_DOUBLE, i , 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		integral = integral + temp;
	}
    }
    else{
	MPI_Send(&integral,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }
   printf("The value of the integral is: %f \n",integral);
   return 0;
}
