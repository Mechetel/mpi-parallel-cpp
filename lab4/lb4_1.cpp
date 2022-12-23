#include <mpi.h>
#include <iostream>
#include <stdio.h>
using namespace std;

struct newtype {
    float a;
    float b;
    int n;
};
int main(int argc, char* argv[]) {
  int myrank;
  MPI_Datatype NEW_MESSAGE_TYPE;
  int block_lengths[3];
  MPI_Aint displacements[3];
  MPI_Aint addresses[4];
  MPI_Datatype typelist[3];
  int blocks_number;
  struct newtype indata;
  int tag = 0;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  typelist[0] = MPI_FLOAT;
  typelist[1] = MPI_FLOAT;
  typelist[2] = MPI_INT;
  block_lengths[0] = block_lengths[1] = block_lengths[2] = 1;
  MPI_Address(&indata, &addresses[0]);
  MPI_Address(&(indata.a), &addresses[1]);
  MPI_Address(&(indata.b), &addresses[2]);
  MPI_Address(&(indata.n), &addresses[3]);
  displacements[0] = addresses[1] - addresses[0];
  displacements[1] = addresses[2] - addresses[0];
  displacements[2] = addresses[3] - addresses[0];
  blocks_number = 3;
  MPI_Type_struct(blocks_number, block_lengths, displacements, typelist, &NEW_MESSAGE_TYPE);
  MPI_Type_commit(&NEW_MESSAGE_TYPE);
  if (myrank == 0) {
    indata.a = 3.14159;
    indata.b = 2.71828;
    indata.n = 2010;
    MPI_Send(&indata, 1, NEW_MESSAGE_TYPE, 1, tag, MPI_COMM_WORLD);
    printf("Process %i send: %f %f %i\n", myrank, indata.a, indata.b, indata.n);
  } else {
    MPI_Recv(&indata, 1, NEW_MESSAGE_TYPE, 0, tag, MPI_COMM_WORLD, &status);
    printf("Process %i received: %f %f %i, status %d\n", myrank, indata.a, indata.b, indata.n, status.MPI_ERROR);
  }
  MPI_Type_free(&NEW_MESSAGE_TYPE);
  MPI_Finalize();
  return 0;
}
