#include <mpi.h>
#include <iostream>
#include <stdio.h>
using namespace std;

#define ARR_SIZE 10

void myFunction(int* first, int* second, int* len, MPI_Datatype* datatype) {
  for (int i = 0; i < *len; i++) {
    if (first[i] > 0) {
      second[i] = first[i];
    }
  }
}

int main(int argc, char** argv) {
  srand(time(NULL));

  int rank, size;
  int firstArray[ARR_SIZE];
  int secondArray[ARR_SIZE];

  MPI_Op MPI_USER;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Op_create((MPI_User_function*)myFunction, 1, &MPI_USER);
  for (int i = 0; i < ARR_SIZE; i++) {
    firstArray[i] = rand() % 10;
    secondArray[i] = 0;
  }
  MPI_Reduce(&firstArray, &secondArray, ARR_SIZE, MPI_INT, MPI_USER, 0, MPI_COMM_WORLD);

  for (int i = 0; i < ARR_SIZE; i++) {
    cout << secondArray[i] << " ";
  }

  double min = secondArray[0];
  for (int i = 1; i < ARR_SIZE; i++) {
    if ( secondArray[i] < min ) {
      min = secondArray[i];
    }
  }
  printf("Min: %f\n", min);

  MPI_Op_free(&MPI_USER);
  MPI_Finalize();

  return 0;
}
