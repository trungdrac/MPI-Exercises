/* Chương trình này sẽ broadcast một số nguyên từ tiến trình rank = 0 đến các tiến trình khác */
#include <stdio.h>
#include "mpi.h"
#include "create_comm.h"

int main(int argc, char** argv){
  int rank, value, oldValue;
  oldValue = value;
  int size;
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
  
  if (rank == 0){
    printf("Enter a number to broadcast: ");
    scanf("%d", &value);
    printf("\n");
  }

  MPI_Bcast(&value, 1, MPI_INT, 0, comm);
  printf("process %d: Before MPI_Bcast, value = %d\n", rank, oldValue);
  printf("process %d: After MPI_Bcast, value = %d\n\n", rank, value);
  MPI_Finalize();
}