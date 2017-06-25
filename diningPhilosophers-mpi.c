#include "mpi.h"
#include "stdlib.h"
#include "stdio.h"

#define nFil 5
#define MASTER 0
#define FROM_MASTER 1
#define true 1
#define false 0
#define LEFT (taskId+nFil-1)%nFil
#define RIGHT (taskId)%nFil
#define request 10
#define nill 	11
#define fork 	12


typedef int bool;


void needs_resource(bool* hungry, bool* holds_fork, int taskId)
{
	hungry[taskId] = true;
	int message = request;
	MPI_Send(&message, 1, MPI_INT, LEFT, 1, MPI_COMM_WORLD);
	MPI_Send(&message, 1, MPI_INT, RIGHT, 1, MPI_COMM_WORLD);
}

void pegaFork(int taskId, bool hungry, bool holds_turn, bool owes_fork)
{
	MPI_Status status;
	int message;
	MPI_Recv(&message, nFil, MPI_INT, taskId, 1, MPI_COMM_WORLD, &status);
	if(message == fork)
	{
		holds_fork = true;
		// if()
	}
}
void verificaRequest(int destination, bool hungry, bool holds_turn, bool owes_fork)
{
	MPI_Status status;
	int message;
	MPI_Recv(&message, nFil, MPI_INT, destination, 1, MPI_COMM_WORLD, &status);
	if(message == request || message_b == request)
	{
		if (hungry == false || holds_turn == false)
		{
			holds_turn = false;
			if(hungry == false)
				message = nill;
				// MPI_Send(message, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
			else
				message = fork;
			
			MPI_Send(&message, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
		}
	}else
	{
		owes_fork = true;
	}
}

int main(int argc, char *argv[]) 
{	
	int taskId, 	       //id de cada filósofo
		numTasks;	       //numero total de filósofos
	
	bool hungry, holds_fork, holds_turn, owes_fork;

	// bool hungry[nFil]    , //filósofos com fome
	//      holds_fork[nFil], //
	//      holds_turn[nFil], //
	// 	 owes_fork[nFil];  //
	
	MPI_Init (&argc, &argv);      /* starts MPI */
	MPI_Status status;
	MPI_Comm_rank (MPI_COMM_WORLD, &taskId);
	MPI_Comm_size (MPI_COMM_WORLD, &numTasks);
	
	if (numTasks != nFil) 
	{
		printf("O número de processos deve ser igual a 5");
		MPI_Abort(MPI_COMM_WORLD, 1);
		exit(1);
	}
	
	//Estado inicial dos filósofos: ninguem tem preferência
	//                              nenhum possui qualquer garfo (recurso)
	//                              ninguem deve nenhum garfo (recurso).
	for (int i = 0; i < numTasks; i++) 
	{
		// hungry[i]     = false;
		// holds_fork[i] = false;
		// holds_turn[i] = false;
		// owes_fork[i]  = false;

	}
	
	if(taskId == MASTER) 
	{
		printf("inicializando filósofos...\n");
		// for (int i = 1; i < numTasks; i++) 
		// {
		// 	MPI_Send(&hungry, nFil, MPI_INT, i, 1, MPI_COMM_WORLD);
		// 	MPI_Send(&holds_fork, nFil, MPI_INT, i, 1, MPI_COMM_WORLD);
		// 	MPI_Send(&holds_turn, nFil, MPI_INT, i, 1, MPI_COMM_WORLD);
		// 	MPI_Send(&owes_fork, nFil, MPI_INT, i, 1, MPI_COMM_WORLD);
		// }
	}else
	{
		while(true)
		{
			//verifica se há request
			verificaRequest(LEFT, hungry, holds_turn, owes_fork);
			verificaRequest(RIGHT, hungry, holds_turn, owes_fork);
			pegaFork(LEFT, hungry, holds_turn, owes_fork);
			pegaFork(RIGHT, hungry, holds_turn, owes_fork);
		}
		// while(true)
		// {
			
		// 	for(int i = 0; i < nFill; i++)
		// 	{
		// 		int resp;
		// 		MPI_Recv(&resp, nFil, MPI_INT, i, 1, MPI_COMM_WORLD, &status);	
		// 		if (resp == request)
		// 		{

		// 		}
		// 	}
			
		// }
		// MPI_Recv(&hungry, nFil, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		// MPI_Recv(&holds_fork, nFil, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		// MPI_Recv(&holds_turn, nFil, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		// MPI_Recv(&owes_fork, nFil, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		// if(taskId == 1)
			// needs_resource(hungry, holds_fork, taskId);
	}
}