/****************************************************************************
  This program does matrix multiplication with multithreading (POSIX).
  
  Compile with:

    cc -o Task2.2 Task2.2.c time_diff.c -lrt -pthread 

  Dipesh lama, University of Wolverhampton, 2020 
*****************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define MATRIX_SIZE 1200

int N = MATRIX_SIZE,
    P = MATRIX_SIZE,
    M = MATRIX_SIZE;

int A[MATRIX_SIZE][MATRIX_SIZE],
    B[MATRIX_SIZE][MATRIX_SIZE],
    C[MATRIX_SIZE][MATRIX_SIZE];

int thread_load; // amount of task a single thread would do.

//Core multiplication logic.
void *multiplyMatrices(void *arg)
{
    int *start = arg;

    int from = ((*start - 1) * thread_load); // jumping directly in mid index.

    for (int i = from; i < N; i += (N / thread_load))
    {
        for (int j = 0; j < M; j++)
        {
            for (int k = 0; k < P; k++)
            {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(0);
}

void printMatrices()
{

    printf("\nMatrix A: \n");
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < P; y++)
        {
            printf("%d ", A[x][y]);
        }
        printf("\n");
    }

    printf("\nMatrix B: \n");
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < P; y++)
        {
            printf("%d ", A[x][y]);
        }
        printf("\n");
    }

    printf("Matrix C (A X B) = \n");
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < M; y++)
        {
            printf("%d ", C[x][y]);
        }
        printf("\n");
    }
}

long long int evaluateMult(int thread_count)
{
    struct timespec start, finish;
    long long int difference;

    //defining threads
    pthread_t thread[thread_count];

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int x = 0; x < thread_count; x++)
    {
        int error;

        //Creating threads for each to be calculated thread load sections.
        error = pthread_create(&thread[x], NULL, multiplyMatrices, (void *)&x);

        if (error != 0)
        {
            printf("Error while initializing");
            exit(0);
        }
    }

    //Waiting for thread teminations.
    for (int z = 0; z < (thread_count); z++)
    {
        pthread_join(thread[z], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    time_difference(&start, &finish, &difference);
    printf("Program took %lldns / %9.5lfs\n", difference, difference / 1000000000.0);

    return difference;
}

int main()
{

    //Initializing all defined matrices by random numbers
    for (int x = 0; x < MATRIX_SIZE; x++)
    {
        for (int y = 0; y < MATRIX_SIZE; y++)
        {
            A[x][y] = 2;
            B[x][y] = 3;
            C[x][y] = 0;
        }
    }

    int thread_count;

    printf("Enter thread count: ");
    scanf("%d", &thread_count);

    thread_load = (int)N / thread_count;

    evaluateMult(thread_count);

    // printMatrices();

    printf("Using %d threads.\n", thread_count);

    int shouldBenchmark;
    printf("Want to average over multiple runs (1/0)?");
    scanf("%d", &shouldBenchmark);

    if (shouldBenchmark == 1)
    {
        long long int totalTime = 0;

        for (int p = 0; p < 10; p++)
        {
            totalTime += evaluateMult(thread_count);
        }

        printf("Average run time for %d threads is %lldns / %9.5lfs\n", thread_count, totalTime / 10, totalTime / 10000000000.0);
    }

    return 0;
}
