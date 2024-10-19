/* 
各々の設定はsetting.hhで行ってください。 
*/

#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include "json.hpp"
#include "setting.hh"
#include "receive_and_send.cpp"

typedef long long ll;
#define rep(i, n) for (ll i = 0; i < n; i++)
#define nrep(i, n) for (ll i = n; i >= 0; i--)
#define rep2(i, a, n) for (ll i = a; i < n; i++)

// 省略
using namespace std;
using vec = vector<vector<int>>;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

/* 変数の先宣言 */
int counter = 0;       // 手数のカウント
bool updated = false;  // ボードが更新されたかを確認するフラグ
double matchRate;      // 一致率
int HEIGHT, WIDTH;
chrono::system_clock::time_point start, end;
vector<int> number{256, 128, 64, 32, 16, 8, 4, 2, 1};

/* 回答用構造体 */
struct Answer {
    int n;
    int p;
    int x;
    int y;
    int s;
};
ordered_json answers = ordered_json::array();

/* ぬき型たち */
map<int, vec> all1_nukigata;

// システムの関数
void generateNukigata() 
{
    for (int size : number) {
        vec kata(size, vector<int>(size, 1));
        all1_nukigata[size] = kata;
    }
}

double calculateMatchRate(const vec &sB, const vec &gB) 
{
    int totalElements = 0;
    int matchCount = 0;
    rep(di, HEIGHT) {
        rep(dj, WIDTH) {
            if (sB[di][dj] == gB[di][dj]) matchCount++;
            totalElements++;
        }
    }
    return (double)matchCount / totalElements * 100.0;
}

void scorePrint(const vec &sB, const vec &gB, chrono::system_clock::time_point start, chrono::system_clock::time_point end, int i, int j, int diff, int direction) 
{

#if PRINTING

    system("clear");

    // 型の表示
    rep(di, HEIGHT) {
        rep(dj, WIDTH) {
            #if !DEBUG
                if (sB[di][dj] == gB[di][dj])
                    cout << "\033[31m" << sB[di][dj] << "\033[m";
                else
                    cout << sB[di][dj];
            #endif
            #if DEBUG
                cout << sB[di][dj];
            #endif
        }
        cout << endl;
    }
#endif

    // 一致率, 手数
    end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);
    double score = calculateMatchRate(sB, gB);
    cout << "MatchRate:" << (int)score << "%" << " " << "Count:" << counter << " " << "time:" << (int)time << "s" << " now:(" << i << ", " << j << ")" << endl;
}

vector<int> stringToVector(const string &str) 
{
    vector<int> row;
    for (char c : str) row.push_back(c - '0');
    return row;
}

void loadBoard(const json &j, vec &sB, vec &gB) 
{
    for (const auto &line : j["board"]["start"])
        sB.push_back(stringToVector(line.get<string>()));
    for (const auto &line : j["board"]["goal"])
        gB.push_back(stringToVector(line.get<string>()));
}

// 型抜き i,jには異点の座標、targetには目標座標を送る。
void katanuki(vec &sB, vec &gB, int i, int j, int targeti, int targetj, int direction) 
{
    vector<int> use_nukigata_size; // 使う抜き型のサイズ

    int diff = direction == 0 ? diff = targeti - i : direction == 2 || direction == 4 ? diff = targetj - j : diff = j - targetj;

    for (int size : number) {
        if (size <= diff) {
            use_nukigata_size.push_back(size);
            diff -= size;
        }
    }
    if (diff > 0) use_nukigata_size.push_back(1);

    for (int n : use_nukigata_size) {
        vec unplug(HEIGHT, vector<int>(WIDTH, -1));  // 抜き出す数字
        vec push(HEIGHT, vector<int>(WIDTH, -1));    // 寄せる数字

        // 左だけの場合
        if (direction == 2) {
            ordered_json answer;
            if(log2(n) != 0){
                answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
            }else{
                answer["p"] = 0;
            }
            answer["x"] = j;
            answer["y"] = i;
            answer["s"] = direction > 3 ? direction - 2 : direction;
            
            answers.push_back(answer);

            rep(di, n) {
                int curtRow = i + di;
                if (curtRow >= HEIGHT)
                    break;
                else {
                    copy(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n, unplug[di].begin());
                    sB[curtRow].erase(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n);
                    sB[curtRow].insert(sB[curtRow].end(), unplug[di].begin(), unplug[di].begin() + n);
                }
            }
        }

        // 直線上にないときの右
        else if (direction == 5) {
            ordered_json answer;
            if(log2(n) != 0){
                answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
            }else{
                answer["p"] = 0;
            }
            answer["x"] = targetj + 1;
            answer["y"] = targeti;
            answer["s"] = direction > 3 ? direction - 2 : direction;
            
            answers.push_back(answer);

            rep(di, n) {
                int curtRow = targeti + di;
                if (curtRow >= HEIGHT) break;
                else {
                    copy(sB[curtRow].begin() + targetj + 1, sB[curtRow].begin() + targetj + n + 1, unplug[di].begin());
                    sB[curtRow].erase(sB[curtRow].begin() + targetj + 1, sB[curtRow].begin() + targetj + n + 1);
                    sB[curtRow].insert(sB[curtRow].begin(), unplug[di].begin(), unplug[di].begin() + n);
                }
            }
            targetj += n;
        }

        // 直線上にないときの左
        else if (direction == 4) {
            ordered_json answer;
            if(log2(n) != 0){
                answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
            }else{
                answer["p"] = 0;
            }
            answer["x"] = j;
            answer["y"] = targeti;
            answer["s"] = direction > 3 ? direction - 2 : direction;
            
            answers.push_back(answer);

            rep(di, n) {
                int curtRow = targeti + di;
                if (curtRow >= HEIGHT) break;
                else {
                    copy(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n, unplug[di].begin());
                    sB[curtRow].erase(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n);
                    sB[curtRow].insert(sB[curtRow].end(), unplug[di].begin(), unplug[di].begin() + n);
                }
            }
        }

        // 上の場合
        else if (direction == 0) {
            unsigned height_diff = HEIGHT - i - n;
            
            ordered_json answer;
            if(log2(n) != 0){
                answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
            }else{
                answer["p"] = 0;
            }
            answer["x"] = j;
            answer["y"] = i;
            answer["s"] = direction > 3 ? direction - 2 : direction;
            
            answers.push_back(answer);

            rep(di, n) {
                int curtRow = i + di;
                if (j + n >= WIDTH) copy(sB[curtRow].begin() + j, sB[curtRow].end(), unplug[di].begin());
                else copy(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n, unplug[di].begin());
            }
            rep(di, height_diff) {
                int curtRow = i + di;
                if (j + n >= WIDTH) copy(sB[curtRow + n].begin() + j, sB[curtRow + n].end(), push[di].begin());
                else copy(sB[curtRow + n].begin() + j, sB[curtRow + n].begin() + j + n, push[di].begin());
            }
            rep(di, height_diff) {
                if (push[di][0] == -1) break;
                rep(dj, n) {
                    if (push[di][dj] == -1) break;
                    else sB[i + di][j + dj] = push[di][dj];
                }
            }
            rep(di, n) {
                if (unplug[di][0] == -1) break;
                rep(dj, n) {
                    if (unplug[di][dj] == -1) break;
                    else sB[i + height_diff + di][j + dj] = unplug[di][dj];
                }
            }
        } else {
            cerr << "ERROR!" << endl;
        }

        counter++;

        auto end = chrono::system_clock::now();
        //scorePrint(sB, gB, start, end, i, j, diff, direction);
    }
}

int main() 
{
    // 時間を計測
    start = chrono::system_clock::now();
    // 抜き型生成
    generateNukigata();

    #if SERVER_RECIEVE
        receive_problem("kamiyama5eb116f8fbfc79f1a32e91d16a2c7aa661e3e0de274f40259259516c");
    #endif

    // Json読み込み
    ifstream ifs(SORT_FILE);
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json J = json::parse(str);
    vec sB, gB;
    loadBoard(J, sB, gB);
    HEIGHT = sB.size(), WIDTH = gB[0].size();
    double matchRate = calculateMatchRate(sB, gB);

    #if EXP
        ifstream ex("./export.json");
        ordered_json exp;
        ex >> exp;
        int n = exp["n"].get<int>();
        //cout << n << endl;
        counter = n;
        rep(i, n) {
            answers.push_back(exp["ops"].at(i));
        }
    #endif

    while (matchRate < 100.0) {
        rep(i, HEIGHT) {
            rep(j, WIDTH) {
                if (sB[i][j] != gB[i][j]) {
                    int target = gB[i][j];
                    updated = false;
                    bool flag = false;

                    int pivot = 0;

                    /* numberの座標を探索 */
                    rep  (xi, number.size()) {
                        pivot += number[xi];

                        for (int di : number) {
                            if (di >= HEIGHT || di <= pivot) break; 
                            di += pivot;

                            if (sB[di + i][j] == gB[i][j]) {
                                pair<int, int> target_idx = {i + di, j};
                                katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                flag = true;
                                break;
                            }
                        }
                        if(flag) break;
                    }

                    pivot = 0;

                    if (!flag) {
                        rep (xi, number.size()) {
                            pivot += number[xi];

                            for (int dj : number) {
                                if (dj >= WIDTH || dj <= pivot) break; 
                                dj += pivot;

                                if (sB[i][dj] == gB[i][j]) {
                                    pair<int, int> target_idx = {i, dj};
                                    katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                    flag = true;
                                    break;
                                }
                            }
                        }
                        if(flag) break;
                    }

                    int pivot1 = 0;
                    int pivot2 = 0;
                    

                    if (!flag) {
                        rep (xi, number.size()) {
                            pivot1 += number[xi];

                            for (int di : number) {
                                if (di == 1) continue;
                                else if (di >= HEIGHT || di <= pivot) break;
                                di += pivot1;

                                rep (xj, number.size()) {
                                    pivot2 += number[xj];

                                    for(int dj : number) {
                                        rep(xj, 2) {
                                            if(xj == 0) dj *= -1;
                                            int curtRow = j + dj;
                                            if(curtRow >= WIDTH || dj <= pivot2) break;
                                            curtRow += pivot2;

                                            if(sB[i + di][curtRow] == gB[i][j]) {
                                                pair<int, int> target_idx = {i + di, curtRow};
                                                if (j >= curtRow) {
                                                    katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 5);
                                                    katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                                } else {
                                                    katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 4);
                                                    katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                                }
                                                flag = true;
                                                break;
                                            }
                                        }
                                    }
                                }
                                if(flag) break;
                            }
                            if(flag) break;
                        }
                    }
                    

                    // 縦方向
                    if(!flag) {
                        rep2(di, i, HEIGHT) {
                            if (sB[di][j] == target) {
                                pair<int, int> target_idx = {di, j};
                                katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                flag = true;
                                break;
                            }
                        }
                    }

                    // 横方向
                    if (!flag) {
                        rep2(dj, j, WIDTH) {
                            if (sB[i][dj] == target) {
                                pair<int, int> target_idx = {i, dj};
                                if (j >= dj) katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 3);
                                else katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 2);

                                flag = true;
                                break;
                            }
                        }
                    }

                    // 両方使う場合
                    if (!flag) {
                        rep2(di, i + 1, HEIGHT) {
                                rep(dj, WIDTH) {
                                    if (sB[di][dj] == target) {
                                        pair<int, int> target_idx = {di, dj};

                                        if (j >= dj) {
                                            katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 5);
                                            katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                        } else {
                                            katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 4);
                                            katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                        }

                                        flag = true;
                                        break;
                                    }
                                }
                            if (flag) break;
                        }
                    }
                }
            }
        }
        matchRate = calculateMatchRate(sB, gB);
    }

    auto end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);

    cout << "\033[31m" << "FINISHED!!" << "\033[m" << " count:" << counter << " time:" << time << endl;

    // 回答JSONの作成
    ordered_json final_answer;
    final_answer["n"] = counter;    // 手数を保存
    final_answer["ops"] = answers;  // 操作履歴を保存

    // 回答JSONをファイルに保存
    ofstream ofs("answer.json");
    ofs << final_answer.dump(4);  // インデント付きでJSONを書き込む

    #if SERVER_SEND
        send_problem("kamiyama5eb116f8fbfc79f1a32e91d16a2c7aa661e3e0de274f40259259516c");
    #endif

    return 0;
}