#include <bits/stdc++.h>
//#include <omp.h>

using namespace std;

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

int main(){

    int queens = 4;
    cout<<"Enter Queens: ";
    cin>>queens;
   // double sTime = omp_get_wtime();
    solve(queens);
    ////cout<<"total configurations are : "<<ans<<endl;
    //double eTime = omp_get_wtime();
   // cout<<"Time taken for execusion is :"<<(eTime-sTime)<<endl;    
    cout<<ans<<endl;
    return 0;
}
