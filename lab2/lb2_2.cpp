#include <mpi.h>
#include <iostream>

int rank, size, messageInt;

void SendMessageInt(int dest, int* message, MPI_Request* req, MPI_Status* stat) {
  MPI_Isend(message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD, req);
  MPI_Wait(req, stat);
}

void RecieveMessageInt(MPI_Request* req, MPI_Status* stat) {
  int receive = MPI_Irecv(&messageInt, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, req);
  MPI_Wait(req, stat);
  std::cout << "[PROCESS#" << rank << "] Message from process " << stat->MPI_SOURCE << ": " << messageInt << std::endl;
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  MPI_Request request;
  MPI_Status stat;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int sendto = rank + 1 == size ? 0 : rank + 1;

  while (size > 3) {
    if (rank == 0) {
      messageInt = 0;
      SendMessageInt(sendto, &messageInt, &request, &stat);
      RecieveMessageInt(&request, &stat);
    } else if (rank == 4)
    {
      messageInt = 0;
      RecieveMessageInt(&request, &stat);
      SendMessageInt(sendto, &messageInt, &request, &stat);
      size--;
    }
    else {
      RecieveMessageInt(&request, &stat);
      messageInt++;
      SendMessageInt(sendto, &messageInt, &request, &stat);
    }
  }

  MPI_Finalize();
  return 0;
}
