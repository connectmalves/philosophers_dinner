#include "mpi.h"
#include "stdlib.h"
#include "stdio.h"

#define nFil 			5
#define MASTER 			0
#define FROM_MASTER 	1
#define true 			1
#define false 			0
#define LEFT 			(taskId+nFil-1)%nFil
#define RIGHT 			(taskId)%nFil
#define request 		10
#define fork_nill 		11
#define fork_request	12
#define fork_turn 		13	
#define turn 			14


typedef int bool;


void needs_resource(bool* hungry, bool* holds_fork, int taskId)
{
	hungry[taskId] = true;
	int message = request;
	MPI_Send(&message, 1, MPI_INT, LEFT, 1, MPI_COMM_WORLD);
	MPI_Send(&message, 1, MPI_INT, RIGHT, 1, MPI_COMM_WORLD);
}

void pegaFork(int taskId, bool hungry, bool holds_turn, bool owes_fork, bool holds_fork)
{
	MPI_Status status;
	int message;
	MPI_Recv(&message, nFil, MPI_INT, taskId, 1, MPI_COMM_WORLD, &status);
	if(message == fork_nill || message == fork_request || message == fork_turn)
	{
		holds_fork = true;
		if(message == turn)
		{
			holds_turn = true;
		}
		if(message == fork_request)
		{
			owes_fork = true;
		}
	}
}
void verificaRequest(int destination, bool hungry, bool holds_turn, bool owes_fork, bool holds_fork)
{
	MPI_Status status;
	int message;
	MPI_Recv(&message, nFil, MPI_INT, destination, 1, MPI_COMM_WORLD, &status);
	if(message == request)
	{
		if (hungry == false || holds_turn == false)
		{
			holds_fork = false;
			if(hungry == false)
				message = fork_nill;
			else
				message = fork_request;
			
			MPI_Send(&message, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
		}
	}else
	{
		owes_fork = true;
	}
}

void pegaRodada(int source, bool holds_turn)
{
	MPI_Status status;
	int message;
	MPI_Recv(&message, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
	if(message == turn)
	{
		holds_turn= true;
	}
}

void acessaResources(int destination, bool hungry, bool holds_turn, bool owes_fork, bool holds_fork)
{
	if(holds_fork == true)
	{
		printf("Acessando recurso compartilhado");
		hungry = false;
		int message;
		if(holds_turn == true)
		{
			holds_turn = false;
			if(owes_fork == true)
			{
				owes_fork = false;
				holds_fork = false;
				message = fork_turn;
			}
		}
		else
		{
			message = turn;		
		}
		MPI_Send(&message, 1, MPI_INT, destination, 1, MPI_COMM_WORLD);
	}
}

int main(int argc, char *argv[]) 
{	
	int taskId, 	       //id de cada filósofo
		numTasks;	       //numero total de filósofos
	
	bool hungry, holds_fork, holds_turn, owes_fork;
	
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
		hungry     = false;
		holds_fork = false;
		holds_turn = false;
		owes_fork  = false;
	}
	
	if(taskId == MASTER) 
	{
		printf("inicializando filósofos...\n");
		for (int i = 1; i < numTasks; i++) 
		{
			MPI_Send(&hungry, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&holds_fork, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&holds_turn, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&owes_fork, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
	}else
	{
		while(true)
		{
			verificaRequest(LEFT, hungry, holds_turn, owes_fork, holds_fork);
			verificaRequest(RIGHT, hungry, holds_turn, owes_fork, holds_fork);
			pegaFork(LEFT, hungry, holds_turn, owes_fork, holds_fork);
			pegaFork(RIGHT, hungry, holds_turn, owes_fork, holds_fork);
			acessaResources(LEFT, hungry, holds_turn, owes_fork, holds_fork);
			acessaResources(RIGHT, hungry, holds_turn, owes_fork, holds_fork);
		}
	}
}