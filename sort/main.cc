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
	int size;
	int i;
	int j;
	int direction;
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

void scorePrint(const vec& sB, const vec& gB, chrono::system_clock::time_point start, chrono::system_clock::time_point end)
{

#if PRINTING

	system("clear");

	// 型の表示
	rep(di, HEIGHT)
	{
		rep(dj, WIDTH)
		{
#if !DEBUG
#if !PRINT_COLOR
			if (sB[di][dj] == gB[di][dj])
				cout << "\033[31m" << sB[di][dj] << "\033[m";
			else
				cout << sB[di][dj];
#else
			if(sB[di][dj] == 0) cout << "\033[34m" << sB[di][dj] << "\033[m";
			else if(sB[di][dj] == 1) cout << "\033[33m" << sB[di][dj] << "\033[m";
			else if(sB[di][dj] == 2) cout << "\033[32m" << sB[di][dj] << "\033[m";
            else if(sB[di][dj] == 3) cout << sB[di][dj];
#endif
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
	cout << "MatchRate:" << (int)score << "%" << " " << "Count:" << counter << " " << "time:" << (int)time << "s" << endl;
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

// 型抜き i,jには適応したいどこと
void katanuki(vec& sB, int i, int j, int n, int direction)
{

	vec unplug(HEIGHT, vector<int>(WIDTH, -1)); // 抜き出す数字
	vec push(HEIGHT, vector<int>(WIDTH, -1));	// 寄せる数字

	// 左
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
			else if (j < 0) {
				copy(sB[curtRow].begin(), sB[curtRow].begin() + j + n, unplug[di].begin());
				sB[curtRow].erase(sB[curtRow].begin(), sB[curtRow].begin() + j + n);
				sB[curtRow].insert(sB[curtRow].end(), unplug[di].begin(), unplug[di].begin() + n);
			} else {
				copy(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n, unplug[di].begin());
				sB[curtRow].erase(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n);
				sB[curtRow].insert(sB[curtRow].end(), unplug[di].begin(), unplug[di].begin() + n);
			}
		}
	}

	// 右
	else if (direction == 3) {
		ordered_json answer;
		if (log2(n) != 0) {
			answer["p"] = unsigned(3 * (log2(n) - 1) + 1);
		} else {
			answer["p"] = 0;
		}
		answer["x"] = j - n;
		answer["y"] = i;
		answer["s"] = direction > 3 ? direction - 2 : direction;

		answers.push_back(answer);

		rep(di, n)
		{
			int curtRow = i + di;
			if (curtRow >= HEIGHT)
				break;
			if (j + n >= WIDTH) {
				copy(sB[curtRow].end()-1 + j, sB[curtRow].end(), unplug[di].begin());
				sB[curtRow].erase(sB[curtRow].end()-1 + j, sB[curtRow].end());
				sB[curtRow].insert(sB[curtRow].begin(), unplug[di].begin(), unplug[di].begin() + n);
			} else {
				copy(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n, unplug[di].begin());
				sB[curtRow].erase(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n);
				sB[curtRow].insert(sB[curtRow].begin(), unplug[di].begin(), unplug[di].begin() + n);
			}
		}
	}

	// 上
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
			else if (j < 0)
				copy(sB[curtRow].begin(), sB[curtRow].begin() + j + n, unplug[di].begin());
			else
				copy(sB[curtRow].begin() + j, sB[curtRow].begin() + j + n, unplug[di].begin());
		}
		rep(di, height_diff)
		{
			int curtRow = i + di;
			if (j + n >= WIDTH)
				copy(sB[curtRow + n].begin() + j, sB[curtRow + n].end(), push[di].begin());
			else if (j < 0)
				copy(sB[curtRow].begin(), sB[curtRow].begin() + j + n, push[di].begin());
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
	this_thread::sleep_for(chrono::milliseconds(10));
}

vector<int> use_nukigata(int i, int j, int targeti, int targetj, int direction)
{
	vector<int> use_nukigata_size;
	int diff;

	if (direction == 0)
		diff = targeti - i;
	else if (direction == 2)
		diff = targetj - j;
	else
		diff = j - targetj + 1;

	for (int size : number) {
		if (size <= diff) {
			use_nukigata_size.push_back(size);
			diff -= size;
		}
	}
	if (diff > 0)
		use_nukigata_size.push_back(1);

	return use_nukigata_size;
}

vector<Answer> shuffling(vec& gB)
{
	vector<Answer> shuffling_answers;

	rep(x, 1)
	{
		rep(i, HEIGHT)
		{
			katanuki(gB, i, -255, 256, 2);
			Answer answer = { 256, (int)i, WIDTH + 255, 3 };
			shuffling_answers.push_back(answer);
		}
	}

	return shuffling_answers;
}

int main()
{
	// 時間を計測
	start = chrono::system_clock::now();
	// 抜き型生成
	generateNukigata();

#if SERVER
	receive_problem("kamiyama5eb116f8fbfc79f1a32e91d16a2c7aa661e3e0de274f40259259516c");
#endif

	// Json読み込み
	ifstream ifs(SORT_FILE);
	string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	json J = json::parse(str);
	vec sB, gB;
	loadBoard(J, sB, gB);
	HEIGHT = sB.size(), WIDTH = gB[0].size();

#if SHUFFLE
	const vec no_shuffle_gB = gB;
	vector<Answer> shuffle_answers = shuffling(gB);
#endif

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
				vector<int> sizes;

				if (cost[0].second.first == i) {
					sizes = use_nukigata(i, j, i, cost[0].second.second, 2);
					for (int size : sizes)
						katanuki(sB, i, j, size, 2);
				} else if (cost[0].second.second == j) {
					sizes = use_nukigata(i, j, cost[0].second.first, j, 0);
					for (int size : sizes)
						katanuki(sB, i, j, size, 0);
				} else {
					if (j >= cost[0].second.second) {
						sizes = use_nukigata(i, j, i, cost[0].second.second + 1, 3);
						for (int size : sizes) {
							katanuki(sB, cost[0].second.first, cost[0].second.second + 1, size, 3);
							cost[0].second.second += size;
						}
						sizes = use_nukigata(i, j, cost[0].second.first, j, 0);
						for (int size : sizes)
							katanuki(sB, i, j, size, 0);
					} else {
						sizes = use_nukigata(i, j, i, cost[0].second.second, 2);
						for (int size : sizes)
							katanuki(sB, cost[0].second.first, j, size, 2);
						sizes = use_nukigata(i, j, cost[0].second.first, j, 0);
						for (int size : sizes)
							katanuki(sB, i, j, size, 0);
					}
				}

#if ALL_BREAK
				auto end = chrono::system_clock::now();
				scorePrint(sB, gB, start, end);
#endif
			}
		}
	}

#if SHUFFLE
	for (int i = shuffle_answers.size() - 1; i >= 0; i--) {
		katanuki(sB, shuffle_answers[i].i, shuffle_answers[i].j, shuffle_answers[i].size, shuffle_answers[i].direction);
		auto end = chrono::system_clock::now();
		scorePrint(sB, no_shuffle_gB, start, end);
	}
#endif

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

#if SERVER
	send_problem("kamiyama5eb116f8fbfc79f1a32e91d16a2c7aa661e3e0de274f40259259516c");
#endif

	return 0;
}