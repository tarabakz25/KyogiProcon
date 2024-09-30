#ifndef BOARD_SETTING_HPP
#define BOARD_SETTING_HPP

#include <vector>
using namespace std;

// 変数の宣言
extern int BOARD_WIDTH;
extern int BOARD_HEIGHT;

// 関数のプロトタイプ宣言
vector<vector<int>> define_size();
vector<vector<vector<int> > > define_nukigata(vector<vector<int>> size);
vector<int> katanuki(int piece_num, int x_min, int y_min, int direction, vector<vector<int>>& size, vector<vector<vector<int>>>& nukigata, vector<int> board, int BOARD_WIDTH, int BOARD_HEIGHT);


#endif // BOARD_SETTING_HPP