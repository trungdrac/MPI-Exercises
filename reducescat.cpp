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
  
  for (int i=0; i<size; i++)
        sendbuf[i] = rank + i;
  recvcounts = (int *)malloc( size * sizeof(int) );
  for (int i=0; i<size; i++)
        recvcounts[i] = 1;
  MPI_Reduce_scatter(sendbuf, &recvbuf, recvcounts, MPI_INT, MPI_SUM, comm);
 
   sumval = size * rank + ((size - 1) * size)/2;
    /* recvbuf should be size * (rank + i) */
    if (recvbuf != sumval) {
        err++;
        fprintf( stdout, "Did not get expected value for reduce scatter\n" );
        fprintf( stdout, "[%d] Got %d expected %d\n", rank, recvbuf, sumval );fflush(stdout);
    }
  MPI_Finalize();
}