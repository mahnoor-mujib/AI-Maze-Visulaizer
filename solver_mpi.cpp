#include "maze.h"
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        cout << "MPI running with " << size << " processes\n";
    }

    cout << "Hello from process " << rank << endl;

    MPI_Finalize();
    return 0;
}