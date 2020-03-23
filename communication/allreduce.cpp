/* Chương trình này sẽ broadcast một số nguyên từ tiến trình rank = 0 đến các tiến trình khác */
#include <stdio.h>
#include "mpi.h"
#include "create_comm.h"

int main(int argc, char** argv){
  int rank, value;
  int size;
  int sum = 0;
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
  
  value = rank +1;
  MPI_Allreduce(&value, &sum, 1, MPI_INT, MPI_SUM, comm);
 
  printf("%d\n", sum);
  MPI_Finalize();
}