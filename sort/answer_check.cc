/* 
各々の設定はsetting.hhで行ってください。 
*/

#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include "json.hpp"
#include "setting.hh"

typedef long long ll;
#define rep(i, n) for (ll i = 0; i < n; i++)
#define nrep(i, n) for (ll i = n; i >= 0; i--)
#define rep2(i, a, n) for (ll i = a; i < n; i++)

// 省略
using namespace std;
using vec = vector<vector<int>>;
using json = nlohmann::json;

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
json answers = json::array();

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
    vector<int> use_nukigata_size;

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

        // 右だけの場合
        if (direction == 3) {
            rep(di, n) {
                int curtRow = i + di;
                if (curtRow >= HEIGHT)
                    break;
                else {
                    copy(sB[curtRow].begin() + targetj + 1, sB[curtRow].begin() + targetj + n + 1, unplug[di].begin());
                    sB[curtRow].erase(sB[curtRow].begin() + targetj + 1, sB[curtRow].begin() + targetj + n + 1);
                    sB[curtRow].insert(sB[curtRow].begin(), unplug[di].begin(), unplug[di].begin() + n);
                }
            }
        }

        // 左だけの場合
        else if (direction == 2) {
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

        // JSONに保存
        json answer;
        if (pow(2, log2(n)) != n) {
            cerr << "無効なサイズです。nは2の累乗でなければなりません。" << endl;
            exit(-1);
        } else {
            answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
        }
        answer["x"] = j;
        answer["y"] = i;
        answer["s"] = direction > 3 ? direction - 2 : direction;

        answers.push_back(answer);
        auto end = chrono::system_clock::now();
        scorePrint(sB, gB, start, end, i, j, diff, direction);
    }
}

int main() 
{
    // 時間を計測
    start = chrono::system_clock::now();
    // 抜き型生成
    generateNukigata();

    // problem.json読み込み
    ifstream ifs(SORT_FILE);
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json J1 = json::parse(str);
    vec sB, gB;
    loadBoard(J1, sB, gB);
    HEIGHT = sB.size(), WIDTH = gB[0].size();
    double matchRate = calculateMatchRate(sB, gB);

    // answer.json読み込み
    ifstream ifs_answer("./answer.json");
    string str_answer((istreambuf_iterator<char>(ifs_answer)), istreambuf_iterator<char>());
    json J2 = json::parse(str_answer);
    
    int n = J2["n"];

    if(J2["n"].is_null()){
        cerr << "ERROR!" << endl;
        exit(-1);
    }

    cout << J2["ops"].size() << " " << endl;

    int cnt = 0;

    rep(i, HEIGHT){
        rep(j, WIDTH){
            if(sB[i][j] != gB[i][j]){
                rep(di, HEIGHT){
                    rep(dj, WIDTH){
                        if(sB[di][dj] == gB[i][j]){
                            cnt ++;
                            int dj = J2["ops"][cnt]["x"];
                            int di = J2["ops"][cnt]["y"];
                            int direction = J2["ops"][cnt]["s"];

                            int targeti = di;
                            int targetj = dj;
                        
                            katanuki(sB, gB, i, j, targeti, targetj ,direction);
                        }
                    }
                }
            }
        }
    }

    if(matchRate != 100.0) cout << "\033[31m" << "FAILED" << "\033[m" << endl;
    else cout << "\033[31m" << "SUCCESS" << "\033[m" << endl;

    return 0;
}
