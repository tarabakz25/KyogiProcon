#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;


void katanuki(vector<vector<int>>& tmpBoard, vector<vector<int>>& board, int i, int j, int action)
{
    int temp = tmpBoard[i][j];
    if(action == 0 && i != board.size() - 1){//upKatanuki
        for(int k = i ;k < board.size()-1; k++){
            tmpBoard[k][j] = tmpBoard[k + 1][j];
        }
        tmpBoard[board.size() - 1][j] = temp;
    }
    else if(action == 1 && j != board[i].size() - 1){//leftKatanuki
        for(int k = j; k < board[i].size()-1; k++){
            tmpBoard[i][k] = tmpBoard[i][k + 1];
        }
        tmpBoard[i][board[i].size() - 1] = temp;
    }
}

// 文字列からベクトルに変換する関数
vector<int> stringToVector(const string& str) {
    vector<int> row;
    for (char c : str) {
        row.push_back(c - '0');  // 文字を整数に変換
    }
    return row;
}

// JSONからボードを読み込む関数
int loadBoard(const json& jobj, vector<vector<int>>& startBoard, vector<vector<int>>& goalBoard) {
    for (const auto& line : jobj["board"]["start"]) 
    {
        startBoard.push_back(stringToVector(line.get<string>()));
    }
    for (const auto& line : jobj["board"]["goal"]) 
    {
        goalBoard.push_back(stringToVector(line.get<string>()));
    }
    return 0;
}


// ボードを表示する関数
void printBoard(const vector<vector<int>>& board1, const vector<vector<int>>& board2, bool action) {
    if (action) {
        for (int i = 0; i < board1.size(); i++) {
            for (int j = 0; j < board1[i].size(); j++) {
                if (board1[i][j] == board2[i][j]) {
                    cout << "\033[31m" << board1[i][j] << "\033[m" << " ";
                } else {
                    cout << board1[i][j] << " ";
                }
            }
            cout << endl;
        }
    } else {
        for (int i = 0; i < board1.size(); i++) {
            for (int j = 0; j < board1[i].size(); j++) {
                cout << board1[i][j] << " ";
            }
            cout << endl;
        }
    }
}

// 一致率を計算する関数
double calculateMatchRate(const vector<vector<int>>& startBoard, const vector<vector<int>>& goalBoard) 
{
    int totalElements = 0;
    int matchCount = 0;
    for (int i = 0; i < startBoard.size(); i++) {
        for (int j = 0; j < startBoard[i].size(); j++) {
            if (startBoard[i][j] == goalBoard[i][j]) {
                matchCount++;
            }
            totalElements++;
        }
    }
    return (double)matchCount / totalElements * 100.0;
}

int main() 
{
    //時間計測
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();

    //JSONファイルの読み込み
    ifstream ifs("./sample2.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json jobj = json::parse(str);
    vector<vector<int>> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard);
    int boardSide = jobj["board"]["width"].get<int>();
    int boardWarp = jobj["board"]["height"].get<int>();

    //抜き型
    vector<int> nukigata = {1};

    system("clear");
    printBoard(startBoard, goalBoard, true);
    cout << "一致率: " << calculateMatchRate(startBoard, goalBoard) << " %" << endl;
   

    //次から探索アルゴリズムの開始
    vector<vector<int>> board = startBoard;
    vector<vector<int>> tmpBoard = board;
    int count = 1;
    int breakPoint = 2000;
    bool flag = false;
    double lastScore = 0;
    int breakCount = 0;
    double totalMaxScore = 0;
    double maxScore = 0;

    while(totalMaxScore != 100)
    {
    for(int i = 0; i < board.size(); i++)
    {
        int j = 0;       
        
        while(j < board[i].size()){
            
            if(i != board.size()-1){
                katanuki(tmpBoard, board, i, j, 0);
                count ++;
            }
            if(j != board[i].size()-1){
                katanuki(tmpBoard, board, i, j, 1);
                count ++;
            }

            for(int k = i; k < i + 2; k++){
                if(k == board.size()-1) break;
                katanuki(tmpBoard, board, k+1, 0, 1);
                count ++;
            }


            
            maxScore = calculateMatchRate(tmpBoard, goalBoard);


            if(tmpBoard[i][j] == goalBoard[i][j]) j++;

            system("clear");
            printBoard(tmpBoard, goalBoard, true);
            cout << "動かした所: " << i << ", " << j << ", 動かし方: " << 0 << " 手数: " << count << endl;
            totalMaxScore = max(maxScore, totalMaxScore);
            cout << "一致率: " << maxScore << " %" << endl;
            cout << "最大一致率:" << totalMaxScore << " %" << endl;
            cout << "time: " << static_cast<double>(chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0) << "ms" << endl;
            
            //count ++;
            //this_thread::sleep_for(chrono::milliseconds(500));
            //if(count >= breakPoint) break;
        }  
        
        //if(count >= breakPoint) break;
    }
    break;
    //if(count >= breakPoint) break;
    }

    return 0;
}