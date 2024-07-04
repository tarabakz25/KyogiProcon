#include <bits/stdc++.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

const vector<vector<int>> startBoard;
const vector<vector<int>> goalBoard;
long actionCount = 0;

/* 抜き型の構造体 */
struct Pattern {
    int type;
    int size;
    vector<vector<int>> cells;
};

/* 文字列からベクトルに変換する */
vector<int> stringToVector(const string &str)
{
    vector<int> row;
    for (char c : str)
    {
        row.push_back(c - '0'); // 文字を整数に変換
    }
    return row;
}

/* boardやその他情報を表示する */
void printBoard(vector<vector<int>> &board, vector<vector<int>> &goalBoard,  int matchRate)
{
    system("cls");

    for(int i = 0; i < board.size(); i ++){
        for(int j = 0; j < board[i].size(); j ++){
            if(board[i][j] == goalBoard[i][j]){
                cout << "\033[32m" << board[i][j] << "\033[0m";
            }
            else {
                cout << board[i][j];
            }
        }
        cout << endl;
    }

    cout << "Match rate: " << matchRate << "%" << endl;
    cout << "Action count: " << actionCount << endl;
}


/* 型抜きをする */
int katanuki(vector<vector<int>> &tmpBoard, vector<vector<int>> &board, int i, int j, int action)
{
    int tmp = board[i][j];

    switch(action){
        case 0: //上方向の型抜き
            for(int k = i; k < board.size(); k ++){
                tmpBoard[k][j] = tmpBoard[k + 1][j];
            }
            tmpBoard[board.size() - 1][j] = tmp;
        
        case 1://左方向の型抜き
            for(int k = j; k < board[i].size() - 1; k++){
                tmpBoard[i][k] = tmpBoard[i][k + 1];
            }
            tmpBoard[i][board[i].size() - 1] = tmp;
        
        default:
           ;
    }
    actionCount ++;
    return 0;
}


/* 問題Jsonを読み込む */
int loadingJson(const json &jobj, vector<vector<int>> &startBoard, vector<vector<int>> &goalBoard)
{
    try{
        for(const auto &line : jobj["board"]["start"]){
            startBoard.push_back(stringToVector(line.get<string>()));
        }

        for(const auto &line : jobj["board"]["goal"]){
            goalBoard.push_back(stringToVector(line.get<string>()));
        }
    }
    catch(const int e){
        cerr << "Error: " << e << endl;
        return -1;
    }
    return 0;
}


/* 提出用Jsonに書き込む */
void writeJson()
{
    
}

/* ボードの一致率を計算する */
double calculateMatchRate(const vector<vector<int>> &startBoard, const vector<vector<int>> &goalBoard)
{
    int totalElements = 0;
    int matchCount = 0;

    for(int i = 0; i < startBoard.size(); i++){
        for(int j = 0; j < startBoard[i].size(); j++){
            if(startBoard[i][j] == goalBoard[i][j]){
                matchCount ++;
            }
            totalElements ++;
        }
    }
    return (double)matchCount / totalElements * 100.0;
}

/* 定形抜き型の生成 */
vector<Pattern> generatePatterns()
{
    vector<Pattern> patterns;

    for(int size = 1; size <= 256; size << 2){
        Pattern p;
        p.type = 1;
        p.size = size;
        p.cells.resize(size, vector<int>(size, 1));
        patterns.push_back(p);
    }

    for(int size = 2; size <= 256; size << 2){
        Pattern p;
        p.type = 2;
        p.size = size;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(i % 2 == 0 && j % 2 == 0){
                    p.cells[i][j] = 1;
                }
                else if(i % 2 == 1 && j % 2 == 1){
                    p.cells[i][j] = 1;
                }
                else{
                    p.cells[i][j] = 0;
                }
            }
        }
        patterns.push_back(p);
    }

    for(int size = 2; size <= 256; size << 2){
        Pattern p;
        p.type = 3;
        p.size = size;
        for(int i = 0; i < size; i++){
            for(int j = 0; j < size; j++){
                if(i % 2 == 0 && j % 2 == 0){
                    p.cells[i][j] = 1;
                }
                else if(i % 2 == 1 && j % 2 == 1){
                    p.cells[i][j] = 0;
                }
                else{
                    p.cells[i][j] = 1;
                }
            }
        }
        patterns.push_back(p);
    }
    return patterns;
}

/* エラーが発生した時 */
int errorException(const int e)
{
    cout << "エラー発生。エラーコード: " << e << endl;
    return -1;
}


/* メイン */
int main()
{
    /* 時間計測 */
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();

    /* Json読み込み */
    ifstream ifs("./sample2.json");
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    json jobj = json::parse(str);

    vector<vector<int>> startBoard, goalBoard;
    loadingJson(jobj, startBoard, goalBoard);
    int boardSide = jobj["board"]["width"].get<int>();
    int boardWarp = jobj["board"]["width"].get<int>();

    vector<Pattern> nukigata_patterns = generatePatterns();

    vector<vector<int>> tmpBoard = startBoard;
    vector<vector<int>> board = startBoard;

    /* ここからメインプログラム */
    try
    {
        while(calculateMatchRate(board, goalBoard) < 100.0)
        {
            for(int ia = 0; ia < boardWarp; ia ++){
                for(int ja = 0; ja < boardSide; ja ++){
                    if(board[ia][ja] != goalBoard[ia][ja]){

                        pair<int, int> posNow{ia, ja}; 
                        int targetNum = goalBoard[ia][ja];
                        pair<int, int> targetPos{256, 256}; //　最大値をとる

                        int ib = 256;
                        int jb = 256;
                        
                        /* targetNumと同じ数字がある座標を見つける */
                        for(int ib = ia; ib < boardWarp; ib ++) for(int jb = ja; jb < boardSide; jb ++){
                            if(board[ib][jb] != targetNum){
                                targetPos.first = min(targetPos.first, ja);
                                targetPos.second = min(targetPos.second, jb);
                            }
                        }

                        /* ターゲット数字を移動させる */
                        while(tmpBoard[ib][ja] == goalBoard[ia][ja]) katanuki(tmpBoard, board, ib, ja, 1);
                        
                        while(tmpBoard[ia][ja] == goalBoard[ia][ja]) katanuki(tmpBoard, board, ia, ja, 0);
                        
                        printBoard(tmpBoard, goalBoard, calculateMatchRate(board, goalBoard));
                    }
                }
            }
        }
    }
    catch(const int e){
        errorException(e);
    }
}
