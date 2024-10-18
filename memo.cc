#include <bits/stdc++.h>
#include "./sort/json.hpp"
#include "./sort/setting.hh"
#define rep1(i, a, n) for(long long i = a; i < n; i++)
#define rep2(i, a, n, b) for(long long i = a; i < n; i += b)
#define nrep(i, n) for(long long i = n; i > 0; i--)

using namespace std;
typedef long long ll;
typedef vector<vector<int>> vec;
using json = nlohmann::json;

vector<int> stringToVector(const string& str)
{
	vector<int> row;
	for (char c : str)
		row.push_back(c - '0');
	return row;
}

void loadBoard(const json& j, vec& sB, vec& gB)
{
	for (const auto& line : j["problem"]["board"]["start"])
		sB.push_back(stringToVector(line.get<string>()));
	for (const auto& line : j["problem"]["board"]["goal"])
		gB.push_back(stringToVector(line.get<string>()));
}


int main()
{
	// Json読み込み
	ifstream ifs(SORT_FILE);
	string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
	json J = json::parse(str);
	vec sB, gB;
	loadBoard(J, sB, gB);
	int HEIGHT = sB.size(), WIDTH = gB[0].size();

#if PRINTING

	system("clear");

	// 型の表示
	rep1(di, 0, HEIGHT)
	{
		rep1(dj, 0, WIDTH)
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
}