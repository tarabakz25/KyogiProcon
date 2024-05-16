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


using namespace std;


int main(){
    string input_csv_file_path = "";
    string output_csv_file_path = "test.csv";

    ifstream ifs_csv_file(input_csv_file_path  );

    ofstream outputfile(output_csv_file_path );
    outputfile << "move";
    outputfile << ",";
    outputfile << "score";
    outputfile << ",";
    outputfile << "match";
    outputfile << ",";
    outputfile << "\n";

    board_set(); //初期状態の設定と表示

    cout << "かみやまひらがなこうせんは　たまねぎを　はなった！" << endl;
    cout << "＞＞「くらえッ　タマネギ！」" << endl;
    board_save(0);

    //実行時間計測用
    std::chrono::system_clock::time_point  start, end; 
    start = std::chrono::system_clock::now(); // 計測開始時間
    //ここまで


    //型抜き
    /*------------------------------------------------------------------------------------------------------
    | katanuki関数で型抜きを行えます。                                                                          |
    |                                                                                                       |
    | katanuki(型番号, 型抜きする場所の左上のx座標, 型抜きする場所の左上のy座標, 詰める方向(0:上, 1:下, 2:左, 3:右));    |
    | で呼び出してください。                                                                                    |
    ------------------------------------------------------------------------------------------------------*/
    

    katanuki(6, 2, 2, 1); 
    show_board(1);
    cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
    MOVE++;

    katanuki(2, 5, 2, 3); 
    show_board(1);
    cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
    MOVE++;


    
    



    //実行時間計測用
    end = std::chrono::system_clock::now();  // 計測終了時間
    double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(); //処理に要した時間をマイクロ秒に変換
    cout << elapsed / 1000 << "ms" << "\n";
    //ここまで
}