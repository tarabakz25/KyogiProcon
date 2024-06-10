<各ファイルの説明>
main.cpp main関数があります。
board_setting.cpp 盤面の生成と抜き型の生成、型抜きを行う関数があります。
evaluation.cpp 評価関数があります。

<配列>
2次元配列 size 
size.at(n).at(0) 型番号nのx方向の長さ
siez.at(n).at(1) 型番号nのy方向の長さ


<関数>
[board_setting.cpp]

define_nukigata();
抜き型を生成します。
nukigata.at(型番号).at(y座標).at(x座標) に0,1 で保存されています。

random_start_board();
初期盤面(board)をランダムに生成します。

make_start_board();
テスト用
0123を左上から1つずつ埋めて初期盤面(board)を生成します。
[0][1][2][3][0][1]
[2][3][0][1][2][3]
[0][1][2][3][0][1]
[2][3][0][1][2][3]
[0][1][2][3][0][1]
[2][3][0][1][2][3]

make_finish_board();
テスト用
0123を左上から一つの数字ずつ埋めて最終盤面(finish_board)を作ります。
[0][0][0][0][0][0]
[0][0][0][1][1][1]
[1][1][1][1][1][1]
[2][2][2][2][2][2]
[2][2][2][3][3][3]
[3][3][3][3][3][3]

random_finish_board();
最終盤面(finish_board)をランダムに生成します。

board_save(セーブする場所番号);
現在の盤面(board)を、saved_board.at(セーブする場所)に保存します。

board_road(ロードする場所番号);
saved_board.at(ロードする場所番号)をboardにロードします。

show_board();
現在の盤面(board)を表示します。
最終盤面と一致しているところは黄色になります。

show_saved_board(番号);
saved_boardから引数の場所を表示します。ロードはされません。

show_finish_board();
最終盤面を表示します。

count_match();
現在の盤面と最終盤面を比較し、一致したセルの数を返します。

compare_with0();
0番目にの場所にセーブした盤面と比較し、一致率(%)で返します。
最初の盤面を0番目に保存して(board_save(0);)ください。

katanuki(型番号, X座標, Y座標, 方向);
現在の盤面(board)に対して、型抜きを行います。
方向は
0 上
1 下
2 左
3 右

board_set();
初期状態、完成形をリセットし、再設定します。
