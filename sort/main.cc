/*
各々の設定はsetting.hhで行ってください。
*/

#include "json.hpp"
#include "receive_and_send.cpp"
#include <curl/curl.h>
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
using ordered_json = nlohmann::ordered_json;

/* 変数の先宣言 */
int counter = 0;	  // 手数のカウント
bool updated = false; // ボードが更新されたかを確認するフラグ
double matchRate;	  // 一致率
int HEIGHT, WIDTH;
chrono::system_clock::time_point start, end;
vector<int> number{ 256, 128, 64, 32, 16, 8, 4, 2, 1 };

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

int nukigata_size_cost(int diffi, int diffj)
{
	int cost = 0;
	for (int size : number) {
		if (size <= diffi) {
			cost++;
			diffi -= size;
		}
	}
	if (diffi > 0)
		cost++;
	for (int size : number) {
		if (size <= diffj) {
			cost++;
			diffj -= size;
		}
	}
	if (diffj > 0)
		cost++;

	return cost;
}

double calculateMatchRate(const vec& sB, const vec& gB)
{
	int totalElements = 0;
	int matchCount = 0;
	rep(di, HEIGHT)
	{
		rep(dj, WIDTH)
		{
			if (sB[di][dj] == gB[di][dj])
				matchCount++;
			totalElements++;
		}
	}
	return (double)matchCount / totalElements * 100.0;
}

void scorePrint(const vec& sB, const vec& gB, chrono::system_clock::time_point start, chrono::system_clock::time_point end, int i, int j, int diff, int direction)
{

#if PRINTING

	system("clear");

	// 型の表示
	rep(di, HEIGHT)
	{
		rep(dj, WIDTH)
		{
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

vector<int> stringToVector(const string& str)
{
	vector<int> row;
	for (char c : str)
		row.push_back(c - '0');
	return row;
}

void loadBoard(const json& j, vec& sB, vec& gB)
{
	for (const auto& line : j["board"]["start"])
		sB.push_back(stringToVector(line.get<string>()));
	for (const auto& line : j["board"]["goal"])
		gB.push_back(stringToVector(line.get<string>()));
}

// 型抜き i,jには異点の座標、targetには目標座標を送る。
void katanuki(vec& sB, vec& gB, int i, int j, int targeti, int targetj, int direction)
{
	vector<int> use_nukigata_size; // 使う抜き型のサイズ

	int diff = direction == 0 ? diff = targeti - i : direction == 2 || direction == 4 ? diff = targetj - j : diff = j - targetj;

	for (int size : number) {
		if (size <= diff) {
			use_nukigata_size.push_back(size);
			diff -= size;
		}
	}
	if (diff > 0)
		use_nukigata_size.push_back(1);

	for (int n : use_nukigata_size) {
		vec unplug(HEIGHT, vector<int>(WIDTH, -1)); // 抜き出す数字
		vec push(HEIGHT, vector<int>(WIDTH, -1));	// 寄せる数字

		// 左だけの場合
		if (direction == 2) {
			ordered_json answer;
			if (log2(n) != 0) {
				answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
			} else {
				answer["p"] = 0;
			}
			answer["x"] = j;
			answer["y"] = i;
			answer["s"] = direction > 3 ? direction - 2 : direction;

			answers.push_back(answer);

			rep(di, n)
			{
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
			if (log2(n) != 0) {
				answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
			} else {
				answer["p"] = 0;
			}
			answer["x"] = targetj + 1;
			answer["y"] = targeti;
			answer["s"] = direction > 3 ? direction - 2 : direction;
			
			answers.push_back(answer);

			rep(di, n)
			{
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
			ordered_json answer;
			if (log2(n) != 0) {
				answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
			} else {
				answer["p"] = 0;
			}
			answer["x"] = j;
			answer["y"] = targeti;
			answer["s"] = direction > 3 ? direction - 2 : direction;
			
			answers.push_back(answer);

			rep(di, n)
			{
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

			ordered_json answer;
			if (log2(n) != 0) {
				answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
			} else {
				answer["p"] = 0;
			}
			answer["x"] = j;
			answer["y"] = i;
			answer["s"] = direction > 3 ? direction - 2 : direction;
			
			answers.push_back(answer);

			rep(di, n)
			{
				int curtRow = i + di;
				if (j + n >= WIDTH)
					copy(sB[curtRow].begin() + j, sB[curtRow].end(), unplug[di].begin());
				else
					copy(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n, unplug[di].begin());
			}
			rep(di, height_diff)
			{
				int curtRow = i + di;
				if (j + n >= WIDTH)
					copy(sB[curtRow + n].begin() + j, sB[curtRow + n].end(), push[di].begin());
				else
					copy(sB[curtRow + n].begin() + j, sB[curtRow + n].begin() + j + n, push[di].begin());
			}
			rep(di, height_diff)
			{
				if (push[di][0] == -1)
					break;
				rep(dj, n)
				{
					if (push[di][dj] == -1)
						break;
					else
						sB[i + di][j + dj] = push[di][dj];
				}
			}
			rep(di, n)
			{
				if (unplug[di][0] == -1)
					break;
				rep(dj, n)
				{
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
		//this_thread::sleep_for(chrono::seconds(1));
	}
}

int main()
{
	// 時間を計測
	start = chrono::system_clock::now();
	// 抜き型生成
	generateNukigata();

#if SERVER_GET
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

	rep(i, HEIGHT)
	{
		rep(j, WIDTH)
		{
			if (sB[i][j] != gB[i][j]) {
				int target = gB[i][j];
				bool flag = false;

				// 評価をかけるための変数
				vector<pair<int, pair<int, int>>> cost;

				// 全探索をかける。
				rep2(di, i, HEIGHT)
				{
					if (di == i) {
						rep2(dj, j, WIDTH)
						{
							int now = sB[di][dj];

							if (now == target) {
								int katanuki_cost = nukigata_size_cost(di - i, abs(dj - j));
								cost.push_back(make_pair(katanuki_cost, make_pair(di, dj)));

								if (katanuki_cost == 1) {
									flag = true;
									break;
								}
							}
						}
					} else {
						rep(dj, WIDTH)
						{
							int now = sB[di][dj];

							if (now == target) {
								int katanuki_cost = nukigata_size_cost(di - i, abs(dj - j));
								cost.push_back(make_pair(katanuki_cost, make_pair(di, dj)));

								if (katanuki_cost == 1) {
									flag = true;
									break;
								}
							}
						}
					}
					if (flag)
						break;
				}

				sort(cost.begin(), cost.end());

				if (cost[0].second.first == i)
					katanuki(sB, gB, i, j, 0, cost[0].second.second, 2);
				else if (cost[0].second.second == j)
					katanuki(sB, gB, i, j, cost[0].second.first, 0, 0);
				else {
					if (j >= cost[0].second.second) {
						katanuki(sB, gB, i, j, cost[0].second.first, cost[0].second.second, 5);
						katanuki(sB, gB, i, j, cost[0].second.first, cost[0].second.second, 0);
					} else {
						katanuki(sB, gB, i, j, cost[0].second.first, cost[0].second.second, 4);
						katanuki(sB, gB, i, j, cost[0].second.first, cost[0].second.second, 0);
					}
				}
			}
		}
	}

	auto end = chrono::system_clock::now();
	double time = static_cast<double>(chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000.0);

	cout << "\033[31m" << "FINISHED!!" << "\033[m" << " count:" << counter << " time:" << time << " match:" << calculateMatchRate(sB, gB) << endl;

	// 回答JSONの作成
	ordered_json final_answer;
	final_answer["n"] = counter;   // 手数を保存
	final_answer["ops"] = answers; // 操作履歴を保存

	// 回答JSONをファイルに保存
	ofstream ofs("answer.json");
	ofs << final_answer.dump(4); // インデント付きでJSONを書き込む

#if SERVER_POST
	send_problem("kamiyama5eb116f8fbfc79f1a32e91d16a2c7aa661e3e0de274f40259259516c");
#endif

	return 0;
}