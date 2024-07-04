#include <bits/stdc++.h>
using namespace std;

double Evaluation(int depth){ //ロードされている盤面を評価する。
    double score;

    //
    
    score = 100*(count_match()+0.000) / (board_size_width * board_size_height);

    if(compare_with0() == 100){ //前の手に比べて変更点がまったくなければ、評価を減らす
        score = score * 0;
    } 
    return score;
}
