/* Chương trình này sẽ all to all một số nguyên từ tiến trình rank = 0 đến các tiến trình khác */
#include <stdio.h>
#include "mpi.h"
#include "create_comm.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv){
  int rank, value, oldValue;
  oldValue = value;
  int size;
  bool result;
  int *outBuf, *inBuf;
  MPI_Comm comm;

  if (argc < 2) {
    fprintf(stderr, "Command line argument type of communicator is needed!\n");
    return 1;
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  value = rank;
  result = createComm(size, atoi(argv[1]), MPI_COMM_WORLD, comm);
  if (!result) {
    printf("Invalid parameters\n");
    return 1;
  } else {

  }
  
  // if (rank == 0){
  //   printf("Enter number of elements: ");
  //   scanf("%d", &value);
  //   printf("\n");
  // }
  outBuf = new int[size];
  for (int i = 0; i < size; i++) {
    outBuf[i] = rank;
  }
 
  
  inBuf = new int[size];
  MPI_Barrier(comm);
  MPI_Allgather(outBuf, 1, MPI_INT, inBuf, 1, MPI_INT, comm);
  MPI_Barrier(comm);
  cout << "Process " << rank << " after all-to-all: ";
  for (int i = 0; i < size; i++) {
    cout << inBuf[i] << " ";
  }
  cout << endl;

  // printf("process %d: Before MPI_Bcast, value = %d\n", rank, oldValue);
  // printf("process %d: After MPI_Bcast, value = %d\n\n", rank, value);
  MPI_Finalize();
}