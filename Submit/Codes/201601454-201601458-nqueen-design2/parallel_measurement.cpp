/* 
	HPC PROJECT- N QUEEN - Backtracking 
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


int nrOfSolutions=0;
int size=0;

void setQueen(int queens[], int row, int col, int id) {     // Same as serial just parallism is added.
  for(int i=0; i<row; i++) {
    // vertical attacks
    if (queens[i]==col) {
      return;
    }
    // diagonal attacks
    if (abs(queens[i]-col) == (row-i) ) {
      return;
    }
  }

  queens[row]=col;
  if(row==size-1) {
        #pragma omp critical
        nrOfSolutions++;
  }
  else {
    for(int i=0; i<size; i++) {
      setQueen(queens, row+1, i, id);
    }
  }
}

void solve(int p) {
    int myid=0 ;
    omp_set_num_threads(p);
    #pragma omp parallel					//Parallel block is added.
    #pragma omp single
    {
        for(int i=0; i<size; i++) {
        #pragma omp task						//New task is added for first row and each column recursion.
        setQueen(new int[size], 0, i, myid);
        }
    }
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
	int p=atoi(argv[2]);
	char *problem_name="nqueen";
	char *approach_name="design2";
	FILE* outputFile;
	//inputFile = fopen(argv[3],"r");

	char outputFileName[50];		
	sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);	

	/*----------------------Core algorithm starts here----------------------------------------------*/

	clock_gettime(CLK, &start_alg);	/* Start the algo timer */
	size = n;
    solve(p);

	/*----------------------Core algorithm finished--------------------------------------------------*/
	
	clock_gettime(CLK, &end_alg);	/* End the algo timer */
	/* Ensure that only the algorithm is present between these two
	   timers. Further, the whole algorithm should be present. */


	/* Should end before anything else (printing comes later) */
	clock_gettime(CLK, &end_e2e);
	e2e = diff(start_e2e, end_e2e);
	alg = diff(start_alg, end_alg);
//	outputFile = fopen(outputFileName,"w");
//	fprintf(outputFile,(char*)nrOfSolutions);		

	/* problem_name,approach_name,n,p,e2e_sec,e2e_nsec,alg_sec,alg_nsec
	   Change problem_name to whatever problem you've been assigned
	   Change approach_name to whatever approach has been assigned
	   p should be 0 for serial codes!! 
	 */
	printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, n, p, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);
	return 0;  
}
