#include <bits/stdc++.h>
#include <stdio.h>
#include <omp.h>

using namespace std;
int dfs(int lv, int row, int dia1, int dia2, int mask) {
    if (row == mask)
        return 1;
    int pos = mask & (~(row | dia1 | dia2)) , p;
    int ret = 0;
    while (pos) {
        p = pos & (-pos);
        pos -= p;
        ret += dfs(lv+1, row|p, (dia1|p)<<1, (dia2|p)>>1, mask);
    }
    return ret;
}
int totalNQueens(int n) {
    int sum = 0;
    int chunk = 1;
    int row = 0, dia1 = 0, dia2 = 0, mask = (1<<n)-1, lv = 0;
    int pos = mask & (~(row | dia1 | dia2)), p;
    omp_set_num_threads(2);
    #pragma omp parallel for schedule(dynamic, chunk) reduction(+:sum)
    for (int i = 0; i < n; i++) {
        if ((pos>>i)&1) {
            p = 1<<i;
            int t = dfs(lv+1, row|p, (dia1|p)<<1, (dia2|p)>>1, mask);
            sum += t;
        }
    }
    return sum;
}

int main() {
    int queens = 4;
    cout<<"Enter Queens: ";
    cin>>queens;
    int size = queens;
    double sTime = omp_get_wtime();
    int nr= totalNQueens(size);
    cout<<"total configurations are : "<<nr <<endl;
    double eTime = omp_get_wtime();
    cout<<"Time taken for execusion is :"<<(eTime-sTime)<<endl;    
    return 0;
    return 0;
}