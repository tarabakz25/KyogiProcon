#include <bits/stdc++.h>
#include "json.hpp"

#define rep(i, n) for(int i = 0; i < n; i++)

using namespace std;
using json = nlohmann::json;

/* 変数の先宣言 */
int counter = 0;


vector<vector<vector<int>>> nukigata;

void generateNukigata()
{
    for(int size = 1; size < 256; size *= 2){
        vector<vector<int>> kata(size, vector<int>(size));

        if(size == 1){
            kata[0][0] = 1;
            nukigata.push_back(kata);
        }
        else{
            /* all 1 */
            rep(i, size){
                rep(j, size){
                    kata[i][j] = 1;
                }
            }
            nukigata.push_back(kata);

            /* 横 */
            rep(i, size){
                rep(j, size){
                    if(i % 2 == 0) kata[i][j] = 1;
                    else kata[i][j] = 0;
                }
            }        
            nukigata.push_back(kata);

            /* 縦 */
            rep(i, size){
                rep(j, size){
                    if(j % 2 == 0) kata[i][j] = 1;
                    else kata[i][j] = 0;
                }
            }        
            nukigata.push_back(kata);
        }
    }
}



void katanuki(vector<vector<int>>& startB, vector<vector<int>>& goalB, int i, int j, int iCount, int jCount, int action)
{
    int temp = startB[i][j];

    /* up */
    if(action == 0){
        
    }

    /* left */
    

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

int Main()
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

                    int iCount = 0;
                    int jCount = 0;

                    for(int ii = i; ii < height; ii++){
                        iCount ++;
                        for(int jj = j; jj < width; jj++){
                            jCount ++;

                            if(startB[ii][jj] == goalB[i][j]){
                                
                                
                            }
                        }
                    }
                }
            }
        }
    }


    return 0;
}

void test()
{
    rep(k, nukigata.size()){
        vector<vector<int>> kata = nukigata[k];

        rep(i, kata.size()){
            rep(j, kata[i].size()){
                cout << kata[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

int main()
{
    generateNukigata();

    return 0;
}