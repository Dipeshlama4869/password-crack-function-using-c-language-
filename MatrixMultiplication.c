#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define MAT_SIZE 10
#define MAX_THREADS 10

int N,P,M;
  int i, j, k;
  int A[MAT_SIZE][MAT_SIZE], B[MAT_SIZE][MAT_SIZE], C[MAT_SIZE][MAT_SIZE];

//type defining for passing function arguments
typedef struct parameters{
  int x, y;
  
}args;

//function to calculate each element in result matrix used by threads
void *mult(void *arg){
  args* p = arg;
  
  //calculate each element in result matrix using passed arguments
  for(int a=0; a<P; a++){
    C[p->x][p->y] += A[p->x][a] * B[a][p->y];
  }
  sleep(3);
  
  //end of thread
  pthread_exit(0);
  
}

int main(){

  clock_t start, stop;
  double t=0.0;
  
  //initializing all defined matrices by zero
  for(int x=0;x<10;x++){
    for(int y=0;y<10;y++){
      A[x][y]=0;
      B[x][y]=0;
      C[x][y]=0;
    }
  }
  
  //getting matrix from user
  
  printf("\nEnter the size first of matrix\n");
  scanf("%d%d",&N,&P);
  
  for(int x=0;x<N;x++){
    for(int y=0;y<P;y++){
      printf("Enter variable [%d,%d]: ",x+1,y+1);
      scanf("%d",&A[x][y]);
    }
  }
  
  printf("\nEnter the size second of matrix\n");
  scanf("%d%d",&P,&M);
  
  for(int x=0;x<P;x++){
    for(int y=0;y<M;y++){
      printf("Enter variable [%d,%d]: ",x+1,y+1);
      scanf("%d",&B[x][y]);
    }
  }

  //printing matrices
  printf("\n----matrix A-----\n\n");
  for(int x=0;x<N;x++){
    for(int y=0;y<P;y++){
      printf("%15d",A[x][y]);
    }
    printf("\n\n");
  }
  
  printf("\n----matrix B-----\n\n");
  for(int x=0;x<N;x++){
    for(int y=0;y<P;y++){
      printf("%15d",A[x][y]);
    }
    printf("\n\n");
  }

  //multiply matrices using threads
  
  //defining threads
  pthread_t thread[MAX_THREADS];
  
  //counter for thread index
  int thread_number = 0;
  
  //defining p for passing parameters to function as struct
  args p[N*M];
  
  //start timer
  start = clock();
  assert(start != -1);
  
  for(int x=0;x<N;x++){
    for(int y=0;y<M;y++){
      //initializing parameters for passing to function
      p[thread_number].x = x;
      p[thread_number].y = y;
      
      //status for checking errors
      int status;
      
      //create specific thread for each element in result matrix
      status = pthread_create(&thread[thread_number],NULL,mult,(void *)&p[thread_number]);
      
      //check for error
      if(status!=0){
         printf("Error in threads");
         exit(0);
      }
      
      thread_number++;
      
    }
  }
  
  //wait for all threads done
  
  for(int z=0;z<(N*M);z++){
    pthread_join(thread[z],NULL);
  }
  
  //print multiplied matrix
  
  printf("----matrix C----\n\n");
  for(int x=0;x<N;x++){
    for(int y=0;y<M;y++){
      printf("%15d",C[x][y]);
    }
    printf("\n\n");
  }
  
  //Calculate Total Time Including 3 Soconds Sleep In Each Thread - - - -//
   stop = clock();
   t = (double)(stop-start)/CLOCKS_PER_SEC;
   printf("Run time: %f\n", t); 
  //printf(" ---> Time Elapsed : %.2f Sec\n\n", (double)(time(NULL) - start));
    
  //Total Threads Used In Process - - - - - - - - - - - - - - - - - - - -//
    
    printf(" ---> Used Threads : %d \n\n",thread_number);
    for(int z=0;z<thread_number;z++)
        printf(" - Thread %d ID : %d\n",z+1,(int)thread[z]);
    
    return 0;  

}
