//
// Created by misash on 09/10/22.
//

#include "mpi/mpi.h"
#include <iostream>

//mpic++ -g -Wall -o ejecutable HelloWorld.cpp
// mpirun -n 5 ./ejecutable

using namespace  std;


int main(int argc, char * argv[]){

    int rank , size ;

    MPI_Init(&argc , &argv); //inicializacion del entorno MPI
    MPI_Comm_size(MPI_COMM_WORLD,&size); // get processor number
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // get id de procesos

    cout<<"\nHola Mundo desde el proceso "<<rank<<" de "<<size<<" que somos!\n";

    MPI_Finalize();


}