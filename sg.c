// Run with: mpicc ring.c -o ring && mpirun -n 8 ring 3 2
// num_elements_per_proc: 2
// num_of_nodes: 8
// num_of_dims: 3

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>
#include <stdbool.h>

// Tạo topology
bool createComm(int world_size, int dimension, MPI_Comm WORLD, MPI_Comm new_world) {

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
}
// Tạo mảng có num_elements float trong khoảng 0, 1
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

// Tính TBC
float compute_avg(float *array, int num_elements) {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++) {
    sum += array[i];
  }
  return sum / num_elements;
}

int main(int argc, char** argv) {

  int num_elements_per_proc = atoi(argv[2]);
  srand(time(NULL));

    int world_rank, world_size;
    MPI_Comm newComm;
    int reorder;
    int coord[2], id;
    int variable = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    bool result = createComm(world_size, atoi(argv[1]), MPI_COMM_WORLD, newComm);
    if (!result) {
        printf("Invalid parameters\n");
        return 1;
    } 

  // Tạo mảng trên process 0
  float *rand_nums = NULL;
  if (world_rank == 0) {
    rand_nums = create_rand_nums(num_elements_per_proc * world_size);
    printf("On process %d: ", world_rank);
    for (int i = 0; i < num_elements_per_proc * world_size; i++)
    {
      printf("%f, ", rand_nums[i]);
    }
    printf("\n\n");
  }

  // Cấp bộ nhớ chứa mảng nhận đc từ process 0 trên các process còn lại
  float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
  assert(sub_rand_nums != NULL);

  // Scatter các mảng từ p0 
  MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);
  if (world_rank != 0){
    printf("On process %d: ", world_rank);
    float sum_on_process = 0;
    for (int i = 0; i < num_elements_per_proc; i++)
    {
      printf("%f, ", sub_rand_nums[i]);
    }
    printf("\n");
  }
  // Tính TBC trên các mảng con
  float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);
  if (world_rank != 0) {
    printf("Avg on process %d is: %f\n\n", world_rank, sub_avg);
  }
  // Gather các TBC đến p0 từ các process còn lại
  float *sub_avgs = NULL;
  if (world_rank == 0) {
    sub_avgs = (float *)malloc(sizeof(float) * world_size);
    assert(sub_avgs != NULL);
  }
  MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  if (world_rank == 0) {
    printf("Array sub_avgs on process 0 is: ");
    for (int i = 0; i < world_size; i++)
    {
      printf("%f ", sub_avgs[i]);
    }
  }

  // Clean up
  if (world_rank == 0) {
    free(rand_nums);
    free(sub_avgs);
  }
  free(sub_rand_nums);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}