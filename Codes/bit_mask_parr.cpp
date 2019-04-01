#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

long long int ans = 0;

int  SIZE, MASK;

string fun(int n){
    string str = "";
    while(n!=0){
        char ch ;
        int y = n%2;
        n /= 2;
        ch = y+'0';
        str += ch;
    }
    for(int i=str.length();i<8;i++){
        str += '0';
    }
    reverse(str.begin(), str.end());
    return str; 
}

void Backtrack(int y, int left, int down, int right)
{
    // cout<<y<<" "<<fun(left)<<" "<<fun(down)<<" "<<fun(right)<<endl;
    int  bitmap, bit;
    if (y == SIZE)
    {
        #pragma omp critical
        ans++;
    } 
    else
    {
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
    omp_set_num_threads(2);
    #pragma omp parallel
    #pragma omp single
    {
        int y = 0,left = 0,right = 0,down = 0;
        int  bitmap, bit;
        MASK = (1 << SIZE) - 1;
        bitmap = MASK & ~(left | down | right);
        while (bitmap) {
            bit = -bitmap & bitmap;
            bitmap ^= bit;
            #pragma omp task 
            Backtrack(y+1, (left | bit)<<1, down | bit, (right | bit)>>1);
        }
    }
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