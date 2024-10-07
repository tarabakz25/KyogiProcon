#include <bits/stdc++.h>
#include "json.hpp"
typedef long long ll;

#define rep(i, n) for (ll i = 0; i < n; i++)
#define nrep(i, n) for (ll i = n; i >= 0; i--)
#define rep2(i, a, n) for (ll i = a; i < n; i++)

// #define DEBUG
#define PRINTING

using namespace std;
using json = nlohmann::json;

/* 変数の先宣言 */
int counter = 0; // 手数のカウント
double matchRate;
int HEIGHT, WIDTH;
int use_samplefile = 1;
chrono::system_clock::time_point start, end;
bool updated = false; // ボードが更新されたかを確認するフラグ
vector<int> number{1, 2, 4, 8, 16, 32, 64, 128, 256};

/* 回答用構造体 */
struct Answer{
    int move_num;
    int kata_num;
    int x;
    int y;
    int direction;
};
json answers = json::array();

/* ぬき型たち */
map<int, vector<vector<int>>> all1_nukigata;

/* ぬき型の生成 */
void generateNukigata()
{
    ll i, j;
    for (int size = 1; size <= 256; size *= 2)
    {
        vector<vector<int>> kata(size, vector<int>(size));

        rep(i, size)
        {
            rep(j, size)
            {
                kata[i][j] = 1;
            }
        }
        all1_nukigata[size] = kata;
    }
}

// 一致率計算
double calculateMatchRate(const vector<vector<int>> &startBoard, const vector<vector<int>> &goalBoard)
{
    int totalElements = 0;
    int matchCount = 0;
    rep(i, startBoard.size())
    {
        rep(j, startBoard[i].size())
        {
            if (startBoard[i][j] == goalBoard[i][j]) matchCount++;
            totalElements++;
        }
    }
    return (double)matchCount / totalElements * 100.0;
}

void scorePrint(const vector<vector<int>> &startB, const vector<vector<int>> &goalB, chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{
    system("clear");

#ifdef PRINTING
    // 型の表示
    rep(i, HEIGHT){
        rep(j, WIDTH){
            if (startB[i][j] == goalB[i][j]) cout << "\033[31m" << startB[i][j] << "\033[m";
            else cout << startB[i][j];
            cout << " ";
        }
        cout << endl;
    }
    cout << endl; 
#endif

    // 一致率, 手数
    end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    double score = calculateMatchRate(startB, goalB);
    cout << "Match Rate: " << score << "%" << " " << "Count: " << counter << " " << "time: " << time << endl;
}

void up(vector<vector<int>> &startB, vector<vector<int>> &goalB, int i, int j, int iCount, int jCount)
{
    vector<int> use_kata_size;
    int total_size = iCount;

    // 抜き型サイズの決定
    for (int i = number.size() - 1; i >= 0; i--) {
        int n = number[i];
        while (total_size >= n) {
            use_kata_size.push_back(n);
            total_size -= n;
        }
    }
    if (total_size > 0) {
        use_kata_size.push_back(total_size);
    }

    for (int n : use_kata_size){
        // 終点からの距離
        unsigned int height_diff = HEIGHT - n - i;
        unsigned int width_diff = WIDTH - n - j;

        vector<vector<int>> unplug_num(n, vector<int>(n)); // 押し出した数字
        vector<vector<int>> push_num(height_diff, vector<int>(n)); // 寄せる数字

        for(int ii = 0; ii < n; ii++){
            for(int jj = 0; jj < n; jj++){
                if(jj <= WIDTH) unplug_num[ii][jj] = startB[i + ii][j + jj];
                else break;
            }
        }
        for(int ii = 0; ii < height_diff; ii++){
            for(int jj = 0; jj < n; jj++){
                push_num[ii][jj] = startB[i + n + ii][jj + n + j];
            }
        }
        // 上にシフト
        for(int ii = 0; ii < height_diff; ii++){
            for(int jj = 0; jj < push_num[0].size(); jj++){
                startB[i + ii][j + jj] = push_num[ii][jj];
            }
        }
        for(int ii = 0; ii < n; ii++){
            for(int jj = 0; jj < unplug_num[0].size(); jj++){
                startB[i + n + ii][j + n + jj] = unplug_num[ii][jj];
            }
        }
        counter++;
        
        // JSONに保存
        json answer;
        answer["move_number"] = counter;
        answer["kata_number"] = 3 * (n - 2) + 1;
        answer["x"] = j;
        answer["y"] = i;
        answer["direction"] = 0;

        answers.push_back(answer);
    }

    auto end = chrono::system_clock::now();
    scorePrint(startB, goalB, start, end);
    if(matchRate < calculateMatchRate(startB, goalB)) updated = true;
}


void right(vector<vector<int>> &startB, vector<vector<int>> &goalB, int i, int j, int iCount, int jCount)
{
    vector<int> use_kata_size;
    int total_size = jCount;

    // 抜き型サイズの決定
    for (int i = number.size() - 1; i >= 0; i--) {
        int n = number[i];
        while (total_size >= n) {
            use_kata_size.push_back(n);
            total_size -= n;
        }
    }
    if (total_size > 0) {
        use_kata_size.push_back(total_size);
    }


    for (int n : use_kata_size){
        // 終点からの距離
        unsigned int height_diff = HEIGHT - n;
        unsigned int width_diff = j - n;

        vector<vector<int>> unplug_num(n, vector<int>(n)); // 押し出した数字
        vector<vector<int>> push_num(height_diff, vector<int>(width_diff)); // 寄せる数字

        for(int ii = 0; ii < n; ii++){
            if(HEIGHT < height_diff + ii) break;
            for(int jj = 0; jj < n; jj++){
                unplug_num[ii][jj] = startB[height_diff + ii][width_diff + jj];
            }
        }
        for(int ii = 0; ii < height_diff; ii++){
            for(int jj = 0; jj < width_diff; jj++){
                push_num[ii][jj] = startB[i + ii][jj];
            }
        }
        // 右にシフト
        for(int ii = 0; ii < height_diff; ii++){
            for(int jj = 0; jj < n; jj++){
                startB[i + ii][n + jj] = push_num[ii][jj];
            }
        }
        // 押し出した数字を後ろにシフト
        for(int ii = 0; ii < n; ii++){
            if(HEIGHT < height_diff + ii) break;
            for(int jj = 0; jj < n; jj++){
                startB[i + ii][jj] = unplug_num[ii][jj];
            }
        }
        counter++;

        // JSONに保存
        json answer;
        answer["move_number"] = counter;
        answer["kata_number"] = 3 * (n - 2) + 1;
        answer["x"] = j;
        answer["y"] = i;
        answer["direction"] = 2; 

        answers.push_back(answer);
    }

    auto end = chrono::system_clock::now();
    scorePrint(startB, goalB, start, end);
    if(matchRate < calculateMatchRate(startB, goalB)) updated = true;
}


void left(vector<vector<int>> &startB, vector<vector<int>> &goalB, int i, int j, int iCount, int jCount)
{
    vector<int> use_kata_size;
    int total_size = jCount;

    // 抜き型サイズの決定
    for (int i = number.size() - 1; i >= 0; i--) {
        int n = number[i];
        while (total_size >= n) {
            use_kata_size.push_back(n);
            total_size -= n;
        }
    }
    if (total_size > 0) {
        use_kata_size.push_back(total_size);
    }


    for (int n : use_kata_size){
        // 終点からの距離
        unsigned int height_diff = HEIGHT - i - n;
        unsigned int width_diff = WIDTH - j - n;

        vector<vector<int>> unplug_num(n, vector<int>(n)); // 押し出した数字
        vector<vector<int>> push_num(height_diff, vector<int>(width_diff)); // 寄せる数字

        for(int ii = 0; ii < n; ii++){
            if(HEIGHT < height_diff + ii) break;
            for(int jj = 0; jj < n; jj++) {
                unplug_num[ii][jj] = startB[i + ii][j + jj];
            }
        }
        for(int ii = 0; ii < height_diff; ii++){
            for(int jj = 0; jj < width_diff; jj++){
                push_num[ii][jj] = startB[i + n + ii][j + n + jj];
            }
        }
        // 左にシフト
        for(int ii = 0; ii < height_diff; ii++){
            for(int jj = 0; jj < width_diff; jj++){
                startB[i + ii][j + jj] = push_num[ii][jj];
            }
        }

        for(int ii = 0; ii < n; ii++){
            if(HEIGHT < height_diff + ii) break;
            for(int jj = 0; jj < n; jj++){
                startB[i + ii][j + n + jj] = unplug_num[ii][jj];
            }
        }
        counter++;

        // JSONに保存
        json answer;
        answer["move_number"] = counter;
        answer["kata_number"] = 3 * (n - 2) + 1;
        answer["x"] = j;
        answer["y"] = i;
        answer["direction"] = 1;

        answers.push_back(answer);
    }

    auto end = chrono::system_clock::now();
    scorePrint(startB, goalB, start, end);
    if(matchRate < calculateMatchRate(startB, goalB)) updated = true;
}



vector<int> stringToVector(const string &str)
{
    vector<int> row;
    for (char c : str)
        row.push_back(c - '0');

    return row;
}

// 型の読み込み
void loadBoard(const json &j, vector<vector<int>> &startB, vector<vector<int>> &goalB)
{
    for (const auto &line : j["board"]["start"])
        startB.push_back(stringToVector(line.get<string>()));
    for (const auto &line : j["board"]["goal"])
        goalB.push_back(stringToVector(line.get<string>()));
}

// 探索中のボードを表示。　使うことは多分ないかも
void findingPrint(const vector<vector<int>> &startB, const vector<vector<int>> &goalB, const int i, const int j, chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{
    system("clear");

    rep(ii, startB.size())
    {
        rep(jj, startB[0].size())
        {
            if (ii == i && jj == j)
                cout << "\033[7m" << startB[ii][jj] << "\033[m" << " ";
            else
                cout << startB[ii][jj] << " ";
        }
        cout << endl;
    }

    // 一致率, 手数
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
    WIDTH = j["board"]["width"].get<int>();
    HEIGHT = j["board"]["height"].get<int>();

    double matchRate = calculateMatchRate(startB, goalB);

    while (matchRate < 100.0){
        rep(i, HEIGHT){
            rep(j, WIDTH){
                if (startB[i][j] != goalB[i][j]){
                    updated = false;
                    bool flag = false;
                    //縦方向
                    rep2(ii, i, HEIGHT){
                        if(startB[ii][j] == goalB[i][j]){
                            up(startB, goalB, i, j, ii, 0);
                            flag = true;
                            break;
                        }
                    }
                    //横方向
                    if(!flag){
                        rep2(jj, j, WIDTH){
                            if(startB[i][jj] == goalB[i][j]){
                                if(i > jj) right(startB, goalB, i, j, 0, jj);
                                else left(startB, goalB, i, j, 0, jj);
                                flag = true;
                                break;
                            }
                        }
                    }
                    //両方使う場合
                    if(!flag){
                        rep2(ii, i + 1, HEIGHT){
                            rep2(jj, j + 1, WIDTH){
                                if(startB[ii][jj] == goalB[i][j]){
                                    up(startB, goalB, i, j, ii, jj);
                                    if(j > jj) right(startB, goalB, i, j, ii, jj);
                                    else left(startB, goalB, i, j, ii, jj);
                                    flag = true;
                                    break;
                                }
                            }
                            if(flag) break;
                        }
                    }
                }
            }
        }
        // 一致率を計算
        matchRate = calculateMatchRate(startB, goalB);
        // スコアを表示
        auto end = chrono::system_clock::now();
        scorePrint(startB, goalB, start, end);
        if (!updated) break;
    }
    cout << "\033[31m" << "FINISHED!!" << "\033[m]" << endl;
}

int main()
{
    generateNukigata();
    Main();

    // 操作履歴をファイルに保存
    ofstream ofs("answer.json");

    return 0;
}
