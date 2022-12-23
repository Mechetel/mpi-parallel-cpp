#include <mpi.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Comm comm;
  MPI_Group mainGroup;

  MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &comm);
  MPI_Comm_group(comm, &mainGroup);

  int ranks[] = { 1,2,3,4,10 };

  MPI_Group mainGroupPart;
  MPI_Group_incl(mainGroup, 5, ranks, &mainGroupPart);

  int buff_rank, buff_size;

  MPI_Group_rank(mainGroup, &buff_rank);
  MPI_Group_size(mainGroup, &buff_size);

  if (buff_rank % 4 == 0) {
    cout << "mainGroup: rank #" << buff_rank << " size:" << buff_size << endl;
  }

  MPI_Group_rank(mainGroupPart, &buff_rank);
  MPI_Group_size(mainGroupPart, &buff_size);

  if (buff_rank % 4 == 0) {
    cout << "mainGroupPart: rank #" << buff_rank << " size:" << buff_size << endl;
  }

  if (rank % 2 == 0) {
    MPI_Comm_split(MPI_COMM_WORLD, 1, rank, &comm);
    MPI_Group even_group;
    MPI_Comm_group(comm, &even_group);
    MPI_Group diff_group;
    MPI_Group_difference(mainGroup, even_group, &diff_group);

    int buff_rank, buff_size;

    MPI_Group_size(even_group, &buff_size);
    MPI_Group_rank(even_group, &buff_rank);

    if (buff_rank % 4 == 0) {
      cout << "even_group: rank #" << buff_rank << " size:" << buff_size << endl;
    }

    MPI_Group_size(diff_group, &buff_size);
    MPI_Group_rank(diff_group, &buff_rank);

    if (buff_rank % 5 == 0) {
      cout << "diff_group: rank #" << buff_rank << " size:" << buff_size << endl;
    }
  } else if (rank % 2 != 0) {

    MPI_Comm_split(MPI_COMM_WORLD, 2, rank, &comm);
    MPI_Group uneven_group;
    MPI_Comm_group(comm, &uneven_group);
    MPI_Group inter_group;
    MPI_Group_intersection(mainGroup, uneven_group, &inter_group);

    int buff_rank, buff_size;

    MPI_Group_size(uneven_group, &buff_size);
    MPI_Group_rank(uneven_group, &buff_rank);

    if (buff_rank % 4 == 0) {
      cout << "uneven_group: rank #" << buff_rank << " size:" << buff_size << endl;
    }

    MPI_Group_size(inter_group, &buff_size);
    MPI_Group_rank(inter_group, &buff_rank);

    if (buff_rank % 4 == 0) {
      cout << "inter_group: rank #" << buff_rank << " size:" << buff_size << endl;
    }
  }

  MPI_Finalize();
  return 0;
}
