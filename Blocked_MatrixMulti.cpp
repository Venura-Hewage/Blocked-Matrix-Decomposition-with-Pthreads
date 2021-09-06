#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <pthread.h>
#include <stdio.h>
using namespace std::chrono;
using namespace std;
const int size = 2000;

#define num_of_pieces 100

 int a[size][size];
 int b[size][size];
 int c[size][size];


struct thread_data {

  int  id;
  int start;
  int end;
  int  **a;
  int  **b;
  int  **c;
 

};



void randomMatrix(int **array, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {

             array[i][j] = rand() % 100;
        }
       
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////
void *multiplyMatrix(void *arguments)
{

struct thread_data *args;
args=(struct thread_data *) arguments;


double sum;
int i, j, k, kk, jj;
int bsize = size / num_of_pieces;
int diff = (args->id)+ 1;
int sm = bsize*diff;
int start = sm -bsize;
kk = start;
jj = start;


for (kk; kk < sm; kk += bsize) {
 for (jj; jj < sm; jj += bsize) {
 for (i = 0; i < size; i++) {
     for (j = jj; j < jj + bsize; j++) {
    sum = args->c[i][j];
     for (k = kk; k < kk + bsize; k++) {
         sum += args->a[i][k]*args->b[k][j];
    }
 args->c[i][j] = sum;
 }
 }
 }


}




   return args;
    pthread_exit(NULL);


}

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){


    pthread_t threads[num_of_pieces];
    pthread_t thread,thread2;
    struct thread_data td[num_of_pieces];

  
  

    srand(time(0));

 
    int rc;

int** a = new int*[size];
for (int j = 0; j < size; ++j){
    a[j] = new int[size];
}

int** b = new int*[size];
for (int j = 0; j < size; ++j){
    b[j] = new int[size];
}

int** c = new int*[size];
for (int j = 0; j < size; ++j){
    c[j] = new int[size];
}



   randomMatrix(a, size);

   randomMatrix(b,size);




    //ToDo: Add Comment
for(int i=0;i<num_of_pieces;i++)
    {
        td[i].id =i;
        td[i].a = a;
        td[i].b =b;
        td[i].c =c;  
       
    }
  
 auto start = high_resolution_clock::now();
    for(int i=0;i<num_of_pieces;i++)
    {
        //cout << "main() : creating thread, " << i << endl;
         rc =pthread_create(&threads[i], NULL, *multiplyMatrix,(void *)&td[i]);     
        if(rc){
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    for (int i = 0; i < num_of_pieces; i = i + 1)
    {
        pthread_join(threads[i], NULL);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);


    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    
 


 


    return 0;

 



}