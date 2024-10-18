/*
各々の設定はsetting.hhで行ってください。
*/

#include "json.hpp"
#include "setting.hh"
#include <bits/stdc++.h>
#include <chrono>
#include <thread>

typedef long long ll;
#define rep(i, n) for (ll i = 0; i < n; i++)
#define nrep(i, n) for (ll i = n; i >= 0; i--)
#define rep2(i, a, n) for (ll i = a; i < n; i++)

// 省略
using namespace std;
using vec = vector<vector<int>>;
using json = nlohmann::json;

/* 変数の先宣言 */
int counter = 0;      // 手数のカウント
bool updated = false; // ボードが更新されたかを確認するフラグ
double matchRate;     // 一致率
int HEIGHT, WIDTH;
chrono::system_clock::time_point start, end;
vector<int> number1{256, 128, 64, 32, 16, 8, 4, 2, 1};

// 探索用の数字
vec search_number{
    {256, 128, 64, 32, 16, 8, 4, 2, 1},
    {192, 160, 144, 136, 132, 130, 129, 96, 80, 72, 68, 66, 65, 48, 40, 36, 34, 33, 24, 20, 18, 17, 12, 10, 9, 6, 5, 3},
    {224, 208, 200, 196, 194, 193, 192, 176, 168, 164, 162, 161, 160, 152, 148, 146, 145, 144, 140, 138, 137, 136, 134, 133, 132, 131, 130, 129, 128, 112, 104, 100, 98, 97, 96, 88, 84, 82, 81, 80, 76, 74, 73, 72, 70, 69, 68, 67, 66, 65, 64, 56, 52, 50, 49, 48, 44, 42, 41, 40, 38, 37, 36, 35, 34, 33, 32, 28, 26, 25, 24, 22, 21, 20, 19, 18, 17, 16, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4},
    {240, 232, 228, 226, 225, 224, 216, 212, 210, 209, 208, 204, 202, 201, 200, 198, 197, 196, 195, 194, 193, 192, 184, 180, 178, 177, 176, 172, 170, 169, 168, 166, 165, 164, 163, 162, 161, 160, 156, 154, 153, 152, 150, 149, 148, 147, 146, 145, 144, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 120, 116, 114, 113, 112, 108, 106, 105, 104, 102, 101, 100, 99, 98, 97, 96, 92, 90, 89, 88, 86, 85, 84, 83, 82, 81, 80, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 60, 58, 57, 56, 54, 53, 52, 51, 50, 49, 48, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5},
};

/* 回答用構造体 */
struct Answer {
    int n;
    int p;
    int x;
    int y;
    int s;
};
json answers = json::array();

/* ぬき型たち */
map<int, vec> all1_nukigata;

// システムの関数
void generateNukigata() {
    for (int size : number1) {
        vec kata(size, vector<int>(size, 1));
        all1_nukigata[size] = kata;
    }
}

double calculateMatchRate(const vec &sB, const vec &gB) {
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

void scorePrint(const vec &sB, const vec &gB, chrono::system_clock::time_point start, chrono::system_clock::time_point end, int i, int j, int diff, int direction) {

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
    cout << "MatchRate:" << (int)score << "%" << " " << "Count:" << counter << " di: " << direction << " time:" << (int)time << "s" << " now:(" << i << ", " << j << ")" << endl;
}

vector<int> stringToVector(const string &str) {
    vector<int> row;
    for (char c : str)
        row.push_back(c - '0');
    return row;
}

void loadBoard(const json &j, vec &sB, vec &gB) {
    for (const auto &line : j["board"]["start"])
        sB.push_back(stringToVector(line.get<string>()));
    for (const auto &line : j["board"]["goal"])
        gB.push_back(stringToVector(line.get<string>()));
}

// 型抜き i,jには異点の座標、targetには目標座標を送る。
void katanuki(vec &sB, vec &gB, int i, int j, int targeti, int targetj, int direction) {
    vector<int> use_nukigata_size; // 使う抜き型のサイズ

    int diff = direction == 0 ? diff = targeti - i : direction == 2 || direction == 4 ? diff = targetj - j
                                                                                      : diff = j - targetj;

    for (int size : number1) {
        if (size <= diff) {
            use_nukigata_size.push_back(size);
            diff -= size;
        }
    }
    if (diff > 0) use_nukigata_size.push_back(1);

    for (int n : use_nukigata_size) {
        vec unplug(HEIGHT, vector<int>(WIDTH, -1)); // 抜き出す数字
        vec push(HEIGHT, vector<int>(WIDTH, -1));   // 寄せる数字

        // 左だけの場合
        if (direction == 2) {
            json answer;
            answer["x"] = j;
            answer["y"] = i;
            answer["s"] = direction > 3 ? direction - 2 : direction;
            answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
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
            json answer;
            answer["x"] = targetj + 1;
            answer["y"] = targeti;
            answer["s"] = direction > 3 ? direction - 2 : direction;
            answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
            answers.push_back(answer);

            rep(di, n) {
                int curtRow = targeti + di;
                if (curtRow >= HEIGHT)
                    break;
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
            json answer;
            answer["x"] = targetj;
            answer["y"] = targeti - n;
            answer["s"] = direction > 3 ? direction - 2 : direction;
            answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
            answers.push_back(answer);

            rep(di, n) {
                int curtRow = targeti + di;
                if (curtRow >= HEIGHT)
                    break;
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

            json answer;
            answer["x"] = j;
            answer["y"] = i;
            answer["s"] = direction > 3 ? direction - 2 : direction;
            answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
            answers.push_back(answer);

            rep(di, n) {
                int curtRow = i + di;
                if (j + n >= WIDTH)
                    copy(sB[curtRow].begin() + j, sB[curtRow].end(), unplug[di].begin());
                else
                    copy(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n, unplug[di].begin());
            }
            rep(di, height_diff) {
                int curtRow = i + di;
                if (j + n >= WIDTH)
                    copy(sB[curtRow + n].begin() + j, sB[curtRow + n].end(), push[di].begin());
                else
                    copy(sB[curtRow + n].begin() + j, sB[curtRow + n].begin() + j + n, push[di].begin());
            }
            rep(di, height_diff) {
                if (push[di][0] == -1) break;
                rep(dj, n) {
                    if (push[di][dj] == -1)
                        break;
                    else
                        sB[i + di][j + dj] = push[di][dj];
                }
            }
            rep(di, n) {
                if (unplug[di][0] == -1) break;
                rep(dj, n) {
                    if (unplug[di][dj] == -1)
                        break;
                    else
                        sB[i + height_diff + di][j + dj] = unplug[di][dj];
                }
            }
        } else {
            cerr << "ERROR!" << endl;
        }

        counter++;

        auto end = chrono::system_clock::now();

#if ALL_BREAK
        scorePrint(sB, gB, start, end, i, j, diff, direction);
#endif
    }
}

int main() {
    // 時間を計測
    start = chrono::system_clock::now();
    // 抜き型生成
    generateNukigata();

    // Json読み込み
    ifstream ifs(SORT_FILE);
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json J = json::parse(str);
    vec sB, gB;
    loadBoard(J, sB, gB);
    HEIGHT = sB.size(), WIDTH = gB[0].size();
    double matchRate = calculateMatchRate(sB, gB);

    while (matchRate < 100.0) {
        rep(i, HEIGHT) {
            rep(j, WIDTH) {
                if (sB[i][j] != gB[i][j]) {
                    int target = gB[i][j];
                    updated = false;
                    bool flag = false;

                    /* numberの座標を探索 */
                    rep(xi, search_number.size()) {
                        for (int di : search_number[xi]) {
                            if (di >= HEIGHT) break;

                            if (sB[di + i][j] == gB[i][j]) {
                                pair<int, int> target_idx = {i + di, j};
                                katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                flag = true;
                                break;
                            }
                        }
                        if (flag) break;
                    }

                    if (!flag) {
                        rep(xi, search_number.size()) {
                            for (int dj : search_number[xi]) {
                                if (dj >= WIDTH) break;

                                if (sB[i][dj] == gB[i][j]) {
                                    pair<int, int> target_idx = {i, j + dj};
                                    if (j >= dj)
                                        katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 1);
                                    else
                                        katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 0);
                                    flag = true;
                                    break;
                                }
                            }
                            if (flag) break;
                        }
                    }

                    if (!flag) {
                        rep(xi, search_number.size()) {
                            for (int di : search_number[xi]) {
                                if (di == 1)
                                    continue;
                                else if (di >= HEIGHT)
                                    break;

                                rep(xj, search_number.size()) {
                                    for (int dj : search_number[xj]) {
                                        rep(xj, 2) {
                                            int curtRow;

                                            if (xj == 0)
                                                curtRow = j + dj;
                                            else
                                                curtRow = j - dj;
                                            if (curtRow >= WIDTH || curtRow <= 0)
                                                continue;
                                            else if (curtRow >= WIDTH && curtRow <= 0)
                                                break;

                                            if (sB[i + di][curtRow] == gB[i][j]) {
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
                            }
                        }
                    }

                    // 縦方向
                    if (!flag) {
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
                                if (j >= dj)
                                    katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 3);
                                else
                                    katanuki(sB, gB, i, j, target_idx.first, target_idx.second, 2);

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

    chrono::system_clock::time_point end_point;
    end_point = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::milliseconds>(end_point - start).count() / 1000.0);

    cout << "\033[31m" << "FINISHED!!" << "\033[m count:" << counter << " time:" << time << endl;

    // 回答JSONの作成
    json final_answer;
    final_answer["n"] = counter;   // 手数を保存
    final_answer["ops"] = answers; // 操作履歴を保存

    // 回答JSONをファイルに保存
    ofstream ofs("answer.json");
    ofs << final_answer.dump(4); // インデント付きでJSONを書き込む

    return 0;
}