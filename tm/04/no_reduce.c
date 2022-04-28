#include <stdio.h>
#include <math.h>
#include <mpi.h>


int main(int argc, char **argv) {
  // MPI initialization 
  int rank, size;
  MPI_Status status;

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // The initial values, u_i = i^2; v_i = log(i+1)
  float u_i = rank*rank;
  float v_i = log(rank+1.0);

  // Each process computes its intermediate value
  float i_value = u_i * v_i;


  if (rank == 0) {
    // Reducing on process 0:
    float result = 0.0;
    float aux;
    
    MPI_Barrier(MPI_COMM_WORLD);
    for (int i = 0; i < size -1; i++) {
      MPI_Recv(&aux, 1, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      result += aux;
      printf("Received value from %d\n", status.MPI_SOURCE);
    }

    printf ("The reduced value is %f on rank 0\n", result);

    // Checking the result
    float validation = 0.0f;
    for (int i=0; i < size; ++i) {
        validation += i*i * log(i+1.0f);
    }

    printf("Validation gives the value : %f\n ", validation);
  }
  else {
    // print tmp and result in each other process 
    printf("Process %d ui*vi = %f\n", rank, i_value);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Send(&i_value, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}