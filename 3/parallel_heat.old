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

        if(myid == 0){
                printf("Tree barrier reached, entering tree code.\n");
        }


	MPI_Finalize();
	return 0;
}
