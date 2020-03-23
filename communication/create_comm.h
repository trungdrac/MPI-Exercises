#ifndef CREATE_COMM
#define CREATE_COMM

#include <iostream>
#include <mpi.h>

bool createComm(int world_size, int dimension, MPI_Comm WORLD, MPI_Comm &new_world);

#endif