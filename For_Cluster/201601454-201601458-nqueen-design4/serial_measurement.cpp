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

long long int ans = 0;

int  SIZE, MASK;

void fun(int x)
{
    vector<int> g;
    while(x!=0)
    {
        g.push_back(x%2);
        x=x/2;
    }
    reverse(g.begin(),g.end());
    for(int i=0;i<g.size();i++)
        cout<<g[i];
    cout<<endl;
}

void Backtrack(int y, int left, int down, int right)
{
    int  bitmap, bit;

    if (y == SIZE) 
    {
        ans++;
    }
    else 
    {
        int x=1<<4 - 1;
        /*if(left>x || right>x || down>x)
        {
            cout<<left<<" "<<right<<" "<<down<<endl;
        }*/
        bitmap = MASK & ~(left | down | right);
        for(int i=bitmap;i>0;)
        {
            bit = ((-i) & i);
            i ^= bit;
            Backtrack(y+1, (left | bit)<<1, down | bit, (right | bit)>>1);
        }
    }
}
void solve(int n){
    SIZE = n;
    MASK = (1 << SIZE) - 1;
    Backtrack(0,0,0,0);
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

	clock_gettime(CLK, &start_alg);	/* Start the algo timer */
	SIZE = n;
	solve(n);

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
	printf("%s,%s,%d,%d,%d,%ld,%d,%ld\n", problem_name, approach_name, n, pp, e2e.tv_sec, e2e.tv_nsec, alg.tv_sec, alg.tv_nsec);
	return 0;  
}
