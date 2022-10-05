

#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

// usar: ./matrix-vector <cantidad de theads> <m> <n>


using namespace std;

/* Global variables */
int     thread_count;
int     m, n;
double* A;
double* x;
double* y;

/* Funciones en Serie */
void Usage(char* prog_name);
void Gen_matrix(double A[], int m, int n);
void Read_matrix(char* prompt, double A[], int m, int n);
void Gen_vector(double x[], int n);
void Read_vector(char* prompt, double x[], int n);
void Print_matrix(char* title, double A[], int m, int n);
void Print_vector(char* title, double y[], double m);

/* Funciones en Paralelo */
void *Pth_mat_vect(void* rank);

/*------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    long       thread;
    pthread_t* thread_handles;

    if (argc != 4) Usage(argv[0]);
    thread_count = strtol(argv[1], NULL, 10);
    m = strtol(argv[2], NULL, 10);
    n = strtol(argv[3], NULL, 10);

#  ifdef DEBUG
    printf("Cantidad de thread =  %d, m = %d, n = %d\n", thread_count, m, n);
#  endif


    long memSize = sizeof(float)*m*n + 2*sizeof(float)*n;
    long maxSize = 5L*1024*1024*1024; // 5 GiB limite(evitar que arriba de este valor se dirija al disco)
    /*
      Procure dejar un espacio de 2 a 3 GiB libres de su ram,
      para el uso del sistema.
    */

    if (memSize > maxSize) {
        fprintf(stderr,"ERROR: Error Intentando alojar un array de \n %ld bytes > Tamanyo Maximo = %ld bytes. Abortado.\n", memSize, maxSize);
        fprintf(stderr,"Cambiar maxSize en el codigo para sobre escribir\n");
        exit(-1);
    }

    thread_handles = static_cast<pthread_t *>(malloc(thread_count * sizeof(pthread_t)));
    A = static_cast<double *>(malloc(m * n * sizeof(double)));
    x = static_cast<double *>(malloc(n * sizeof(double)));
    y = static_cast<double *>(malloc(m * sizeof(double)));

    Gen_matrix(A, m, n);
#  ifdef DEBUG
    Print_matrix("Generamos", A, m, n);
#  endif

    Gen_vector(x, n);
#  ifdef DEBUG
    Print_vector("Generamos", x, n);
#  endif

/****** Inicio Seccion en Paralelo *************************/
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       Pth_mat_vect, (void*) thread);

    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
/****** Finc Seccion en Paralelo *************************/

#  ifdef DEBUG
    Print_vector("El producto es ", y, m);
#  endif

    free(A);
    free(x);
    free(y);

    return 0;
}  /* main */


/*------------------------------------------------------------------ */
void Usage (char* prog_name) {
    fprintf(stderr, "usar: %s <cantidad de theads> <m> <n>\n", prog_name);
    exit(0);
}  /* Usage */

/*------------------------------------------------------------------ */
void Read_matrix(char* prompt, double A[], int m, int n) {
    int             i, j;

    printf("%s\n", prompt);
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            scanf("%lf", &A[i*n+j]);
}  /* Read_matrix */

/*------------------------------------------------------------------ */
void Gen_matrix(double A[], int m, int n) {
    int i, j;
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            A[i*n+j] = random()/((double) RAND_MAX);
}  /* Gen_matrix */

/*------------------------------------------------------------------
*/
void Gen_vector(double x[], int n) {
    int i;
    for (i = 0; i < n; i++)
        x[i] = random()/((double) RAND_MAX);
}  /* Gen_vector */

/*------------------------------------------------------------------*/
void Read_vector(char* prompt, double x[], int n) {
    int   i;

    printf("%s\n", prompt);
    for (i = 0; i < n; i++)
        scanf("%lf", &x[i]);
}  /* Read_vector */


/*------------------------------------------------------------------
 * Funcion:       Pth_mat_vect
 * Proposito:     Multiplicar una matriz de mxn por un vetor columna de nx1
 * Argumentos de Entrada:         rank
 * Variables Globales: A, x, m, n, thread_count
 * Salida, variable global : y
 */
void *Pth_mat_vect(void* rank) {
    long my_rank = (long) rank;
    int i;
    int j;
    int local_m = m/thread_count;
    int my_first_row = my_rank*local_m;
    int my_last_row = my_first_row + local_m;
    register int sub = my_first_row*n;
    double start, finish;
    double temp;

#  ifdef DEBUG
    printf("Thread %ld > local_m = %d, sub = %d\n",
         my_rank, local_m, sub);
#  endif

    GET_TIME(start);
    for (i = my_first_row; i < my_last_row; i++) {
        y[i] = 0.0;
        for (j = 0; j < n; j++) {
            temp = A[sub++];
            temp *= x[j];
            y[i] += temp;
        }
    }
    GET_TIME(finish);
    printf("Thread %ld > Tiempo de enlace = %.15f segundos\n",
           my_rank, finish - start);

    return NULL;
}  /* Pth_mat_vect */


/*------------------------------------------------------------------
 * Funcion:    Print_matrix
 * Proposito:  Imprimir una Matriz
 * Argumentos de Entrada:     title, A, m, n
 */
void Print_matrix( char* title, double A[], int m, int n) {
    int   i, j;

    printf("%s\n", title);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++)
            printf("%6.3f ", A[i*n + j]);
        printf("\n");
    }
}  /* Print_matrix */


/*------------------------------------------------------------------
 * Funcion:    Print_vector
 * Proposito:     Imprimir un vector
 * Argumentos de Entrada:    Titulo, y, m
 */
void Print_vector(char* title, double y[], double m) {
    int   i;

    printf("%s\n", title);
    for (i = 0; i < m; i++)
        printf("%6.3f ", y[i]);
    printf("\n");
}  /* Print_vector */
