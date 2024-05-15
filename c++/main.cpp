#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;



void katanuki(vector<vector<int>>& tmpBoard, vector<vector<int>>& board, int i, int j, int action)
{
    if(action == 0 && i != board.size() - 1){//upKatanuki
        int temp = board[i][j];
        for(int k = i ;k < board.size()-1; k++){
            tmpBoard[k][j] = tmpBoard[k + 1][j];
        }
        tmpBoard[board.size() - 1][j] = temp;
    }
    else if(action == 1 && i != 0){//downKatanuki
        int temp = board[i][j];
        for(int k = i; k > 0; k--){
            tmpBoard[k][j] = tmpBoard[k - 1][j];
        }
        tmpBoard[0][j] = temp;
    }
    else if(action == 2 && j != board[i].size() - 1){//leftKatanuki
        int temp = board[i][j];
        for(int k = j; k < board.size()-1; k++){
            tmpBoard[i][k] = tmpBoard[i][k + 1];
        }
        tmpBoard[i][board[i].size() - 1] = temp;
    }
    else if(action == 3 && j != 0){//rightKatanuki
        int temp = board[i][j];
        for(int k = j; k > 0; k--){
            tmpBoard[i][k] = tmpBoard[i][k - 1];
        }
        tmpBoard[i][0] = temp;
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

// JSONから抜き型を読み込む関数
// int generateNukigata(const json& jobj) 
// {
//     for (const auto& pattern : jobj["general"]["patterns"]) {
//         int index = pattern["p"];
//         vector<vector<int>> patternCells;
//         for (const auto& line : pattern["cells"]) {
//             patternCells.push_back(stringToVector(line.get<string>()));
//         }
//         nukigata[index] = patternCells;
//     }
//     return 0;
// }

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
    ifstream ifs("./sample1.json");
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
    bool scoreFlag = false;
    double lastScore;

    while(calculateMatchRate(startBoard, goalBoard) < 100.0)
    {
        vector<tuple<double, int, int, int>> scores;
        int boardCount = 0;
               

        if(scoreFlag){
            for(int i = 0; i < board.size(); i++){
                for(int j = 0; j < board[i].size(); j++){
                    if(tmpBoard[i][j] == goalBoard[i][j]){
                        if(i == 0) katanuki(tmpBoard, board, i, j, 0);
                        else if(i == tmpBoard.size() - 1) katanuki(tmpBoard, board, i, j, 1);
                        else if(j == 0) katanuki(tmpBoard, board, i, j, 2);
                        else katanuki(tmpBoard, board, i, j, 3);
                    }
                }
            }
        }

        else{
            for(int i = 0; i < board.size(); i++){
                for(int j = 0; j < board[i].size(); j++){
                   for(int k = 0; k < 4; k++){
                        katanuki(tmpBoard, board, i, j, k);
                        scores.push_back(make_tuple(calculateMatchRate(tmpBoard, goalBoard), i, j, k));
                        tmpBoard = board;
                        boardCount++;
                    }
                }
            }
        }
        
        sort(scores.begin(), scores.end(), [](const tuple<double, int, int, int>& a, const tuple<double, int, int, int>& b) {
            return get<0>(a) < get<0>(b);
        });

        
        tuple<double, int, int, int> maxData;
        maxData = scores[scores.size()-1];

        katanuki(tmpBoard, board, get<1>(maxData), get<2>(maxData), get<3>(maxData));
        double maxScore = calculateMatchRate(tmpBoard, goalBoard);
        board = tmpBoard;


        if(maxScore == lastScore){
            katanuki(tmpBoard, goalBoard, get<2>(maxData), get<2>(maxData), get<3>(maxData) + 1);
        }
        double lastScore = maxScore;


        system("clear");
        printBoard(tmpBoard, goalBoard, true);
        cout << "動かした所: " << get<1>(maxData) << ", " << get<2>(maxData) << ", 動かし方: " << get<3>(maxData) << endl;
        cout << "手数: " << count << " Flag: " << scoreFlag << endl;
        cout << "一致率: " << calculateMatchRate(tmpBoard, goalBoard) << " %" << endl;
        cout << "time: " << static_cast<double>(chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - start).count() / 1000.0) << "ms" << endl;

        if(count == 500) break;
        else count ++;
    }
    return 0;
}