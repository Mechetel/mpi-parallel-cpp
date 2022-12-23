#include <mpi.h>
#include <iostream>
#include <stdio.h>
using namespace std;

float* creatingRandomArray(int size) {
  srand(time(NULL));
  float* array = (float*)malloc(sizeof(float) * size * size);
  for (int i = 0; i < size * size; i++)
    array[i] = (rand() / (float)RAND_MAX);

  return array;
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 2) {
    if (rank == 0) {
      printf("For executing this program you need only 2 ranks,in your case you tried to use %d! Exiting....\n", size);
    }
    MPI_Finalize();
    exit(0);
  }

  MPI_Request request;
  MPI_Status status;
  float* array = 0;
  float sum = 0;
  double start = MPI_Wtime(), end = 0;

  if (rank == 0) {
    array = creatingRandomArray(size);
    for (int i = 0; i < size * size; i++)
      sum += array[i];

    MPI_Send(&sum, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
  }
  else {
    MPI_Recv(&sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
    end = MPI_Wtime() - start;
    printf("Elapsed time: %f\n", end);
    printf("Sum: %f", sum);
  }
  free(array);
  MPI_Finalize();
}


/*
void Simple(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank,size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    double start = MPI_Wtime();
    float sum = 0.0f;
    float* array = creatingRandomArray(size);
    for (int i = 0; i < size * size; i++)
      sum += array[i];

    double end = MPI_Wtime() - start;
    printf("Elapsed time: %f\n", end);
    printf("Sum: %f", sum);

    free(array);
  }
  MPI_Finalize();
}

void Collective(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  float* matrixRow = (float*)malloc(sizeof(float) * size);
  float* array = 0;
  float* matrixSum = (float*)malloc(sizeof(float) * size);
  double start;

  if (!rank) {
    start = MPI_Wtime();
    array = creatingRandomArray(size);
  }

  MPI_Scatter(array, size, MPI_FLOAT, matrixRow, size, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Reduce(matrixRow, matrixSum, size, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  if(!rank) {
    float sum = 0;

    for (int i = 0; i < size; i++)
      sum += matrixSum[i];

    double end = MPI_Wtime() - start;
    printf("Elapsed time: %f\n", end);
    printf("Sum: %f", sum);
  }

  free(array);
  free(matrixRow);
  free(matrixSum);

  MPI_Finalize();
}

void Point(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 2) {
    if (rank == 0) {
      printf("For executing this program you need only 2 ranks,in your case you tried to use %d! Exiting....\n", size);
    }
    MPI_Finalize();
    exit(0);
  }

  MPI_Request request;
  MPI_Status status;

  float* array = 0;
  float sum = 0;
  double start = MPI_Wtime(), end = 0;

  if (rank == 0) {
    array = creatingRandomArray(size);
    for (int i = 0; i < size * size; i++)
      sum += array[i];

    MPI_Send(&sum, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
  }
  else {
    MPI_Recv(&sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
    end = MPI_Wtime() - start;
    printf("Elapsed time: %f\n", end);
    printf("Sum: %f", sum);
  }
  free(array);
  MPI_Finalize();
}
*/
