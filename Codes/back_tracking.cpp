#include <bits/stdc++.h>
#include <omp.h>

using namespace std;



long long int nrOfSolutions=0;
int size=0;

void setQueen(int queens[], int row, int col, int id) {
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
        nrOfSolutions++;
  }
  else {
    for(int i=0; i<size; i++) {
      setQueen(queens, row+1, i, id);
    }
  }
}

void solve() {
    int myid=0 ;
		if(size%2==0){
			  for(int i=0; i<size/2; i++) {
        	setQueen(new int[size], 0, i, myid);
        }
				nrOfSolutions *= 2;
		}
		else{
			for(int i=0; i<(size/2); i++) {
        setQueen(new int[size], 0, i, myid);
			}
			nrOfSolutions *= 2;
			setQueen(new int[size], 0, (size)/2, myid);
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