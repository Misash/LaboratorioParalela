//
// Created by misash on 09/10/22.
//


#include "mpi/mpi.h"
#include <iostream>
#include <string>
#include <cstring>

// mpirun --oversubscribe  -n 8 ./ejecutable

using namespace std;
const int MAX_STRING = 100;


int main(){


    int ndays [7] = {1, 5, 2, 0, 3, 6, 4};
    // Day's name in order
    std::string sdays []={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};

    //MPI
    int size,rank;



    MPI_Status estado;
    MPI_Init(0,0);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);


    if(rank != 0) {

        int day = ndays[rank - 1];
        string msg = "Proc_" + to_string(rank) + " traducio " + to_string(day) +" en " + sdays[day];
        MPI_Send(msg.c_str(),msg.size() + 1,MPI_CHAR,0,0,MPI_COMM_WORLD);

    }else{

        char recv_msg[MAX_STRING];
        printf("Traducciones hechas por los %d procesadores y recibidas por el proc_%d \n"
                ,size ,rank);
        for (int source = 1; source < size; ++source) {
            MPI_Recv(recv_msg,MAX_STRING,MPI_CHAR,source,0,MPI_COMM_WORLD,&estado);
            cout<<"\n"<<recv_msg;
        }

    }

    MPI_Finalize();
    return 0;

}