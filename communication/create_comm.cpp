#include "create_comm.h"

using namespace std;

bool createComm(int world_size, int dimension, MPI_Comm WORLD, MPI_Comm &new_world) {
  //MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &newComm);
  if (dimension == 1) {
    if (world_size != 8) return false;
    int dim[1], period[1];
    dim[0] = 8;
    period[0] = 0;
    MPI_Cart_create(MPI_COMM_WORLD, 1, dim, period, true, &new_world);
    return true;
  }
  if (dimension == 2) {
    if (world_size != 9) return false;
    int dim[2], period[2];
    dim[0] = 3; dim[1] = 3;
    period[0] = 0; period[1] = 0;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, true, &new_world);
    return true;
  }
  if (dimension == 3) {
    if (world_size != 8) return false;
    int dim[3], period[3];
    dim[0] = 2; dim[1] = 2; dim[2] = 2;
    period[0] = 0; period[1] = 0; period[2] = 2;
    MPI_Cart_create(MPI_COMM_WORLD, 3, dim, period, true, &new_world);
    return true;
  }
  return false;
}