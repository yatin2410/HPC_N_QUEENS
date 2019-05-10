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


long long int nrOfSolutions=0;
int size=0;
int nrOdd = 0;
long long int arr[50];
int LEVEL = 1;

void setQueen(int queens[], int row, int col, int id, int col1) {
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
  if(row==size-1 && col1!=(size/2)) {
        arr[omp_get_thread_num()]++;
  }
  else if(row==size-1 && col1==(size/2)){
        nrOdd++;
  }
  else {
    for(int i=0; i<size; i++) {
      setQueen(queens, row+1, i, id,col1);
    }
  }
}

void setQueen1(int queens[], int row, int col, int id, int col1) {
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
  if(row==size-1 && col1!=(size/2)) {
        arr[omp_get_thread_num()]++;
  }
  else if(row==size-1 && col1==(size/2)){
        nrOdd++;
  }
  else {
    if(row<=LEVEL){
      for(int i=0; i<size; i++) {
          setQueen1(queens, row+1, i, id,col1);
      }
    }
    else{
        for(int i=0; i<size; i++) {
          setQueen1(queens, row+1, i, id,col1);
      }
    }
  }
}

void solve() {
    int myid=0 ;
    if(size%2==0){
            for(int i=0; i<(size/2)-1; i++) {
              setQueen(new int[size], 0, i, myid,i);
            }     
            for(int i=(size/2)-1;i<(size/2);i++){
              setQueen1(new int[size], 0, i, myid,i);
            }

    }
    else{
            for(int i=0; i<(size/2); i++) {
              setQueen(new int[size], 0, i, myid,i);
            }     
            for(int i=(size/2);i<=(size/2);i++){
              setQueen1(new int[size], 0, i, myid,i);
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
	char *approach_name="design1";
	FILE* outputFile;
	//inputFile = fopen(argv[3],"r");

	char outputFileName[50];		
	sprintf(outputFileName,"output/%s_%s_%s_%s_output.txt",problem_name,approach_name,argv[1],argv[2]);	

	/*----------------------Core algorithm starts here----------------------------------------------*/

    for(int i=0;i<50;i++){
      arr[i] = 0;
    }


	clock_gettime(CLK, &start_alg);	/* Start the algo timer */
	size = n;
    solve();

	/*----------------------Core algorithm finished--------------------------------------------------*/
	
	clock_gettime(CLK, &end_alg);	/* End the algo timer */
	/* Ensure that only the algorithm is present between these two
	   timers. Further, the whole algorithm should be present. */

	long long int ans = 0;
    int mn = -1;
    for(int i=0;i<50;i++){
      if(arr[i]!=0){
        mn = max(mn,i);
      }
      nrOfSolutions += arr[i];
    }
    if(size%2==0){
        ans = nrOfSolutions*2;
    }
    else{
      ans = (nrOfSolutions*2)+nrOdd;
    }
	// cout<<"ans: "<<ans<<endl;
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
