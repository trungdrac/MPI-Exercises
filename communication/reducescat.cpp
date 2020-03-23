/* Chương trình này sẽ broadcast một số nguyên từ tiến trình rank = 0 đến các tiến trình khác */
#include <stdio.h>
#include "mpi.h"
#include "create_comm.h"

int main(int argc, char** argv){
  int rank, value;
  int size;
  int *sendbuf, recvbuf, *recvcounts;
  bool result;
  MPI_Comm comm;

  if (argc < 2) {
    fprintf(stderr, "Command line argument type of communicator is needed!\n");
    return 1;
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  result = createComm(size, atoi(argv[1]), MPI_COMM_WORLD, comm);
  if (!result) {
    printf("Invalid parameters\n");
    return 1;
  } else {

  }

  sendbuf = (int *) malloc( size * sizeof(int) );
    for (int i=0; i<size; i++)
        sendbuf[i] = size + 1;
    recvcounts = (int *)malloc( size * sizeof(int) );
    for (int i=0; i<size; i++)
        recvcounts[i] = 1;
 
    MPI_Reduce_scatter( sendbuf, &recvbuf, recvcounts, MPI_INT, MPI_SUM, comm );
    printf("%d %d\n",rank, recvbuf);

  MPI_Finalize();
}

