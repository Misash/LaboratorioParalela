//
// Created by misash on 9/21/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>



/* Function declarations */
void Get_input(int argc, char* argv[], int my_rank, double* n_p);
double Trap(double left_endpt, double right_endpt, int trap_count,
            double base_len);
double f(double x);

int main(int argc, char** argv) {
    int my_rank, comm_sz,n = 1024 , local_n;

    double a = 0.0 , b = 3.0 , h , local_a,local_b;
    double local_int , total_int ;
    double source ;



    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);




//    Get_input(argc, argv, my_rank, &n); /*Read user input */
//    Get_data(my_rank,comm_sz,&a,&b,&n);


    h = (b-a)/n;          /* length of each trapezoid */
    local_n = n/comm_sz;  /* number of trapezoids per process */

    /* Length of each process' interval of integration = local_n*h. */
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a , local_b , local_n , h);



    if (my_rank != 0) {
       MPI_Send(&local_int, 1 , MPI_DOUBLE , 0 , 0 , MPI_COMM_WORLD);
    }else{
        total_int = local_int;
        for (source = 1;source < comm_sz ; source++) {
            MPI_Recv(&local_int , 1 , MPI_DOUBLE , source , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
            total_int += local_int;
        }
    }

    if (my_rank == 0) {
        printf("With n = %.0d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %.15e\n",a, b, total_int);
    }

    MPI_Finalize();

    return 0;
} /*  main  */


void Get_input(int argc, char* argv[], int my_rank, double* n_p){
    if (my_rank == 0) {
        if (argc!= 2){
            fprintf(stderr, "usage: mpirun -np <N> %s <number of trapezoids> \n", argv[0]);
            fflush(stderr);
            *n_p = -1;
        } else {
            *n_p = atoi(argv[1]);
        }
    }
    // Broadcasts value of n to each process
    MPI_Bcast(n_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // negative n ends the program
    if (*n_p <= 0) {
        MPI_Finalize();
        exit(-1);
    }
}  /* Get_input */


double Trap(double left_endpt, double right_endpt, int trap_count, double base_len) {
    double estimate, x;
    int i;

    estimate = (f(left_endpt) + f(right_endpt))/2.0;
    for (i = 1; i <= trap_count-1; i++) {
        x = left_endpt + i*base_len;
        estimate += f(x);
    }
    estimate = estimate*base_len;

    return estimate;
} /*  Trap  */



double f(double x) {
    return x*x;
}