//
// Created by misash on 09/10/22.
//

#include <iostream>
#include "mpi/mpi.h"


using namespace std;

int main(int argc , char * argv[]){

    int rank,size,msg;
    MPI_Status estado;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(rank < size - 1){
        //envio el rank
        MPI_Send(&rank,1, MPI_INT,rank+1,0,MPI_COMM_WORLD);
    }

    if(rank > 0) {
        //recv
        MPI_Recv(&msg, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &estado);
        cout<<"Soy el proceso "<<rank<<" y he recibido "<<msg<<endl;
    }

    MPI_Finalize();

}