# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <mpi.h>
# include<stdlib.h>
# include <string.h>

int main ( int argc, char *argv[] );
double cpu_time ( void );

/******************************************************************************/

int main ( int argc, char *argv[] )

/******************************************************************************/
/*
  Purpose:

    MAIN is the main program for HEATED_PLATE.

  Discussion:

    This code solves the steady state heat equation on a rectangular region.

    The sequential version of this program needs approximately
    18/epsilon iterations to complete. 


    The physical region, and the boundary conditions, are suggested
    by this diagram;

                   W = 0
             +------------------+
             |                  |
    W = 100  |                  | W = 100
             |                  |
             +------------------+
                   W = 100

    The region is covered with a grid of M by N nodes, and an N by N
    array W is used to record the temperature.  The correspondence between
    array indices and locations in the region is suggested by giving the
    indices of the four corners:

                  I = 0
          [0][0]-------------[0][N-1]
             |                  |
      J = 0  |                  |  J = N-1
             |                  |
        [M-1][0]-----------[M-1][N-1]
                  I = M-1

    The steady state solution to the discrete heat equation satisfies the
    following condition at an interior grid point:

      W[Central] = (1/4) * ( W[North] + W[South] + W[East] + W[West] )

    where "Central" is the index of the grid point, "North" is the index
    of its immediate neighbor to the "north", and so on.
   
    Given an approximate solution of the steady state heat equation, a
    "better" solution is given by replacing each interior point by the
    average of its 4 neighbors - in other words, by using the condition
    as an ASSIGNMENT statement:

      W[Central]  <=  (1/4) * ( W[North] + W[South] + W[East] + W[West] )

    If this process is repeated often enough, the difference between successive 
    estimates of the solution will go to zero.

    This program carries out such an iteration, using a tolerance specified by
    the user, and writes the final estimate of the solution to a file that can
    be used for graphic processing.

  Parameters:

    Commandline argument 1, double EPSILON, the error tolerance.  

    Commandline argument 2, char *OUTPUT_FILE, the name of the file into which
    the steady state solution is written when the program has completed.

  Local parameters:

    Local, double DIFF, the norm of the change in the solution from one iteration
    to the next.

    Local, double MEAN, the average of the boundary values, used to initialize
    the values of the solution in the interior.

    Local, double U[M][N], the solution at the previous iteration.

    Local, double W[M][N], the solution computed at the latest iteration.
*/
{
# define M 500
# define N 500

  double ctime;
  double ctime1;
  double ctime2;
  double diff;
  double epsilon;
  FILE *fp;
  int i;
  int iterations;
  int iterations_print;
  int j;
  double mean;
  char output_file[80];
  int success;
  double u[M][N];
  double w[M][N];
  int myid,numprocs;
  double min = 1000;


  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  double g[N/numprocs + 2][M];
  double h[N/numprocs + 2][M];


if(myid == 0){
  printf ( "\n" );
  printf ( "HEATED_PLATE\n" );
  printf ( "  A program to solve for the steady state temperature distribution\n" );
  printf ( "  over a rectangular plate.\n" );
  printf ( "\n" );
  printf ( "  Spatial grid of %d by %d points.\n", M, N );
/* 
  Read EPSILON from the command line or the user.
*/
  if ( argc < 2 ) 
  {
    printf ( " %s EPSILON, the error tolerance:\n",argv[0] );
	exit(0);
  }
  else
  {
	epsilon=atof(argv[1]);
	success=1;
  }

  if ( success != 1 )
  {
    printf ( "\n" );
    printf ( "HEATED_PLATE\n" );
    printf ( "  Error reading in the value of EPSILON.\n");
    return 1;
  }

  printf ( "\n" );
  printf ( "  The iteration will be repeated until the change is <= %f\n", epsilon );
  diff = epsilon;
/* 
  Read OUTPUT_FILE from the command line or the user.
*/
  if ( argc < 3 ) 
  {
    printf ( "\n" );
    printf ( "  Enter OUTPUT_FILE, the name of the output file:\n" );
    success = scanf ( "%s", output_file );
  }
  else
  {
    success = sscanf ( argv[2], "%s", output_file );
  }

  if ( success != 1 )
  {
    printf ( "\n" );
    printf ( "HEATED_PLATE\n" );
    printf ( "  Error reading in the value of OUTPUT_FILE.\n");
    return 1;
  }

  printf ( "\n" );
  printf ( "  The steady state solution will be written to %s.\n", output_file );
/* 
  Set the boundary values, which don't change. 
*/
  for ( i = 1; i < M - 1; i++ )
  {
    w[i][0] = 100.0;
  }
  for ( i = 1; i < M - 1; i++ )
  {
    w[i][N-1] = 100.0;
  }
  for ( j = 0; j < N; j++ )
  {
    w[M-1][j] = 100.0;
  }
  for ( j = 0; j < N; j++ )
  {
    w[0][j] = 0.0;
  }
/*
  Average the boundary values, to come up with a reasonable
  initial value for the interior.
*/
  mean = 0.0;
  for ( i = 1; i < M - 1; i++ )
  {
    mean = mean + w[i][0];
  }
  for ( i = 1; i < M - 1; i++ )
  {
    mean = mean + w[i][N-1];
  }
  for ( j = 0; j < N; j++ )
  {
    mean = mean + w[M-1][j];
  }
  for ( j = 0; j < N; j++ )
  {
    mean = mean + w[0][j];
  }
  mean = mean / ( double ) ( 2 * M + 2 * N - 4 );
/* 
  Initialize the interior solution to the mean value.
*/
  for ( i = 1; i < M - 1; i++ )
  {
    for ( j = 1; j < N - 1; j++ )
    {
      w[i][j] = mean;
    }
  }

  for(i=0;i < numprocs;i++){
  	MPI_Send(&w[i * N/numprocs][0], M * N/numprocs , MPI_FLOAT, i, 0, MPI_COMM_WORLD);
	printf("proc 0 sent to id %d\n",i);
  }
  
}
printf("process: %d receiving and beginning memcpy\n",myid);
MPI_Recv(h[1], M*N/numprocs, MPI_FLOAT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
memcpy(h[0],h[1], M * sizeof(float));
memcpy(h[N/numprocs+2],h[N/numprocs+1], M * sizeof(float));

printf("process: %d, after memcpy\n", myid);

MPI_Barrier(MPI_COMM_WORLD);
printf("myid: %d, values %f,%f,%f,%f\n",myid,h[0][0],h[1][0],h[2][0],h[3][0]);

//START PARALLEL
//double g[N/numprocs + 2][M];
//double h[N/numprocs + 2][M];
while ( epsilon <= min){
diff = 0.0;

int k = 1;
for (k = 1; k <= N/numprocs; k++){
	for(j = 0; j <M; j++){
		g[k][j] = 0.25 * (h[k-1][j] + h[k+1][j] + h[k][j-1] + h[k][j+1]);
		if(diff < fabs(g[k][j] - h[k][j])){
			diff = fabs(g[k][j] - h[k][j]);	
		}
	}

}

for (k=1; k < N/numprocs; k++){
	for(j = 1 ; j <= M;j++){
	h[k][j] = g[k][j];
	}
}

//send INNER top and bottom rows to neighbors.
send(&g[1][1], M, myid -1);
send(&g[N/numprocs][1], M, myid + 1);
send(&diff, 1, 0); 
//recv rows from neighbors and store in OUTER (ghost) rows.
recv(&h[0][1], M, myid -1);
recv(&h[N/numprocs+1][1], M, myid + 1);

if(myid == 0){
	float temp = 0.0;
	for(i = 0; i < numprocs; i++){
		recv(&temp,1,i);
		if(temp < diff){
			diff = temp;
		}
	}
	
	for(i = 0; i < numprocs; i++){
		printf("sending best diff of %f min: %f and ep: %f \n",diff, min, epsilon);
		send(&diff, 1, i);
	}
}

recv(&min, 1, 0);

}


if(myid == 0){
/* 
  Write the solution to the output file.
*/
  fp = fopen ( output_file, "w" );

  fprintf ( fp, "%d\n", M );
  fprintf ( fp, "%d\n", N );

  for ( i = 0; i < M; i++ )
  {
    for ( j = 0; j < N; j++)
    {
      fprintf ( fp, "%6.2f ", w[i][j] );
    }
    fputc ( '\n', fp);
  }
  fclose ( fp );

  printf ( "\n" );
  printf ("  Solution written to the output file %s\n", output_file );
/* 
  All done! 
*/
  printf ( "\n" );
  printf ( "HEATED_PLATE:\n" );
  printf ( "  Normal end of execution.\n" );

}
MPI_Finalize();
return 0;
//END PARALLEL

# undef M
# undef N
}
/******************************************************************************/

double cpu_time ( void )

/******************************************************************************/
/*
  Purpose:
    CPU_TIME returns the current reading on the CPU clock.
    Output, double CPU_TIME, the current reading of the CPU clock, in seconds.
*/
{
  double value;
  value = ( double ) clock ( ) / ( double ) CLOCKS_PER_SEC;
  return value;
}
