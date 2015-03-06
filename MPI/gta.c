#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#define BARRIERS 1

double time_begin, time_end, exec_time;


void barrier(int *number_processors, int *proc_id, int *sense)
{
	int msg=1;
	int i;
	int iterator = *(number_processors)-1;
	if(*proc_id != *(number_processors)-1)
	{
		MPI_Send(&msg, 1, MPI_INT, iterator, 1, MPI_COMM_WORLD);
	}
	if( *proc_id == *(number_processors))
	{	for(i=0;i<iterator;i++)
		{
			MPI_Recv(&msg, 1, MPI_INT, i,1,MPI_COMM_WORLD, NULL);	
		}
		
	}

	if(*proc_id == *(number_processors)-1)
		{
				*sense = !*sense;

		}
	{
		MPI_Bcast(sense, 1, MPI_INT, iterator, MPI_COMM_WORLD);
	}

	while(!*sense)
		*sense = !*sense;
}

int main(int argc, char **argv)
{
	int i, proc_id, number_processors;
	int sense =0;
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &number_processors);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

	for(i=0; i<BARRIERS; i++)
	{
		time_begin = MPI_Wtime();
		barrier(&number_processors, &proc_id, &sense);
		time_end=MPI_Wtime();
		exec_time+= (time_end- time_begin);
		printf("Processor %d has reached the barrier %d \n", proc_id, i);
	}
	printf("Time spent in barrier by process %d is %f\n", proc_id, exec_time/BARRIERS);
	MPI_Finalize();
	return 0;

}