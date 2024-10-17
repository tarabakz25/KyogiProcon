/*
基本インデントは4マス。
折り返しはなし
*/
#include <bits/stdc++.h>

// main関数は{を下に下げる
int main()
{
    int a = 0;
    int b = 1;

    //if文は空白を作らないで、
    if(a > b){
        cout << "aa" << endl;
        return 0;
    }else cout << "bb" << endl; // 一列の場合はこのようにする

    return 0;
}