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
  MPI_Group first_group;
  MPI_Group second_group;

  int* arr = new int[size - 1];
  int firstArray[] = { 0,1,2,3,4 };
  int secondArray[] = { 0,1,2,3,4,5,6,7,8,9 };

  for (int i = 1; i < size; i++)
    arr[i - 1] = i;

  MPI_Comm_group(MPI_COMM_WORLD, &main_group);

  MPI_Group_incl(main_group, 4, firstArray, &first_group);
  MPI_Group_incl(main_group, 9, secondArray, &second_group);

  MPI_Group uni;
  MPI_Group_union(first_group, second_group, &uni);

  MPI_Group diff;
  MPI_Group_difference(first_group, second_group, &diff);

  MPI_Group inter;
  MPI_Group_intersection(first_group, second_group, &inter);

  int buff_rank;
  MPI_Group_rank(first_group, &buff_rank);
  if (buff_rank != MPI_UNDEFINED) {
    cout << "Rank #" << buff_rank << " in all groups" << endl;
  }

  delete[] arr;

  MPI_Finalize();
  return 0;
}
