#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

struct Pattern {
    int type;
    int size;
    vector<vector<int>> cells;
};

void katanuki(vector<vector<int>> &tmpBoard, vector<vector<int>> &board, vector<vector<int>> nukigata, int i, int j, int action)
{
    vector<int> nuki_cells = {};
        
        for(int ni = 0; ni < nukigata.size(); ni++){
            for(int nj = 0; nj < nukigata[ni].size(); nj++){
                if(nukigata[ni][nj] == 1){
                    nuki_cells.push_back(tmpBoard[ni][nj]);
                    tmpBoard[ni][nj] = -1;
                }
            }
        }

    if(action == 0 && i <= board.size()-1 && j <= board[i].size()-1){
        for(int bi = i; bi < board.size()-1; bi++){
            for(int bj = j; )
        }
    }
}

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

// ボードを表示する関数
void printBoard(const vector<vector<int>> &board1, const vector<vector<int>> &board2, bool action)
{
    if (action)
    {
        for (int i = 0; i < board1.size(); i++)
        {
            for (int j = 0; j < board1[i].size(); j++)
            {
                if (board1[i][j] == board2[i][j])
                {
                    cout << "\033[31m" << board1[i][j] << "\033[m" << " ";
                }
                else
                {
                    cout << board1[i][j] << " ";
                }
            }
            cout << endl;
        }
    }
    else
    {
        for (int i = 0; i < board1.size(); i++)
        {
            for (int j = 0; j < board1[i].size(); j++)
            {
                cout << board1[i][j] << " ";
            }
            cout << endl;
        }
    }
}

// 一致率を計算する関数
double calculateMatchRate(const vector<vector<int>> &startBoard, const vector<vector<int>> &goalBoard)
{
    int totalElements = 0;
    int matchCount = 0;
    for (int i = 0; i < startBoard.size(); i++)
    {
        for (int j = 0; j < startBoard[i].size(); j++)
        {
            if (startBoard[i][j] == goalBoard[i][j])
            {
                matchCount++;
            }
            totalElements++;
        }
    }
    return (double)matchCount / totalElements * 100.0;
}

//抜き型生成する関数
vector<Pattern> generate_patterns() {
    vector<Pattern> patterns;

    // タイプI：すべてのセルが1
    for (int size = 1; size <= 256; size *= 2) {
        Pattern p;
        p.type = 1;
        p.size = size;
        p.cells.resize(size, vector<int>(size, 1));
        patterns.push_back(p);
    }

    // タイプII：偶数行のセルが1，奇数行のセルが0
    for (int size = 2; size <= 256; size *= 2) {
        Pattern p;
        p.type = 2;
        p.size = size;
        p.cells.resize(size, vector<int>(size));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                p.cells[i][j] = (i % 2 == 0) ? 1 : 0;
            }
        }
        patterns.push_back(p);
    }

    // タイプIII：偶数列のセルが1，奇数列のセルが0
    for (int size = 2; size <= 256; size *= 2) {
        Pattern p;
        p.type = 3;
        p.size = size;
        p.cells.resize(size, vector<int>(size));
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                p.cells[i][j] = (j % 2 == 0) ? 1 : 0;
            }
        }
        patterns.push_back(p);
    }

    return patterns;
}

//抜き型呼び出し関数
// 指定した抜き型を取得する関数
Pattern get_pattern(const vector<Pattern>& patterns, int type, int size) {
    for (const auto& pattern : patterns) {
        if (pattern.type == type && pattern.size == size) {
            return pattern;
        }
    }
    throw invalid_argument("指定したパターンが見つかりません");
}
int main()
{
    // 時間計測
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();

    // JSONファイルの読み込み
    ifstream ifs("./sample2.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json jobj = json::parse(str);
    vector<vector<int>> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard);
    int boardSide = jobj["board"]["width"].get<int>();
    int boardWarp = jobj["board"]["height"].get<int>();

    // 抜き型
    vector<Pattern> nukigata_patterns = generate_patterns();
    
    //アルゴリズムの開始
    vector<vector<int>> tmpBoard = startBoard;
    vector<vector<int>> board = startBoard;

    while(calculateMatchRate(board, tmpBoard) < 100.0){
        for(int i = 0; i < tmpBoard.size(); i++){
            for(int j = 0; j < tmpBoard.size(); j++){
                for(int k = 0; k < 4; k++){

                }
            }
        }
    }


    return 0;
}