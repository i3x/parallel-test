#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include <string.h>

#define MAXSIZE 1000

int main(int argc, char **argv)
{
	int myid, numprocs;
	int data[MAXSIZE];
	int i, x, low, high, myresult=0, result=0, mybest=0, sum=0,best=0;
	char fn[255];
	FILE *fp;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	if (myid == 0) {  /* Open input file and initialize data */
		strcpy(fn, "rand_data.txt");
		if ((fp = fopen(fn,"r")) == NULL) {
			printf("Cannot open the input file: %s\n\n", fn);
			exit(1);
		}

	for(i = 0; i < MAXSIZE; i++){
		 fscanf(fp,"%d", &data[i]);
		}
	
	for(i=1; i<numprocs;i++){
		//send data to each processor
		MPI_Send(data, MAXSIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
	       }
	
	}

	else{
//MPI_Bcast(data, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD); /* broadcast data */ 
	MPI_Recv(data, MAXSIZE, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}

	x = MAXSIZE/numprocs; /* Add my portion of data */
	low = myid * x;
	high = low + x;
	for(i = low; i < high; i++){
		 myresult += data[i];
		 if(data[i] > mybest){
			mybest = data[i];
		}
	}

	printf("partial sum %d and best %d from %d\n", myresult, mybest,  myid); /* Compute global sum */

//	MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (myid == 0){
		int tempsum = 0;
		int tempbest = mybest;

		sum+= myresult;
		for(i=1; i<numprocs;i++){
			//send data to each processor
			MPI_Recv(&tempsum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += tempsum;
		}
		
		best = tempbest;
		for(i=1; i<numprocs;i++){
			//send data to each processor
			MPI_Recv(&tempbest, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(tempbest > best){
				best = tempbest;
			}
		}

	}
	else{
		MPI_Send(&myresult,1, MPI_INT,0,0,MPI_COMM_WORLD);
		MPI_Send(&mybest,1,MPI_INT,0,1,MPI_COMM_WORLD);
	}

	//Print this out last so it doesn't get mixed with the output from the other procs.
	if(myid == 0){

		printf("The sum is %d.\n", sum);
		printf("The highest value is %d.\n",best);
		
	}
	MPI_Finalize();

return 0;
}


