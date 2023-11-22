#include <bits/stdc++.h>
#include <mpi.h>
#include <chrono>

// Pi calculation using liebniz formula with MAX_ITER iterations
// using parallel computing with MPI

using namespace std;

const int MAX_PROCESS = 10; 
const int MAX_ITER = 200000000;


int main(int argc, char** argv){

    MPI_Status status;
    MPI_Request req[MAX_PROCESS];
    int root = 0;

    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int numProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    double acum = 0.0;
    double pi = 0.0;
    int start = MAX_ITER / numProcesses * rank;
    int next = MAX_ITER / numProcesses * (rank + 1);

    for(int i = start; i < next; i++){
        double val = 1.0 / (2 * i + 1);
        if(i % 2 == 0)
            acum += val;
        else
            acum -= val;
    }

    MPI_Reduce(&acum, &pi, 1, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);

    if(rank == root){
        pi *= 4;
        cout << "Pi: " << pi << endl;
    }
        
    return 0;
}