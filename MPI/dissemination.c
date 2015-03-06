#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <sys/time.h>

#define BARRIERS 1000

void dissemination_barrier(int number_processors, int rank)
{

int send_number, receive_number;
int message =1, tag=1;
int total_rounds = ceil(log(number_processors)/log(2));
MPI_Status stat;
int i;
for(i=0; i<total_rounds; i++){
	send_number = (rank + pow(2, i));
	send_number = send_number % number_processors;
	
	receive_number = (rank - pow(2, i) + number_processors);
	receive_number = receive_number %  number_processors;
	
	MPI_Send(&message, 1, MPI_INT, send_number, tag, MPI_COMM_WORLD);
	MPI_Recv(&message, 1, MPI_INT, receive_number, tag, MPI_COMM_WORLD, &stat);

}
}

int main(int argc, char** argv)
{
int number_processors, rank,i,k=0;

double time_final=0,time_begin,time_end;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &number_processors);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Barrier(MPI_COMM_WORLD);

for(i=0;i<BARRIERS;i++)
{
printf("\nBarrier %d: Rank of current processor is %d",i, rank);

 k=0;
		while(k<10000000)
			k++;
			
time_begin=MPI_Wtime();
dissemination_barrier(number_processors, rank);
time_end = MPI_Wtime();
time_final+=time_end-time_begin;
}

printf("\nTime taken by processor %d to complete is: %f\n",rank,time_final/BARRIERS);
MPI_Finalize();
return 0;
}