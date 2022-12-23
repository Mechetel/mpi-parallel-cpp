#include <mpi.h>
#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char** argv) {
  int rank;
  int m[4][4] = { 5, 5, 5, 5, 5, 8, 8, 5, 5, 8, 8, 5, 5, 5, 5, 5 };
  int n[4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  MPI_Status mystatus;
  int i, j;
  char buffer[100];
  int position = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    MPI_Pack(&m[1][1], 2, MPI_INT, buffer, 100, &position, MPI_COMM_WORLD);
    MPI_Pack(&m[2][1], 2, MPI_INT, buffer, 100, &position, MPI_COMM_WORLD);
    MPI_Send(buffer, 100, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
  } else {
    for (i = 0; i < 4; i++) {
      printf("\n");
      for (j = 0; j < 4; j++)
        printf("%i ", n[i][j]);
    }
    MPI_Recv(buffer, 100, MPI_PACKED, 0, 0, MPI_COMM_WORLD, &mystatus);
    MPI_Unpack(buffer, 100, &position, &n[1][1], 2, MPI_INT, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 100, &position, &n[2][1], 2, MPI_INT, MPI_COMM_WORLD);
    printf("\n");

    for (i = 0; i < 4; i++) {
      printf("\n");
      for (j = 0; j < 4; j++)
        printf("%i ", n[i][j]);
    }
  }
  MPI_Finalize();
  return 0;
}
