/*
gcc -o nqueen -O3 -fopenmp

Author: Wenjie Cai 2018.1.21
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

long sum = 0;
long n = 20;

void Solve(long row, long antiDiag, long mainDiag, long allOneMask)
{
    // long allOneMask = (1 << n) - 1; //set it as local var will speed up
    if (row != allOneMask){
        register long pos = allOneMask & ~(row | antiDiag | mainDiag);
        while (pos){
            long p = pos & -pos;
            pos -= p;
            Solve(row | p, (antiDiag | p) << 1, (mainDiag | p) >> 1, allOneMask);
        }
   }else{
        #pragma omp atomic
        sum ++;
   }
}

int main(int argc, char *argv[])
{
    n = atoi(argv[1]);

    time_t start_t;
    start_t = time(0);
    printf("Start time: \t %s\n", ctime(&start_t));

    int odd = n & 1;
    long half = n >> 1;

    long initPos = 1L << (n-1L);
    long allOneMask = (1 << n) - 1;
    long halfOneMask = allOneMask - ((1 << n/2 ) - 1);

    int i;

    // int ncore = omp_get_num_procs();
    #pragma omp parallel //num_threads(ncore)
    #pragma omp single
    for (i = 0; i <= half; ++i) 
    {
        long row, antiDiag, mainDiag, pos, mid;
        if(i < half){
            row = initPos >> i;
            antiDiag = initPos >> (i-1L);
            mainDiag = initPos >> (i+1L);
            pos = allOneMask & ~(row | antiDiag | mainDiag);
        }else{
            if(odd){
                mid = 1 << ( n >> 1);
                row = mid;
                antiDiag = mid << 1;
                mainDiag = mid >> 1;
                pos = halfOneMask & ~(row | antiDiag | mainDiag);
            }
        }
        while (pos){ //calculate position of 2nd row, so OpenMP can create more thread for CPU to use
            long p = pos & -pos;
            pos -= p;
            #pragma omp task
            Solve(row | p, (antiDiag | p) << 1, (mainDiag | p) >> 1, allOneMask);
        }
    }

    sum *= 2;
    printf("Found %ld solutions for %ld queens in %d secends.\n", sum, n, (int) (time(0) - start_t));
}