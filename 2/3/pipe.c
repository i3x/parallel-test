#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include<stdlib.h>
#include <string.h>

#define PIPELINE 0
#define POWER 16

int main(int argc, char **argv)
{
	int myid, numprocs;
	long long int data[2];
	long long int mult = atoll(argv[1]);
	data[0] = mult;
	data[1] = 1;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

//pipeline method
if(PIPELINE == 1){
	int i;
	for(i = 0; i <= POWER/numprocs; i++){
	//start ball rolling
	if (myid == 0) {
		if(i > 0){
		//not the first time around, wait for response.
		    MPI_Recv(data, 2, MPI_LONG_LONG_INT, numprocs, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
		data[0] = data[0] * mult;
		data[1] = data[1] + 1;
		//send data to each processor
		MPI_Send(data, 2, MPI_LONG_LONG_INT, (myid+1)%numprocs, 0, MPI_COMM_WORLD);
	}
	else{
		MPI_Recv(data, 2, MPI_LONG_LONG_INT, myid-1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		data[0] = data[0] * mult;
		data[1]= data[1] +1;
		if(data[1] == POWER){
			printf("The total is %d after %d iterations.\n", data[0], data[1]);
		}	
		MPI_Send(data, 2, MPI_LONG_LONG_INT, (myid+1)%numprocs, 0, MPI_COMM_WORLD);
	}
	}
}
//divide and conquer method.
else{
	if(myid == 0){
		MPI_Bcast(data, 2, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
	}
	
	int mid = numprocs/2;
	
	if(myid < mid){
		data[0] = data[0] * data[0];
		MPI_Send(data, 2, MPI_LONG_LONG_INT, (myid+mid), 0, MPI_COMM_WORLD);
	}
	else{
		
		MPI_Recv(data, 2, MPI_LONG_LONG_INT, myid-mid, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	int mid2 = (numprocs-mid)/2;
	if(myid >= mid && myid < mid+mid2){
		data[0] = data[0] * data[0];
		MPI_Send(data, 2, MPI_LONG_LONG_INT, (myid+mid2), 0, MPI_COMM_WORLD);
	}
	else if(myid >= mid){
		
		MPI_Recv(data, 2, MPI_LONG_LONG_INT, myid-mid2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}	

	MPI_Barrier(MPI_COMM_WORLD);
	int mid3 = (numprocs-mid-mid2)/2;
	if(myid >= mid + mid2 && myid < mid+mid2+mid3){
		data[0] = data[0] * data[0];
		MPI_Send(data, 2, MPI_LONG_LONG_INT, (myid+mid3), 0, MPI_COMM_WORLD);
	}
	else if(myid >= mid + mid2){
		MPI_Recv(data, 2, MPI_LONG_LONG_INT, myid-mid3, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}

	if(myid == numprocs - 1){
	 printf("The total is %d after %d iterations.\n", data[0]*data[0], POWER);	
}	}
MPI_Finalize();	
return 0;
}


