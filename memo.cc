#include <bits/stdc++.h>
#define rep(i, n) for(long long i = 0; i < n; i++)
#define rep1(i, a, n) for(long long i = a; i < n; i++)
#define rep2(i, a, n, b) for(long long i = a; i < n; i += b)
#define nrep(i, n) for(long long i = n; i > 0; i--)
using namespace std;
typedef long long ll;


vector<int> number{256, 128, 64, 32, 16, 8, 4, 2, 1};

int main()
{
    int pivot = 0;
    rep(xi, number.size()){
        rep1(xj, xi + 1, number.size()){
            pivot = number[xi] + number[xj];
            cout << pivot << " ";
        }
        cout << endl;
    }
    return 0; 
}