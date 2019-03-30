#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

long long int ans = 0;

int  SIZE, MASK;

void Backtrack(int y, int left, int down, int right)
{
    int  bitmap, bit;

    if (y == SIZE) {
        ans++;
    } else {
        bitmap = MASK & ~(left | down | right);
        while (bitmap) {
            bit = -bitmap & bitmap;
            bitmap ^= bit;
            Backtrack(y+1, (left | bit)<<1, down | bit, (right | bit)>>1);
        }
    }
}

void solve(int n){
    SIZE = n;
    MASK = (1 << SIZE) - 1;
    Backtrack(0,0,0,0);
}

int main(){

    int queens = 4;
    cout<<"Enter Queens: ";
    cin>>queens;
    double sTime = omp_get_wtime();
    solve(queens);
    cout<<"total configurations are : "<<ans<<endl;
    double eTime = omp_get_wtime();
    cout<<"Time taken for execusion is :"<<(eTime-sTime)<<endl;    

    return 0;
}