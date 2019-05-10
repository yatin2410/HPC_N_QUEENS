#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

long long int nrOfSolutions=0;
int size=0;
int nrOdd = 0;
long long int arr[500];
int LEVEL = 3;
int cnt[20] ={0};

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
        #pragma omp parallel
        #pragma omp single
        {
        #pragma omp task
          setQueen1(queens, row+1, i, id,col1);
        }
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
        #pragma omp parallel
        #pragma omp single
        {
            for(int i=0; i<(size/2)-3; i++) {
            #pragma omp task
              setQueen(new int[size], 0, i, myid,i);
            }     
            for(int i=(size/2)-3;i<(size/2);i++){
              setQueen1(new int[size], 0, i, myid,i);
            }
        }

    }
    else{
        #pragma omp parallel
        #pragma omp single
        {
            for(int i=0; i<=(size/2); i++) {
            #pragma omp task
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
    for(int i=0;i<500;i++){
      arr[i] = 0;
    }
    double sTime = omp_get_wtime();
    solve();
    double eTime = omp_get_wtime();
    long long int ans = 0;
    int mn = 1000;
    for(int i=0;i<500;i++){
      if(arr[i]==0){
        mn = min(mn,i);
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