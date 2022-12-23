#include <mpi.h>
#include <iostream>
using namespace std;

void SendMessageR(int dest, char* message, MPI_Request* req, MPI_Status* stat) {
  MPI_Rsend(message, 5, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
  MPI_Wait(req,stat);
}

void SendMessageIR(int dest, char* message, MPI_Request* req, MPI_Status* stat) {
  MPI_Irsend(message, 5, MPI_CHAR, dest, 0, MPI_COMM_WORLD, req);
  MPI_Wait(req, stat);
}

void ReceiveMessage(MPI_Request* req, MPI_Status* stat) {
  char* buf = new char[5];
  int receive = MPI_Irecv(buf, 5, MPI_CHAR,MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, req);
  MPI_Wait(req,stat);
  cout << "Message from proccess " << stat->MPI_SOURCE << ":" << buf << endl;
  delete[] buf;
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  char pingBuf[] = "Ping";
  char pongBuf[] = "Pong";
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Status status;
  MPI_Request request;

  if (size != 2)
  {
    if (rank == 0) {
      printf("For executing this program you need only 2 ranks,in your case you tried to use %d! Exiting....\n", size);
    }
    MPI_Finalize();
    exit(0);
  }

  double start_time = MPI_Wtime();
  double current_time = 0;

  if (rank) {
    while ((current_time - start_time) < 4.1)
    {
      current_time = MPI_Wtime();
      int flag = 0;
      MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&flag,&status);
      if (flag)
      {
        ReceiveMessage(&request, &status);
        /* SendMessageR(0, pingBuf, &request, &status); */
        SendMessageIR(0, pingBuf, &request, &status);
      }
    }
  }
  else {
    while ((current_time - start_time) < 4.1)
    {
      current_time = MPI_Wtime();
      /* SendMessageR(1, pongBuf, &request, &status); */
      SendMessageIR(1, pongBuf, &request, &status);
      ReceiveMessage(&request, &status);
    }
  }

  MPI_Finalize();
  return 0;
}
