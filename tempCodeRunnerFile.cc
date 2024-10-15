#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;

    vector<vector<char>> arr(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> arr[i][j];
    }

    for(int a = 0; a < n/2; a ++){
        for(int i = 0; i < n; i++){
            for(int j = 0l; j < n; j++){
                swap(arr[j][i], arr[i][n + 1 - j]);
            }
        }
    }

    for(int  i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << arr[i][j];
        }
        cout << endl;
    }
    
}