#include <bits/stdc++.h>
#include <chrono>
#include <unistd.h>
#include <algorithm>
#include <math.h>

#include <string> //read&write
#include <fstream>
#include <sstream>


#include "board_setting.cpp"
#include "evaluation.cpp"



vector<vector<double>> choices(100000, vector<double>(6));
vector<int> ranking(100,{-1});  //top10, depth5