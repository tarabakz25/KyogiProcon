#include <bits/stdc++.h>
#define rep(i, n) for(long long i = 0; i < n; i++)
#define rep1(i, a, n) for(long long i = a; i < n; i++)
#define rep2(i, a, n, b) for(long long i = a; i < n; i += b)
#define nrep(i, n) for(long long i = n; i > 0; i--)
using namespace std;
typedef long long ll;

using vec = vector<vector<int>>;

vector<int> numbers;
vector<int> number1{256, 128, 64, 32, 16, 8, 4, 2, 1};
vector<int> number2{192, 160, 144, 136, 132, 130, 129, 96, 80, 72, 68, 66, 65, 48, 40, 36, 34, 33, 24, 20, 18, 17, 12, 10, 9, 6, 5, 3};
vector<int> number3{224, 208, 200, 196, 194, 193, 192, 176, 168, 164, 162, 161, 160, 152, 148, 146, 145, 144, 140, 138, 137, 136, 134, 133, 132, 131, 130, 129, 128, 112, 104, 100, 98, 97, 96, 88, 84, 82, 81, 80, 76, 74, 73, 72, 70, 69, 68, 67, 66, 65, 64, 56, 52, 50, 49, 48, 44, 42, 41, 40, 38, 37, 36, 35, 34, 33, 32, 28, 26, 25, 24, 22, 21, 20, 19, 18, 17, 16, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4};
vector<int> number4{240, 232, 228, 226, 225, 224, 216, 212, 210, 209, 208, 204, 202, 201, 200, 198, 197, 196, 195, 194, 193, 192, 184, 180, 178, 177, 176, 172, 170, 169, 168, 166, 165, 164, 163, 162, 161, 160, 156, 154, 153, 152, 150, 149, 148, 147, 146, 145, 144, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 120, 116, 114, 113, 112, 108, 106, 105, 104, 102, 101, 100, 99, 98, 97, 96, 92, 90, 89, 88, 86, 85, 84, 83, 82, 81, 80, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 60, 58, 57, 56, 54, 53, 52, 51, 50, 49, 48, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5};

int main()
{
    int pivot = 0;
    rep(zi, number3.size()){
        rep(xi, number1.size()){
            pivot = number3[zi] + number1[xi];
            if(pivot < 256) numbers.push_back(pivot);
        }
    }

    sort(numbers.begin(), numbers.end(), [](auto const &l, auto const &r){ return l > r; });
    auto new_end = unique(numbers.begin(), numbers.end());
    numbers.erase(new_end, numbers.end());

    for(int di : numbers){
        cout << di << ", ";
    }   
    cout << endl;


    return 0; 
}