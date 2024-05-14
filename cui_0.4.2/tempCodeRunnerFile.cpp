#include <bits/stdc++.h>
#include <chrono>
#include <unistd.h>
#include <algorithm>
#include <math.h>

#include <string> //read&write
#include <fstream>
#include <sstream>


#include "board_setting_0.4.1.cpp"
#include "evaluation_0.2.0.cpp"

#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

vector<vector<double>> choices(100000, vector<double>(6));
vector<int> ranking(100,{-1});  //top10, depth5