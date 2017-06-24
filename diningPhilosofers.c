#include <mpi.h>
#include <semaphore.h>
#include <stdio.h>

#define N 			5
#define LEFT 		(i+N-1)%N
#define RIGHT		(i+1)%N
#define THINKING 	0
#define HUNGRY		1
#define EATING		2
#define true 		1
#define false		2
typedef int semaphore;
int state[N];
semaphore mutex = 1;
semaphore s[N];

void test(int i)
{
	if(state[i] == HUNGRY && state[LEFT]!= EATING && state[RIGHT] != EATING)
	{
		state[i] = EATING;
		up(&mutex);
	}
}

void put_forks(int i)
{
	down(&mutex);
	state[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	up(&mutex);
}

void take_forks(int i)
{
	down(&mutex);
	state[i] = HUNGRY;
	test(i);
	up(&mutex);
	down(&s[i]);
}

void philosopher(int i)
{
	while(true)
	{
		//think();
		take_forks(i);
		//eat();
		put_forks(i);
	}
}

int main(int argc, char* argv[])
{
	int rank, size;
 
  	MPI_Init (&argc, &argv);      /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
  	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes */
  	printf( "Hello world from process %d of %d\n", rank, size );
  	MPI_Finalize();
  	return 0;
}

