//
// Created by misash on 09/10/22.
//


#include "mpi/mpi.h"
#include <iostream>

//mpic++ -g -Wall -o ejecutable HelloWorld.cpp
// mpirun -n 5 ./ejecutable

using namespace  std;


int main(int argc, char * argv[]){

   int rank,contador;
   MPI_Status estado;

   MPI_Init(&argc,&argv); //inicializamos la comunicacion procesos

   MPI_Comm_rank(MPI_COMM_WORLD,&rank); // obtener numero procesos

   //enviar y recivir mensajes
   // E , N , T , ID , ET , COM

   MPI_Send(&rank //mensaje
            ,1 // numero de elems a enviar
            ,MPI_INT //tipo de dato a enviar
            ,rank // id del destino
            ,0 // etiqueta
            ,MPI_COMM_WORLD // comunicador
            );

    MPI_Recv(&contador  //donde lo almacenara
             ,1 // tamaño del mensaje
             ,MPI_INT // tipo de dato
             ,rank// source id
             ,0 // tag
             ,MPI_COMM_WORLD
             ,&estado //estado
             );

    cout<<"Soy el proceso "<<rank<<" y he recibido "<<contador<<endl;


    MPI_Finalize();

}