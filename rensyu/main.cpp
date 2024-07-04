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
#include "json.hpp"

using namespace std;
using json = nlohmann::json;



using namespace std;

// 文字列からベクトルに変換する関数
vector<int> stringToVector(const string &str)
{
    vector<int> row;
    for (char c : str)
    {
        row.push_back(c - '0'); // 文字を整数に変換
    }
    return row;
}

// JSONからボードを読み込む関数
int loadBoard(const json &jobj, vector<vector<int>> &startBoard, vector<vector<int>> &goalBoard)
{
    for (const auto &line : jobj["board"]["start"])
    {
        startBoard.push_back(stringToVector(line.get<string>()));
    }
    for (const auto &line : jobj["board"]["goal"])
    {
        goalBoard.push_back(stringToVector(line.get<string>()));
    }
    return 0;
}


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

    // JSONファイルの読み込み
    ifstream ifs("./sample1.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json jobj = json::parse(str);
    vector<vector<int>> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard);
    int boardSide = jobj["board"]["width"].get<int>();
    int boardWarp = jobj["board"]["height"].get<int>();

	board = startBoard;
	finish_board = goalBoard;


    

    //board_set();
    show_setting_board();


    cout << "かみやまひらがなこうせんは　たまねぎを　はなった！" << endl;
    cout << "＞＞「くらえッ　タマネギ！」" << endl;
    board_save(0);

    //実行時間計測用
    std::chrono::system_clock::time_point  start, end; 
    start = std::chrono::system_clock::now(); // 計測開始時間
    //ここまで

    //t


    //型抜き
    /*------------------------------------------------------------------------------------------------------
    | katanuki関数で型抜きを行えます。                                                                          |
    |                                                                                                       |
    | katanuki(型番号, 型抜きする場所の左上のx座標, 型抜きする場所の左上のy座標, 詰める方向(0:上, 1:下, 2:左, 3:右));    |
    | で呼び出してください。                                                                                    |
    ------------------------------------------------------------------------------------------------------*/


    
/* 
    katanuki(0, 1, 1, 1); 
    show_board(1);
    cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
    MOVE++;

    katanuki(1, 5, 2, 3); 
    show_board(1);
    cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
    MOVE++;
 */

    

    while(1){
        int in_kata, in_x, in_y, in_direction_num;
        char in_direction;
        cout << "抜き型を選択してください" << endl;
        cin >> in_kata;
        cout << "適用するx,yを入力(x y)" << endl;
        cin >> in_x >> in_y;
        cout << "方向をwasdキーで入力してください" << endl;
        cin >> in_direction;

        switch (in_direction){
            case 'w':
                in_direction_num = 0;
                break;
            case 's':
                in_direction_num = 1;
                break;
            case 'a':
                in_direction_num = 2;
                break;
            case 'd':
                in_direction_num = 3;
                break;                                                
        
            default:
                break;
        }

        katanuki(in_kata, in_x, in_y, in_direction_num); 
        show_board(1);
        cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
        MOVE++;         

    }


    //実行時間計測用
    end = std::chrono::system_clock::now();  // 計測終了時間
    double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(); //処理に要した時間をマイクロ秒に変換
    cout << elapsed / 1000 << "ms" << "\n";
    //ここまで
}