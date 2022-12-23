#include <mpi.h>
#include <iostream>
#include <stdio.h>
using namespace std;

#define SIZE 22

char message[SIZE];

void SendMessage(int size) {
  MPI_Barrier(MPI_COMM_WORLD);
  for (int i = 1; i < size; i++)
    MPI_Ssend(message, SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
}

void SendReceiveMessageB() {
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Bcast(message, SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
}

void ReceiveMessage(int* source){
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Status status;
  MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,&status);
  *source = status.MPI_SOURCE;
  MPI_Recv(message, SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (!rank) {
    double start = MPI_Wtime();
    sprintf(message, "Just a simple message");
    /* SendMessage(size); */
    SendReceiveMessageB();
    double end = MPI_Wtime() - start;
    printf("Elapsed time %f\n", end);
  }
  else {
    int source = 0;
    /* ReceiveMessage(&source); */
    SendReceiveMessageB();
    printf("[PROCESS #%d] Message from #%d : %s\n", rank, source, message);
  }
  MPI_Finalize();
  return 0;
}
