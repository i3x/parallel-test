/* Integration using Rectangle rule */
/* Tyler Simon */

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
   int a,b,n;

  if (argc!=4){
	 printf("Usage:%s <a> <b> <num_intervals>\n", argv[0]);
	 exit(0);
	 }

   a=atoi(argv[1]);	
   b=atoi(argv[2]);
   n=atoi(argv[3]);
	
   integral=rectangle_integrate(a,b,n,f);
   printf("The value of the integral is: %f \n",integral);
   return 0;
}
