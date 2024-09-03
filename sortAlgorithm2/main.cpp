#include <bits/stdc++.h>
#include "json.hpp"

#define rep(i, n) for(int i = 0; i < n; i++)

using namespace std;
using json = nlohmann::json;

/* 変数の先宣言 */
int counter = 0;

/* 抜き型を作成 */
vector<vector<vector<int>>> nukigata;

nukigata.push_back()

void katanuki(vector<vector<int>>& startB, vector<vector<int>>& goalB, int i, int j, int action)
{
    int temp = startB[i][j];

    /* up */
    if(action == 0 && i  != goalB.size() - 1){
        for(int k = i; k < goalB.size() - 1; k ++){
            startB[k][j] = startB[k + 1][j];
        }

        startB[goalB.size() - 1][j] = temp;
    }

    /* left */
    if(action == 1 && j != goalB[i].size() - 1){
        for(int k = j; k < goalB[i].size(); k ++){
            startB[i][k] = startB[i][k + 1];
        }
        startB[i][goalB[i].size() - 1] = temp;
    }
    counter ++;
}

vector<int> stringToVector(const string& str)
{
    vector<int> row;
    for(char c : str) row.push_back(c - '0');

    return row;
}

//型の読み込み
void loadBoard(const json& j, vector<vector<int>>& startB, vector<vector<int>>& goalB)
{
    for(const auto& line : j["board"]["start"]) startB.push_back(stringToVector(line.get<string>()));
    for(const auto& line : j["board"]["goal"]) goalB.push_back(stringToVector(line.get<string>()));
}

//一致率計算
double calculateMatchRate(const vector<vector<int>>& startBoard, const vector<vector<int>>& goalBoard) 
{
    int totalElements = 0;
    int matchCount = 0;
    rep(i, startBoard.size()){
        rep(j, startBoard[i].size()){
            if (startBoard[i][j] == goalBoard[i][j]) matchCount++;
            totalElements++;
        }
    }
    return (double)matchCount / totalElements * 100.0;
}

void infoPrint(const vector<vector<int>>& startB, const vector<vector<int>>& goalB, const int& width, const int& height)
{
    //型の表示
    rep(i, height){
        rep(j, width){
           if(startB[i][j] == goalB[i][j]) cout << "\033[31m" << startB[i][j] << "\033[m" << " ";
           else cout << startB[i][j] << " ";
        }
        cout << endl;
    }

    //一致率, 手数
    system("clear");

    double score = calculateMatchRate(startB, goalB);
    cout << "Match Rate: " << score << "%" << endl;
    cout << "Count: " << counter << endl;
}

int main()
{
    //時間を計測
    chrono::system_clock::time_point start = chrono::system_clock::now();

    //Json読み込み
    ifstream ifs("sample2.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>()); 
    json j = json::parse(str);
    vector<vector<int>> startB, goalB;
    loadBoard(j, startB, goalB);
    const int width = j["board"]["width"].get<int>();
    const int height = j["board"]["height"].get<int>();

    double matchRate = calculateMatchRate(goalB, startB);


    //ソート開始
    while(matchRate != 100)
    {
        rep(i, height){
            rep(j, width){
                if(startB[i][j] != goalB[i][j]){
                    for(int ai = i; ai < height; ai++){
                        for(int aj = i; aj < width; aj++){
                            if(startB[ai][aj] == goalB[i][j]){
                                while(startB[ai][j] == goalB[i][j]){
                                    katanuki(startB, goalB, ai, j, 1);
                                    infoPrint(startB, goalB, width, height);
                                }

                                while(startB[i][j] == goalB[i][j]){
                                    katanuki(startB, goalB, i, j, 0);
                                    infoPrint(startB, goalB, width, height);
                                }

                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}