#include <bits/stdc++.h>
using namespace std;

int main()
{
    map<pair<int, int>, int> arr;
    
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(i == 2 && j == 2) arr[{i, j}] = 1;
            else arr[{i, j}] = 0;
        }
    }

for (const auto& entry : arr) {
    if (entry.second == 1) {
        cout << "Coordinates with value 1: (" << entry.first.first << ", " << entry.first.second << ")" << endl;
    }
}
}