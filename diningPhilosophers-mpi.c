#include "mpi.h"
#include "stdlib.h"
#include "stdio.h"

#define nFil 5
#define MASTER 0
#define FROM_MASTER 1
#define true 1
#define false 0
typedef int bool;

int main(int argc, char *argv[]) {
	
	
	int taskId, 	       //id de cada filósofo
		numTasks;	       //numero total de filósofos
		
	bool hungry[nFil]    , //filósofos com fome
	     holds_fork[nFil], //
	     holds_turn[nFil], //
		 owes_fork[nFil];  //

	MPI_Status status;
	MPI_Comm_rank (MPI_COMM_WORLD, &taskId);
	MPI_Comm_size (MPI_COMM_WORLD, &numTasks);
	
	if (numTasks != nFil) {
		printf("O número de processos deve ser igual a 5");
		MPI_Abort(MPI_COMM_WORLD, 1);
		exit(1);
	}
	
	//Estado inicial dos filósofos: ninguem tem preferência
	//                              nenhum possui qualquer garfo (recurso)
	//                              ninguem deve nenhum garfo (recurso).
	for (int i = 0; i < numTasks; i++) {
		hungry[i]     = false;
		holds_fork[i] = false;
		holds_turn[i] = false;
		owes_fork[i]  = false;
	}
	
	if(taskId == MASTER) {
		printf("inicializando filósofos...");
		
		for (int i = 1; i < numTasks; i++) {
			MPI_Send(&hungry, nFil, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&holds_fork, nFil, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&holds_turn, nFil, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&owes_fork, nFil, MPI_INT, i, 1, MPI_COMM_WORLD);
		}
	}
}