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

	int data = 0;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	//begin tree barrier.
	//First level.  If node is odd, send to node - 1, then wait.
	
	if(myid % 2 == 1){
		MPI_Send(&data, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD);
		MPI_Recv(&data, 1, MPI_INT, myid-1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	}
	//Not odd.
	else{	
		MPI_Recv(&data, 1, MPI_INT, myid+1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		//Now, collect to next level. (0 2 4 6)
		if(myid % 4 == 2){
			MPI_Send(&data, 1, MPI_INT, myid-2, 0, MPI_COMM_WORLD);
			MPI_Recv(&data, 1, MPI_INT, myid-2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		}
		//myid is 0 or 4, collect results
		else{
			MPI_Recv(&data, 1, MPI_INT, myid+2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			if(myid % 8 == 4){
				
				MPI_Send(&data, 1, MPI_INT, myid-4, 0, MPI_COMM_WORLD);
				MPI_Recv(&data, 1, MPI_INT, myid-4, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			}
			//id is 0.
			else{
				MPI_Recv(&data, 1, MPI_INT, myid+4, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				printf("Barrier arrival reached. Node zero has synchronized all nodes.\n");
				printf("Beginning departure.\n"); 
			}
		}
	}
	MPI_Finalize();
	return 0;
}
