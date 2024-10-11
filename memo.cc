#include <bits/stdc++.h>
using namespace std;

vector<int> gene_only{1, 2, 4, 8, 16, 32, 64, 128, 256};

int main()
{
    int n;
    cin >> n;

    for(int i : gene_only){
        cout << 3 * (log2(i) - 1) + 1 << endl;
    }

    exit(0);
}