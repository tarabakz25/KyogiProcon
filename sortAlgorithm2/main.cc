#include <bits/stdc++.h>
#include "json.hpp"
#include "json_write.cc"
typedef long long ll;

#define rep(i, n) for(ll i = 0; i < n; i++)
#define nrep(i, n) for(ll i = n; i >= 0; i--)
#define rep2(i, a, n) for(ll i = a; i < n; i++)

//#define DEBUG 
#define PRINTING

using namespace std;
using json = nlohmann::json;

/* 変数の先宣言 */
int counter = 0; //手数のカウント
int height, width;
int use_samplefile = 1;
chrono::system_clock::time_point start, end;

vector<int> number{1, 2, 4, 8, 16, 32, 64, 128, 256};

/* ぬき型たち */
vector<int> one_nukigata[1];
map<int, vector<vector<int>>> all1_nukigata;

/* ぬき型の生成 */
void generateNukigata()
{
    ll i, j;
    for(int size = 1; size <= 256; size *= 2){
        vector<vector<int>> kata(size, vector<int>(size));

            rep(i, size){
                rep(j, size){
                    kata[i][j] = 1;
                }
            }
            all1_nukigata[size] = kata;
    }
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

//スコアの表示
void scorePrint(const vector<vector<int>>& startB, const vector<vector<int>>& goalB, const int width, const int height, chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{
    system("clear");

    #ifdef PRINTING
    //型の表示
    rep(i, height){
        rep(j, width){
           if(startB[i][j] == goalB[i][j]) cout << "\033[31m" << startB[i][j] << "\033[m";
           else cout << startB[i][j];
        }
        cout << endl;
    }
    #endif

    //一致率, 手数
    end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    double score = calculateMatchRate(startB, goalB);
    cout << "Match Rate: " << score << "%" << " " << "Count: " << counter << " " << "time: " << time << endl;

    this_thread::sleep_for(chrono::seconds(1));
}

void warp_katanuki(vector<vector<int>>& startB, vector<vector<int>>& goalB, int i, int j, int iCount, int jCount)
{
    vector<ll> use_kata_size;
    ll total_size = iCount;

    for(auto it = number.rbegin(); it != number.rend(); it ++){
        while(total_size >= *it){
            use_kata_size.push_back(*it);
            total_size -= *it;
        }
    }

    #ifdef DEBUG
    rep(k, use_kata_size.size()){
        cout << use_kata_size[k] << " ";
    }
    cout << endl;
    #endif

    for(ll n : use_kata_size){
        if(all1_nukigata.find(n) == all1_nukigata.end()){
            cerr << "抜き型サイズ " << n << " が存在しません。" << endl;
            continue;
        }

        rep(ii, n){
            rep(jj, n){
                int ni = i + ii * (n - 1);
                int nj = j + jj * (n - 1);

                if(ni >= 0 && ni < height && nj >= 0 && nj < width){
                    startB[ni][nj] = all1_nukigata[n][ii][jj];
                }
            }
        }

        // ボードを上方向にシフト
        for(int dj = j; dj < j + n && dj < width; ++dj){
            int write_row = 0;
            for(int row = 0; row < height; ++row){
                if(startB[row][dj] != -1){
                    if(write_row != row){
                        startB[write_row][dj] = startB[row][dj];
                        startB[row][dj] = -1;
                    }
                    write_row++;
                }
            }
            // 空きスペースを補充（目標ボードに基づいて補充）
            for(int row = write_row; row < height; ++row){
                startB[row][dj] = goalB[row][dj];
            }
        }

        counter++;

        auto it = std::find(number.begin(), number.end(), n);
        int index = distance(number.begin(), it);
        int piece = index + 1 + 2; 

        //json_write(counter, piece, i, j, 0);
    }
    
    auto end = chrono::system_clock::now();
    scorePrint(startB, goalB, height, width, start, end);
}

void side_katanuki(vector<vector<int>>& startB, vector<vector<int>>& goalB, int i, int j, int iCount, int jCount)
{
    vector<ll> use_kata_size;
    ll total_size = jCount;

    // 大きい順に抜き型サイズを選定
    for(auto it = number.rbegin(); it != number.rend(); it++){
        while(total_size >= *it){
            use_kata_size.push_back(*it);
            total_size -= *it;
        }
    }

    #ifdef DEBUG
    cout << "Selected kata sizes for side_katanuki: ";
    rep(k, use_kata_size.size()) {
        cout << use_kata_size[k] << " ";
    }
    cout << endl;
    #endif
    for(ll n : use_kata_size){

        // 抜き型を適用する
        for(int di = 0; di < n; ++di){
            for(int dj = 0; dj < n; ++dj){
                int ni = i + di;
                int nj = j + dj;
                if(ni >=0 && ni < height && nj >=0 && nj < width){
                    startB[ni][nj] = -1; // -1 を空きスペースとしてマーク
                }
            }
        }

        // ボードを左方向にシフト
        for(int row = i; row < i + n && row < height; ++row){
            int write_col = 0;
            for(int col = 0; col < width; ++col){
                if(startB[row][col] != -1){
                    if(write_col != col){
                        startB[row][write_col] = startB[row][col];
                        startB[row][col] = -1;
                    }
                    write_col++;
                }
            }
            // 空きスペースを補充（目標ボードに基づいて補充）
            for(int col = write_col; col < width; ++col){
                startB[row][col] = goalB[row][col];
            }
        }

        counter++;
    }
    

    auto end = chrono::system_clock::now();
    scorePrint(startB, goalB, height, width, start, end);
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


//探索中のボードを表示。　使うことは多分ないかも
void findingPrint(const vector<vector<int>>& startB, const vector<vector<int>>& goalB, const int i, const int j, chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{
    system("clear");

    rep(ii, startB.size()){
        rep(jj, startB[0].size()){
            if(ii == i && jj == j) cout << "\033[7m" << startB[ii][jj] << "\033[m" << " ";
            else cout << startB[ii][jj] << " ";
        }
        cout << endl;
    }
    
    //一致率, 手数
    end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    double score = calculateMatchRate(startB, goalB);
    cout << "Match Rate: " << score << "%" << " " << "Count: " << counter << " " << "time: " << time << endl;
}

void Main()
{
    // 時間を計測
    start = chrono::system_clock::now();

    // Json読み込み
    ifstream ifs("sample1.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>()); 
    json j = json::parse(str);
    vector<vector<int>> startB, goalB;
    loadBoard(j, startB, goalB);
    width = j["board"]["width"].get<int>();
    height = j["board"]["height"].get<int>();

    double matchRate = calculateMatchRate(startB, goalB);

    #ifdef DEBUG
    scorePrint(startB, goalB, width, height, start, end);
    #endif

    #ifndef DEBUG
    while(matchRate < 100.0){
        bool updated = false;
        
        rep(i, height){
            rep(j, width){
                if(startB[i][j] != goalB[i][j]){
                    int target_value = goalB[i][j];
                    int pointi = -1;
                    int pointj = -1;
                    int iCount = 0, jCount = 0;
                    bool flag = false;

                    // 縦方向に同じ値を探す
                    rep2(ii, i, height){
                        if(startB[ii][j] == target_value){
                            pointi = ii;
                            iCount = ii - i;
                            flag = true;
                            break;
                        }
                    }

                    // 横方向に同じ値を探す
                    rep2(jj, j, width){
                        if(startB[i][jj] == target_value){
                            pointj = jj;
                            jCount = jj - j;
                            flag = true;
                            break;
                        }
                    }

                    if(pointi != -1 && pointj != -1){
                        bool flag = false;
                        for(ll ii = i + 1; ii < height; ii++){
                            for(ll jj = j + 1; jj < width; jj++){
                                if(startB[ii][jj] == goalB[i][j]){
                                    if(find(number.begin(), number.end(), ii) != number.end() && find(number.begin(), number.end(), jj) != number.end()){
                                        pointi = ii;
                                        pointj = jj;
                                        flag = true;
                                        
                                        warp_katanuki(startB, goalB, i, j, iCount, jCount);
                                        side_katanuki(startB, goalB, i, j, iCount, jCount);
                                    }
                                    else{
                                        pointi = ii;
                                        pointj = jj;
                                    }   
                                }
                            }
                            if(flag) break;
                        }

                        if(!flag){
                            warp_katanuki(startB, goalB, i, j, iCount, jCount);
                            side_katanuki(startB, goalB, i, j, iCount, jCount);
                        }

                        if(pointi == -1 && pointj == -1){

                        }
                    }
                    // 縦方向のみ
                    else if(pointi != -1){
                        warp_katanuki(startB, goalB, i, j, iCount, 0);
                    }
                    // 横方向のみ
                    else if(pointj != -1){
                        side_katanuki(startB, goalB, i, j, 0, jCount);
                    }
                }
            }
        }

        // 一致率を再計算
        matchRate = calculateMatchRate(startB, goalB);

        // スコアを表示
        auto end = chrono::system_clock::now();
        scorePrint(startB, goalB, width, height, start, end);
        if(!updated){
            break;
        }
    }
    #endif
}

int main()
{
    generateNukigata();
    Main();

    return 0;
}
