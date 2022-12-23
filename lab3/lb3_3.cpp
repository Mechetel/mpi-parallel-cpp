#include <mpi.h>
#include <iostream>
#include <stdio.h>
using namespace std;

#define ARR_SIZE 1000

int* matrixInitialization(int x, int y) {
  srand(time(NULL));
  int* array = (int*)malloc(sizeof(int) * x * y);
  for (int i = 0; i < x * y; i++)
    array[i] = rand() % 9;

  return array;
}

void matrixMultiplication(int argc, char** argv) {
  int rank, size;

  int* matrixA = 0;
  int* matrixB = 0;
  int* matrixC = 0;
  double start_time, end_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0)
  {
    start_time = MPI_Wtime();
    matrixA = matrixInitialization(ARR_SIZE, ARR_SIZE);
    matrixB = matrixInitialization(ARR_SIZE, ARR_SIZE);
    matrixC = (int*)malloc(sizeof(int) * ARR_SIZE * ARR_SIZE);

    for (int i = 0; i < ARR_SIZE; i++) {
      for (int j = 0; j < ARR_SIZE; j++) {
        matrixC[i * ARR_SIZE + j] = 0;
        for (int k = 0; k < ARR_SIZE; k++) {
          matrixC[i * ARR_SIZE + j] += matrixA[i * ARR_SIZE + k] * matrixB[k * ARR_SIZE + j];
        }
      }
    }
    end_time = MPI_Wtime() - start_time;
    printf("Total run time = %f\n", end_time);
  }

  free(matrixA);
  free(matrixB);
  free(matrixC);

  MPI_Finalize();
}

void parallelMatrixMultiplication(int argc, char** argv) {
  int rank, size;

  int* matrixA = 0;
  int* matrixB = 0;
  int* matrixC = 0;
  double start_time, end_time;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  start_time = MPI_Wtime();

  if (rank == 0) {
    matrixA = matrixInitialization(ARR_SIZE, ARR_SIZE);
    matrixB = matrixInitialization(ARR_SIZE, ARR_SIZE);
    matrixC = (int*)malloc(sizeof(int) * ARR_SIZE * ARR_SIZE);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank != 0) {
    matrixA = (int*)malloc(sizeof(int) * ARR_SIZE * ARR_SIZE);
    matrixB = (int*)malloc(sizeof(int) * ARR_SIZE * ARR_SIZE);
  }


  MPI_Bcast(matrixB, ARR_SIZE * ARR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

  int* elementsPerProccess = (int*)malloc(sizeof(int) * size);
  int* displacements = (int*)malloc(sizeof(int) * size);

  for (int i = 0; i < size; i++) {
    elementsPerProccess[i] = ARR_SIZE / size;
    elementsPerProccess[i] = elementsPerProccess[i] * ARR_SIZE;
  }

  int unassignedRows = ARR_SIZE % size;
  if (unassignedRows != 0)
    for (int i = 0; i < unassignedRows; i++)
      elementsPerProccess[i] += ARR_SIZE;


  displacements[0] = 0;

  for (int i = 1; i < size; i++)
    displacements[i] = displacements[i - 1] + elementsPerProccess[i - 1];

  int* local_a = (int*)malloc(sizeof(int) * elementsPerProccess[rank]);
  int* local_c = (int*)malloc(sizeof(int) * elementsPerProccess[rank]);

  MPI_Scatterv(matrixA, elementsPerProccess, displacements, MPI_INT, local_a, elementsPerProccess[rank], MPI_INT,0, MPI_COMM_WORLD);

  int rows = elementsPerProccess[rank] / ARR_SIZE;
  int sum = 0;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < ARR_SIZE; j++) {
      sum = 0;
      for (int k = 0; k < ARR_SIZE; k++)
        sum += local_a[i * ARR_SIZE + k] * matrixB[k * ARR_SIZE + j];

      local_c[i * ARR_SIZE + j] = sum;
    }
  }

  MPI_Gatherv(local_c, elementsPerProccess[rank], MPI_INT, matrixC, elementsPerProccess, displacements, MPI_INT, 0, MPI_COMM_WORLD);

  end_time = MPI_Wtime() - start_time;
  printf("Total run time = %f\n", end_time);

  free(matrixA);
  free(matrixB);
  free(matrixC);
  free(elementsPerProccess);
  free(displacements);
  free(local_a);
  free(local_c);

  MPI_Finalize();
}

int main(int argc, char** argv)
{
  parallelMatrixMultiplication(argc, argv);
  /* matrixMultiplication(argc,argv); */
  return 0;
}
