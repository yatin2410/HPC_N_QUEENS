#include <bits/stdc++.h>
#include <omp.h>

using namespace std;


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
        #pragma omp critical
        arr[omp_get_thread_num()]++;
  }
  else if(row==size-1 && col1==(size/2)){
        #pragma omp critical
        nrOdd++;
  }
  else {
        for(int i=0; i<size; i++) {
            #pragma omp task
            setQueen(queens, row+1, i, id,col1);
        }
  }
}

void solve(int p) {
    int myid=0 ;
    omp_set_num_threads(16);

    if(size%2==0){

            for(int i=0; i<(size/2); i++) {
        #pragma omp parallel
        #pragma omp single
        {

              setQueen(new int[size], 0, i, myid,i);
        }
        }
    }
    else{
            for(int i=0; i<=(size/2); i++) {
        #pragma omp parallel
        #pragma omp single
        {
              setQueen(new int[size], 0, i, myid,i);
            }     
    }
    }
}



int main(int argc, char*argv[]) {

    int queens = 4;
    cout<<"Enter Queens: ";
    cin>>queens;
    size = queens;
    for(int i=0;i<50;i++){
      arr[i] = 0;
    }
    double sTime = omp_get_wtime();
    solve(2);
    double eTime = omp_get_wtime();
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
    cout<<"total threads are: "<<mn<<endl;
    cout<<"total configurations are : "<<ans <<endl;
    cout<<"Time taken for execusion is :"<<(eTime-sTime)<<endl;    
    return 0;
}