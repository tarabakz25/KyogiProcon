#include <bits/stdc++.h>
#include "json.hpp"
// #include "json_write.cc" // 使用していないためコメントアウト
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
int HEIGHT, WIDTH;
int use_samplefile = 1;
chrono::system_clock::time_point start, end;

vector<int> number{1, 2, 4, 8, 16, 32, 64, 128, 256};
map<int, vector<vector<int>>> all1_nukigata;

void generateNukigata() /* ぬき型の生成 */
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

double calculateMatchRate(const vector<vector<int>> &startBoard, const vector<vector<int>> &goalBoard) // 一致率計算
{
    int totalElements = 0;
    int matchCount = 0;
    rep(i, startBoard.size())
    {
        rep(j, startBoard[i].size())
        {
            if (startBoard[i][j] == goalBoard[i][j])
                matchCount++;
            totalElements++;
        }
    }
    return (double)matchCount / totalElements * 100.0;
}

void scorePrint(const vector<vector<int>> &startB, const vector<vector<int>> &goalB, const int height, const int width, chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{
    system("clear");

#ifdef PRINTING
    // 型の表示
    rep(i, height)
    {
        rep(j, width)
        {
            if (startB[i][j] == goalB[i][j])
                cout << "\033[31m" << startB[i][j] << "\033[m ";
            else
                cout << startB[i][j] << " ";
        }
        cout << endl;
    }
#endif

    // 一致率, 手数
    end = chrono::system_clock::now();
    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    double score = calculateMatchRate(startB, goalB);
    cout << "Match Rate: " << score << "%" << " " << "Count: " << counter << " " << "Time: " << time << "ms" << endl;

    this_thread::sleep_for(chrono::milliseconds(500)); // 表示のために少し待つ
}

// 型抜きの関数
void up(vector<vector<int>> &startB, int i, int j, int distance)
{
    vector<int> use_kata_size;
    for (int size = 1; size <= 256; size *= 2)
    {
        if (all1_nukigata.find(size) == all1_nukigata.end())
            continue;
        if (size - distance >= 0 && all1_nukigata[size - distance][i][j] == 1)
            use_kata_size.push_back(size);
    }

    for (int n : use_kata_size)
    {
        vector<vector<int>> kata = all1_nukigata[n];
        pair<int, int> point{-1, -1};

        rep2(di, i, n)
        {
            rep2(dj, j, n)
            {
                kata[di][dj] = startB[i + di][j + dj];
                if (startB[i + di + n][j + dj + n] >= 0)
                    startB[i + di][j + dj] = kata[di][dj];
                else if (point.first == -1)
                    point.first = di, point.second = dj;
            }
        }
    }
}

void left(vector<vector<int>> &startB, int i, int j, int distance)
{
}

void right(vector<vector<int>> &startB, int i, int j, int distance)
{
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

void Main()
{
    // 時間を計測
    start = chrono::system_clock::now();

    // Json読み込み
    ifstream ifs("sample2.json");
    if (!ifs.is_open())
    {
        cerr << "ファイルを開くことができませんでした。" << endl;
        return;
    }
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json j = json::parse(str);
    vector<vector<int>> startB, goalB;
    loadBoard(j, startB, goalB);
    WIDTH = j["board"]["width"].get<int>();
    HEIGHT = j["board"]["height"].get<int>();

    double matchRate = calculateMatchRate(startB, goalB);

#ifdef DEBUG
    scorePrint(startB, goalB, height, width, start, end);
#endif

#ifndef DEBUG
    while (matchRate < 100.0)
    {
        bool updated = false; // ボードが更新されたか
        rep(i, HEIGHT)
        {
            rep(j, WIDTH)
            {
                if (startB[i][j] != goalB[i][j])
                {
                    int target = goalB[i][j]; // 目標
                    int foundi = -1, foundj = -1;
                    int distance = INT_MAX;

                    // 縦
                    rep2(ii, 0, HEIGHT)
                    {
                        if (startB[ii][j] == target)
                        {
                            int dist = abs(ii - i);
                            if (dist < distance)
                            {
                                foundi = ii;
                                foundj = j;
                                distance = dist;
                            }
                        }
                    }
                    // 横
                    rep2(jj, 0, WIDTH)
                    {
                        if (startB[i][jj] == target)
                        {
                            int dist = abs(jj - j);
                            if (dist < distance)
                            {
                                foundi = i;
                                foundj = jj;
                                distance = dist;
                            }
                        }
                    }

                    if (foundi == -1)
                    {
                        rep2(ii, 0, HEIGHT)
                        {
                            rep2(jj, 0, WIDTH)
                            {
                                if (startB[ii][jj] == target)
                                {
                                    int dist = abs(ii - i) + abs(jj - j);
                                    if (dist < distance)
                                    {
                                        foundi = ii;
                                        foundj = jj;
                                        distance = dist;
                                    }
                                }
                            }
                        }
                    }

                    if (foundj == -1)
                    {
                    }

                    if (foundi != -1 && foundj != -1)
                    {
                        if (foundj > j)
                        {
                            left(startB, foundi, j, 1);
                        }
                        else if (foundj < j)
                            right(startB, foundi, foundj, 1);

                        if (foundi != i)
                            up(startB, foundi, j, 1);

                        updated = true;
                        break;
                    }
                }
            }
            if (updated)
                break;
        }

        // 一致率を計算
        matchRate = calculateMatchRate(startB, goalB);

        // 無限ループ防止
        if (!updated)
            break;
    }
#endif
}

int main()
{
    generateNukigata();
    Main();

    return 0;
}
