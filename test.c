#include <stdio.h>
#include <mpi.h>

int main()
{
	int npes, myrank, i = 5;
	printf("testing\n");
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &npes);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	printf("i: %d, myrank: %d, npes: %d\n", i, myrank, npes);
	MPI_Finalize();
	return 0;

}
