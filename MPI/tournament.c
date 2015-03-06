#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define victory_node 1
#define losing_node 2
#define lost 3
#define head_node 4
#define lost_round_number1 5
#define BARRIERS 10
#define flag 1

int dyn;

struct match
{
	int role;
	int opponent_node;

};

void tournament_barrier(struct match array[dyn][10], int rank, int num_round_numbers)
{
	
	int round_number=1,message=1,tag=1;
	
	MPI_Status stat;

	while(1)
	{
			
		if(array[rank][round_number].role==losing_node)
		{
			int id_victory_node=array[rank][round_number].opponent_node;
			MPI_Send(&message,1,MPI_INT,id_victory_node,tag,MPI_COMM_WORLD);
			break;
			
		}

		if(array[rank][round_number].role==victory_node)
		{
			int id_losing_node=array[rank][round_number].opponent_node;
			MPI_Recv(&message,1,MPI_INT,id_losing_node,tag,MPI_COMM_WORLD,&stat);
		}

		if(array[rank][round_number].role==head_node)
		{
			int id_runnerup=array[rank][round_number].opponent_node;
			MPI_Recv(&message,1,MPI_INT,id_runnerup,tag,MPI_COMM_WORLD,&stat);
			break;
		}

			round_number++;
	}


	while(1)
	{

		if(array[rank][round_number].role == victory_node||array[rank][round_number].role == head_node) 
		{
			int id_losing_node_to_waken = array[rank][round_number].opponent_node;
			MPI_Send(&message,1,MPI_INT,id_losing_node_to_waken,tag,MPI_COMM_WORLD);
		}

		if(array[rank][round_number].role == losing_node) 
		{
			int id_victory_node1 = array[rank][round_number].opponent_node;
			MPI_Recv(&message,1,MPI_INT,id_victory_node1,tag,MPI_COMM_WORLD,&stat);
		}

		if(array[rank][round_number].role == lost_round_number1)
			break;

			round_number--;

	}

}

int main(int argc, char **argv)
{
	int rank;

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &dyn);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int num_round_numbers=ceil(log(dyn)/log(2));

	int i=0, j=0, k=0;
	int value_1,value_2=0;
	j = rank;
	struct match array[dyn][10];

	for( k=0; k<=num_round_numbers; k++ ) 
	{
		array[j][k].role = 0;
		array[j][k].opponent_node = 0;
	}

	for(k=0;k<=num_round_numbers;k++)
	{
		value_1=ceil(pow(2,k));
		value_2=ceil(pow(2,k-1));

		if(k==0)
			array[j][k].role=lost_round_number1;	
		
		if(k>0 && j==0 && value_1>=dyn)
			array[j][k].role=head_node;

		if(k>0 && (j%value_1==value_2))
			array[j][k].role=losing_node;

		if(k>0 && (j%value_1==0) && (j+value_2)>=dyn)
			array[j][k].role=lost;

		if(k>0 && (j%value_1==0) && (j+value_2)<dyn && value_1<dyn)
			array[j][k].role=victory_node;

		if(array[j][k].role==losing_node)
			array[j][k].opponent_node=j-value_2;

		if(array[j][k].role==victory_node || array[j][k].role==head_node)
			array[j][k].opponent_node=j+value_2;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	
	double time_begin,time_end,time_final=0.0;
	
	for( i=0; i<BARRIERS; i++ )
	{
		k=0;
		while(k<100)
			k++;
	//	printf("Processor %d reached barrier %d\n",rank, i);

     	time_begin = MPI_Wtime();
     	tournament_barrier(array,rank,num_round_numbers);
     	time_end = MPI_Wtime();
       	time_final+=time_end-time_begin;
	}
	
	printf("\nProcessor %d spent %f\n",rank,time_final/BARRIERS);

	MPI_Finalize();

}