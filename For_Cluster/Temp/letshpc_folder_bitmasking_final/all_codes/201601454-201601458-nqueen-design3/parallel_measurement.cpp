#include <bits/stdc++.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

//  Using the MONOTONIC clock 
#define CLK CLOCK_MONOTONIC

/* Function to compute the difference between two points in time */
struct timespec diff(struct timespec start, struct timespec end);
struct timespec diff(struct timespec start, struct timespec end){
    struct timespec temp;
    if((end.tv_nsec-start.tv_nsec)<0){
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    }
    else{
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

//-----------------------------ALGO STARTS------------------------//

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

void solve(int n,int pp){
    int odd = n & 1;
    long half = n >> 1;

    long initPos = 1L << (n-1L);
    long allOneMask = (1 << n) - 1;
    long halfOneMask = allOneMask - ((1 << n/2 ) - 1);

    int i;

    // int ncore = omp_get_num_procs();
    #pragma omp parallel num_threads(pp)
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
}


//-----------------------------ALGO ENDS------------------------//

int main(int argc, char* argv[])
{
    struct timespec start_e2e, end_e2e, start_alg, end_alg, e2e, alg;
    /* Should start before anything else */
    clock_gettime(CLK, &start_e2e);
    if(argc<3)
    {
        printf("Usage: %s n p \n",argv[0]);
        return -1;
    }
    int n=atoi(argv[1]);
    int pp=atoi(argv[2]);
    char *problem_name="nqueen";
    char *approach_name="design3";
    FILE* outputFile;
    //inputFile = fopen(argv[3],"r");

    char outputFileName[50];        
    sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]); 

    /*----------------------Core algorithm starts here----------------------------------------------*/

    clock_gettime(CLK, &start_alg); /* Start the algo timer */
    solve(n,pp);
    /*----------------------Core algorithm finished--------------------------------------------------*/
    
    clock_gettime(CLK, &end_alg);   /* End the algo timer */
    /* Ensure that only the algorithm is present between these two
       timers. Further, the whole algorithm should be present. */


    cout<<"ans: "<<sum;
    /* Should end before anything else (printing comes later) */
    clock_gettime(CLK, &end_e2e);
    e2e = diff(start_e2e, end_e2e);
    alg = diff(start_alg, end_alg);
//  outputFile = fopen(outputFileName,"w");
//  fprintf(outputFile,(char*)nrOfSolutions);       

    /* problem_name,approach_name,n,p,e2e_sec,e2e_nsec,alg_sec,alg_nsec
       Change problem_name to whatever problem you've been assigned
       Change approach_name to whatever approach has been assigned
       p should be 0 for serial codes!! 
     */
    printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, n, pp, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);
    return 0;  
}