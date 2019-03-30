#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int nrOfSolutions=0;
int size=0;
int LEVEL = 4;
#define N size

bool stillLegal(int *board,int row,int col){
    for(int i=0; i<row; i++) {
    // vertical attacks
    if (board[i]==col) {
      return false;
    }
    // diagonal attacks
    if (abs(board[i]-col) == (row-i) ) {
      return false;
    }
  }
    return true;
}

void NQueens(int *board, int row)
{
  if (row == N) 
   #pragma omp critical
    nrOfSolutions++;
  else {
    for (int i = 0; i < N; ++i) {
      board[row] = i;
      if (stillLegal(board, row,i))
        NQueens(board, row+1);
    }
  }
  return;
}

void NQueensD3(int *board, int row)
{
  for (int i = 0; i < N; ++i) {
    board[row] = i;
    if (stillLegal(board, row,i)){
      if (row < LEVEL && N>1 && N<=7) {
        int *bnew = new int[N];
        for (int j = 0; j <= row; ++j) bnew[j] = board[j];
        #pragma omp task 
            NQueensD3(bnew, row+1);  // generate a task
      }
      else
        NQueens(board, row+1);   // no more tasks, search from here
    }
  }
  return;
}

void solve() {
    int myid=0 ;
    omp_set_num_threads(2);
    #pragma omp parallel
    #pragma omp single
    {
        NQueensD3(new int[size],0);        
    }
}

int main(int argc, char*argv[]) {

    int queens = 4;
    cout<<"Enter Queens: ";
    cin>>queens;
    size = queens;
    double sTime = omp_get_wtime();
    solve();
    cout<<"total configurations are : "<<nrOfSolutions <<endl;
    double eTime = omp_get_wtime();
    cout<<"Time taken for execusion is :"<<(eTime-sTime)<<endl;    
    return 0;
}