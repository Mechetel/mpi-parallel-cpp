#include <mpi.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Group main_group;
  MPI_Comm_group(MPI_COMM_WORLD, &main_group);
  int num = (size - 1) / 7;
  int* arr = new int[size - 1];
  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      arr[i - 1] = i;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(arr, size - 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Group first_group;
    MPI_Group_incl(main_group, num, arr, &first_group);

    for (int i = 1; i < 7; i++) {
      MPI_Group group;
      MPI_Group_incl(main_group, num, &arr[num * i], &group);
    }
  } else {
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(arr, size - 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Group first_group;
    MPI_Group_incl(main_group, num, arr, &first_group);
    int buff_rank;
    MPI_Group_rank(first_group, &buff_rank);
    if (buff_rank != MPI_UNDEFINED) {
      cout << "Rank #" << buff_rank << " in first group" << endl;
    }
  }
  delete[] arr;

  MPI_Finalize();
  return 0;
}
