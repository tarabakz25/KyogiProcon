#include <bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

void katanuki(vec<vec<int>> &tmpBoard, vec<vec<int>> &board, int i, int j, int action)
{
    int temp = tmpBoard[i][j];
    if (action == 0 && i != board.size() - 1)
    { // upKatanuki
        for (int k = i; k < board.size() - 1; k++)
        {
            tmpBoard[k][j] = tmpBoard[k + 1][j];
        }
        tmpBoard[board.size() - 1][j] = temp;
    }
    else if (action == 1 && j != board[i].size() - 1)
    { // leftKatanuki
        for (int k = j; k < board[i].size() - 1; k++)
        {
            tmpBoard[i][k] = tmpBoard[i][k + 1];
        }
        tmpBoard[i][board[i].size() - 1] = temp;
    }
}

// 文字列からベクトルに変換する関数
vec<int> stringToVector(const string &str)
{
    vec<int> row;
    for (char c : str)
    {
        row.push_back(c - '0'); // 文字を整数に変換
    }
    return row;
}

// JSONからボードを読み込む関数
int loadBoard(const json &jobj, vec<vec<int>> &startBoard, vec<vec<int>> &goalBoard)
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
void printBoard(const vec<vec<int>> &board1, const vec<vec<int>> &board2, bool action)
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
double calculateMatchRate(const vec<vec<int>> &startBoard, const vec<vec<int>> &goalBoard)
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

int main()
{
    // 時間計測
    chrono::system_clock::time_point start, end;
    start = chrono::system_clock::now();

    // JSONファイルの読み込み
    ifstream ifs("./sample1.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json jobj = json::parse(str);
    vec<vec<int>> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard);
    int boardSide = jobj["board"]["width"].get<int>();
    int boardWarp = jobj["board"]["height"].get<int>();

    // 抜き型
    vec<int> nukigata = {1};

    system("clear");
    printBoard(startBoard, goalBoard, true);
    cout << "一致率: " << calculateMatchRate(startBoard, goalBoard) << " %" << endl;

    // 次から探索アルゴリズムの開始
    vec<vec<int>> tmpBoard = startBoard;

    int breakPoint = 2000;
    bool flag = false;
    double lastScore = 0;
    int breakFlag = false;
    double totalMaxScore = 0;
    double maxScore = 0;
    pair<int, int> shortestMatch;

    while (totalMaxScore != 100)
    {
        for (int i = 0; i < startBoard.size(); i++)
        {
            for (int j = 0; j < startBoard[i].size(); j++)
            {
                for (int k = 0; k < startBoard.size(); k++)
                {
                    for (int ki = i + 1; ki < i + 2; k++)
                    {
                        if (tmpBoard[i][j] == goalBoard[ki][j])
                        {
                            shortestMatch = {ki, j};
                            breakFlag = true;
                            break;
                        }
                    }
                    if (breakFlag)
                    {
                        break;
                    }
                }
                if (breakFlag)
                {
                    break;
                }
            }
        }
    }
    return 0;
}