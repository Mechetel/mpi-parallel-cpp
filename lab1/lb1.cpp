#include <mpi.h>
#include <iostream>
using namespace std;
int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char buf[] = "Hello world";
  MPI_Send(buf, sizeof(buf), MPI_CHAR, 0, 0, MPI_COMM_WORLD);

  for (int i = 0; i < size; i++) {
    MPI_Status s;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
    int count;
    MPI_Get_count(&s, MPI_CHAR, &count);
    char *buf = new char[count];
    MPI_Recv(buf, count, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
    if (s.MPI_SOURCE % 4 == 0) {
      cout << "Message from process " << s.MPI_SOURCE << ": " << buf << endl;
    }
    else {
      cout << "Message from process " << s.MPI_SOURCE << ": " << s.MPI_SOURCE << endl;
    }
    delete[] buf;
  }
  MPI_Finalize();
  return 0;
}
