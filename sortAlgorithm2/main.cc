#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include "json.hpp"
typedef long long ll;
#define rep(i, n) for (ll i = 0; i < n; i++)
#define nrep(i, n) for (ll i = n; i >= 0; i--)
#define rep2(i, a, n) for (ll i = a; i < n; i++)

#define DEBUG
#define PRINTING

using namespace std;
using vec = vector<vector<int>>; //vectorを省略してみやすく
using json = nlohmann::json;

/* 変数の先宣言 */
int counter = 0; // 手数のカウント
double matchRate;
int HEIGHT, WIDTH;
int use_samplefile = 1;
chrono::system_clock::time_point start, end;
bool updated = false; // ボードが更新されたかを確認するフラグ
vector<int> number{256, 128, 64, 32, 16, 8, 4, 2, 1};
vector<int> gene_only{1, 2, 4, 8, 16, 32, 64, 128, 256};

/* 回答用構造体 */
struct Answer{
    int n;
    int p;
    int x;
    int y;
    int s;
};
json answers = json::array();

/* ぬき型たち */
map<int, vec> all1_nukigata;

/* ぬき型の生成 */
void generateNukigata()
{
    for(int size : gene_only){
        vec kata(size, vector<int>(size, 1));

        all1_nukigata[size] = kata;
    }
}
// 一致率計算
double calculateMatchRate(const vec &sB, const vec &gB)
{
    int totalElements = 0;
    int matchCount = 0;
    rep(di, HEIGHT){
        rep(dj, WIDTH){
            if (sB[di][dj] == gB[di][dj]) matchCount++;
            totalElements++;
        }
    }
    return (double)matchCount / totalElements * 100.0;
}

void scorePrint(const vec &sB, const vec &gB, chrono::system_clock::time_point start, chrono::system_clock::time_point end, int i, int j, int diff, int direction)
{
    system("clear");

#ifdef PRINTING
    // 型の表示
    rep(di, HEIGHT){
        rep(dj, WIDTH){
            #ifndef DEBUG
            if (sB[di][dj] == gB[di][dj]) cout << "\033[31m" << sB[di][dj] << "\033[m";
            else cout << sB[di][dj];
            #endif
            #ifdef DEBUG
            cout << sB[di][dj];
            #endif
            cout << " ";
        }
        cout << endl;
    }
#endif

    // 一致率, 手数
    end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);
    double score = calculateMatchRate(sB, gB);
    cout << "MatchRate:" << (int)score << "%" << " " << "Count:" << counter << " " << "time:" << time << "s" << " now:(" << i << ", " << j << ")" << endl;
    //this_thread::sleep_for(chrono::milliseconds(100)); // デバッグのためタイマー制御
}

void katanuki(vec &sB, vec &gB, int i, int j, int diff, int direction)
{
    vector<int> use_nukigata_size;
    
    for(int size : number){
        if(size <= diff){
            use_nukigata_size.push_back(size);
            diff -= size;
        }
    }

    for(int n : use_nukigata_size){
        vec unplug_num(HEIGHT, vector<int>(WIDTH, -1));//抜き出す数字
        vec push_num(HEIGHT, vector<int>(WIDTH, -1));//寄せる数字

        // 右の場合
        if(direction == 3){
            unsigned width_diff = j - n;
            rep(di, n){
                if(i + di >= HEIGHT) break;
                else{
                    copy(sB[i + di].begin() + j + 1, sB[i + di].begin() + j + n + 1, unplug_num[di].begin());
                    sB[i + di].erase(sB[i + di].begin() + j + 1, sB[i + di].begin() + j + n + 1);
                }
            }
            rep(di, n){
                if(i + di >= HEIGHT) break;
                else sB[i + di].insert(sB[i + di].begin(), unplug_num[di].begin(), unplug_num[di].begin() + n);
            }
        }
        // 左の場合
        else if(direction == 2){
            rep(di, n){
                if(i + di >= HEIGHT) break;
                else{
                    copy(sB[i + di].begin() + j, sB[i + di].begin() + j + n, unplug_num[di].begin());
                    sB[i + di].erase(sB[i + di].begin() + j, sB[i + di].begin() + j + n);
                }
            }
            rep(di, n){
                if(i + di >= HEIGHT) break;
                else sB[i + di].insert(sB[i + di].end(), unplug_num[di].begin(), unplug_num[di].begin() + n);
            }
        }
        // 上の場合
        else{
            unsigned height_diff = HEIGHT - i - n;

            rep(di, n){
                if(j + n >= WIDTH) copy(sB[i + di].begin() + j, sB[i + di].end(), unplug_num[di].begin());
                else copy(sB[i + di].begin() + j, sB[i + di].begin() + j + n, unplug_num[di].begin());
            }
            rep(di, height_diff){
                if(j + n >= WIDTH) copy(sB[i + n + di].begin() + j, sB[i  + n + di].end(), push_num[di].begin());
                else copy(sB[i + n + di].begin() + j, sB[i+ n + di].begin() + j + n, push_num[di].begin());
            }
            rep(di, height_diff){
                if(push_num[di][0] == -1) break;
                rep(dj, n){
                    if(push_num[di][dj] == -1) break;
                    else sB[i + di][j + dj] = push_num[di][dj];
                }
            }
            rep(di, n){
                rep(dj, n){
                    if(unplug_num[di][dj] == -1) break;
                    else sB[i + height_diff + di][j + dj] = unplug_num[di][dj];
                }
                if(unplug_num[di][0] == -1) break;
            }
        }
        counter++;

        // JSONに保存
        json answer;
        answer["p"] = 3 * (n - 2) + 1;
        answer["x"] = j;
        answer["y"] = i;
        answer["s"] = direction;

        answers.push_back(answer);

        auto end = chrono::system_clock::now();
        scorePrint(sB, gB, start, end, i, j, diff, direction);
    }
}

vector<int> stringToVector(const string &str)
{
    vector<int> row;
    for (char c : str)
        row.push_back(c - '0');

    return row;
}

// 型の読み込み
void loadBoard(const json &j, vec &sB, vec &gB)
{
    for (const auto &line : j["board"]["start"])
        sB.push_back(stringToVector(line.get<string>()));
    for (const auto &line : j["board"]["goal"])
        gB.push_back(stringToVector(line.get<string>()));
}

// 探索中のボードを表示。　使うことは多分ないかも
void findingPrint(const vec &sB, const vec &gB, const int i, const int j, chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{
    system("clear");

    rep(ii, sB.size())
    {
        rep(jj, sB[0].size())
        {
            if (ii == i && jj == j)
                cout << "\033[7m" << sB[ii][jj] << "\033[m" << " ";
            else
                cout << sB[ii][jj] << " ";
        }
        cout << endl;
    }

    // 一致率, 手数
    end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    double score = calculateMatchRate(sB, gB);
    cout << "Match Rate: " << score << "%" << " " << "Count: " << counter << " " << "time: " << time << endl;
}

void Main()
{
    // 時間を計測
    start = chrono::system_clock::now();

    // Json読み込み
    ifstream ifs("sample1.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json J = json::parse(str);
    vec sB, gB;
    loadBoard(J, sB, gB);
    HEIGHT = sB.size(), WIDTH = gB[0].size();

    double matchRate = calculateMatchRate(sB, gB);

    while (matchRate < 100.0){
        rep(i, HEIGHT){
            rep(j, WIDTH){
                if (sB[i][j] != gB[i][j]){
                    int target = gB[i][j];
                    updated = false;
                    bool flag = false;
                    //縦方向
                    rep2(ii, i, HEIGHT){
                        if(sB[ii][j] == gB[i][j]){
                            katanuki(sB, gB, i, j, ii , 0);
                            flag = true;
                            break;
                        }
                    }
                    //横方向
                    if(!flag){
                        rep2(jj, j, WIDTH){
                            if(sB[i][jj] == gB[i][j]){
                                if(j >= jj) katanuki(sB, gB, i, j, abs(jj - j), 3);
                                else katanuki(sB, gB, i, j, abs(jj - j), 2);
                                flag = true;
                                break;
                            }
                        }
                    }
                    //両方使う場合
                    if(!flag){
                        rep2(ii, i, HEIGHT){
                            if(ii == i){
                                rep2(jj, j, WIDTH){
                                    if(sB[ii][jj] == gB[i][j]){
                                        if(j >= jj) katanuki(sB, gB, ii, jj, abs(jj - j), 3);
                                        else katanuki(sB, gB, ii, j, abs(jj - j), 2);
                                        katanuki(sB, gB, i, j, ii - i, 0);
                                        flag = true;
                                        break;
                                    }
                                }
                            }
                            else{
                                rep(jj, WIDTH){
                                    if(sB[ii][jj] == gB[i][j]){
                                        if(j >= jj) katanuki(sB, gB, ii, jj, abs(jj - j), 3);
                                        else katanuki(sB, gB, ii, j, abs(jj - j), 2);
                                        katanuki(sB, gB, i, j, ii - i, 0);
                                        flag = true;
                                        break;
                                    }
                                }
                            }
                            if(flag) break;
                        }
                    }
                }
            }
        }
        if (!updated) break;
        matchRate = calculateMatchRate(sB, gB);
    }
    cout << "\033[31m" << "FINISHED!!" << "\033[m" << endl;
}

int main()
{
    generateNukigata();
    Main();

    // 回答JSONの作成
    json final_answer;
    final_answer["n"] = counter;  // 手数を保存
    final_answer["ops"] = answers; // 操作履歴を保存

    // 回答JSONをファイルに保存
    ofstream ofs("answer.json");
    ofs << final_answer.dump(4);  // インデント付きでJSONを書き込む

    return 0;
}
