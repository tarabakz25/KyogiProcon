//2次元配列 --> 1次元配列について
2次元配列では
board.at(y座標).at(x座標)と表していましたが、

1次元配列では
board.at(x座標　+ 横幅 * y座標)に変換します。

(例4*4の場合)
0    1   2   3
4    5   6   7
8    9  10  11
12  13  14  15


//関数
vector<vector<int>> define_size();
抜き型の縦、横のサイズを配列に入れます。
1, 1
2, 2
2, 2
2, 2
...　のように[縦幅, 横幅]を25個並べた二次元配列を返します。
(一般抜き型はまだ入っていません)

vector<vector<vector<int> > > define_nukigata(vector<vector<int>> size);
define_size();で返ってきた定型抜き型の大きさをもとに、抜き型を作ります。
抜き型は3次元配列です。

vector<int> katanuki(int piece_num, int x_min, int y_min, int direction, vector<vector<int>>& size, vector<vector<vector<int>>>& nukigata, vector<int> board, int BOARD_WIDTH, int BOARD_HEIGHT);
型抜きをします。
katanuki(型番号, 左上のx座標, 左上のy座標, 抜いた後の方向, 一般型の大きさ(define_sizeで返ってきたもの), 抜き型(define_nukigataで返ってきたもの),  型抜きする盤面(通常now_boardです), 盤面の幅, 盤面の高さ);

変えるのは前4つの、"型番号, 左上のx座標, 左上のy座標, 抜いた後の方向"です。

//注意
一般抜き型は実装できていません。関数を一部書き換える必要があります。
20240824佐藤誠
