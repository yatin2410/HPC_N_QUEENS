/* 
  HPC PROJECT- N QUEEN - Backtracking + bitmasking
  Authors:- YatinPatel - 201601454
        Jaykishan Parmar - 201601458

*/


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

int size = 4;

int recur(int lv, int row, int left, int right, int mask)  // Same as serial just parallelism added.
{
    if (row == mask)
    {
        return 1;
    }
    int bitmap = mask & (~(row | left | right)) , p;
    int ret = 0;
    while (bitmap) 
    {
        p = bitmap & (-bitmap);
        bitmap -= p;
        ret += recur(lv+1, row|p, (left|p)<<1, (right|p)>>1, mask);
    }
    return ret;
}
int totalNQueens(int n,int pp) 
{
    int sum = 0;
    int chunk = 1;
    int row = 0, left = 0, right = 0, mask = (1<<n)-1, lv = 0;
    int bitmap = mask & (~(row | left | right)), p1,p2;
    for (int i = 0; i < n; i++) 
    {
        p1 = 1<<i;
        int t1=row|p1;
        int t2=(left|p1)<<1;
        int t3=(right|p1)>>1;
        int t4= mask & (~(t1 | t2 | t3));
        omp_set_num_threads(pp);
        #pragma omp parallel for schedule(dynamic, chunk) reduction(+:sum)  // we have come one row down now for reduciotn added.
        for(int j=0;j<n;j++)
        {
            if((t4>>j)&1)
            {
                p2=1<<j;
                int t = recur(2, t1|p2, (t2|p2)<<1, (t3|p2)>>1, mask);
                sum += t;
            }
        }
    }
    return sum;
}


void solve(int pp) 
{
        totalNQueens(size,pp);
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
    char *approach_name="design4";
    FILE* outputFile;
    //inputFile = fopen(argv[3],"r");

    char outputFileName[50];        
    sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]); 

    /*----------------------Core algorithm starts here----------------------------------------------*/

    clock_gettime(CLK, &start_alg); /* Start the algo timer */
    size = n;
    solve(pp);

    /*----------------------Core algorithm finished--------------------------------------------------*/
    
    clock_gettime(CLK, &end_alg);   /* End the algo timer */
    /* Ensure that only the algorithm is present between these two
       timers. Further, the whole algorithm should be present. */


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