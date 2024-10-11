#include <Siv3D.hpp> // Siv3D v0.6.15
#include <string>
#include <stdlib.h>

#include <string> //read&write
#include <fstream>
#include <sstream>

#include <vector>
#include "json.hpp"
#include "board_setting.cpp"
#include "json_read.cpp"
#include "json_write.cpp"

#include "receive_and_send.cpp"

#include "setting.hpp"
//using namespace nlohman;
using namespace std;
using json = nlohmann::json;

int BOARD_WIDTH;
int BOARD_HEIGHT;

//プロトタイプ宣言
void Board_draw(int position_x, int position_y, int side_length, map<int, int> &board_now, const Font& font);
void check(map<int, int> &board_now, map<int, int> &board_finish, int &num);

vector<vector<int>> define_size();
vector<vector<vector<int> > > define_nukigata(vector<vector<int>> size);
map<int, int> katanuki(int piece_num, int x_min, int y_min, int direction, vector<vector<int>>& size, vector<vector<vector<int>>>& nukigata, map<int, int> board, int BOARD_WIDTH, int BOARD_HEIGHT);

//jsonファイル読み込み系
// 文字列からベクトルに変換する関数(一列分を返す)

void Main()
{
	//gui setting
	//Scene::SetBackground(ColorF{ 0.0, 0.0, 0.0 });
	const int side_length = 5;
    const Font font{ FontMethod::MSDF, 12, Typeface::Bold };
    float persent;
    int number = 0;
    int time = 0;
    
    // ウィンドウを自由にサイズ変更可能に設定
    Window::SetStyle(WindowStyle::Sizable);
    // ウィンドウサイズを 800x600 に設定
    Window::Resize(800, 800);

    //TextEditState te1{ U"0,0,0" };// デフォルトのテキストを設定する
    //String entered_text;

    vector<vector<int>> board_start;
	vector<vector<int>> board_now;
    vector<vector<int>> board_finish;

    json_path_setting();

    receive_problem("token1");

    //jsonファイル用の設定と、jsonファイルの読み込み。
    json_read(board_start, board_finish, BOARD_WIDTH, BOARD_HEIGHT);

    //書き出し用ファイルをリセット
    json_write_reset();

    board_now = board_start;

    int n; //answerの要素数
    vector<vector<int>> num(n, (vector<int> (4)));

    vector<vector<int>> size = define_size();
    vector<vector<vector<int> > > nukigata = define_nukigata(size);
    nukigata = addnukigata(nukigata);

    printNukigata(nukigata); //一般抜き型を表示

    int i = 0;
	//描画開始
	while (System::Update()){
        if (i == n){
            while (1){
                if (KeyEscape.down()){
                    break;
                }
            }
            break;
        }
        Board_draw(40, 40, side_length, board_now, font);

        check(board_now, board_finish, number);
        
        persent = ((float)number / (BOARD_HEIGHT * BOARD_WIDTH)) * 100;
        font(U"一致率{}%"_fmt(persent)).draw(20, Vec2{90 + side_length * BOARD_WIDTH, 200 + side_length * BOARD_HEIGHT}, ColorF{1,1,1});
        font(U"{}手型抜き操作済み"_fmt(time)).draw(20, Vec2{90 + side_length * BOARD_WIDTH, 230 + side_length * BOARD_HEIGHT}, ColorF{1,1,1});

        if (KeyEscape.down()){
            break;
        }

        katanuki();
        i++;
        time++;
	}
    cout << time << endl;
}

//盤面の表示
void Board_draw(int position_x, int position_y, int side_length, vector<vector<int>> &board_now, const Font& font){
    for(int x = 0; x<BOARD_HEIGHT; x++){
            for(int y =0; y<BOARD_WIDTH; y++){
                switch(board_now.at(x).at(y)){
                    case 0:
                        Rect{position_x + side_length * y, position_y + side_length * x, side_length, side_length }.draw(Palette::White);
                        //font(U"0").draw(position_x + 3 + side_length * y, position_y - 1 + side_length * x, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 1:
                        Rect{position_x + side_length * y, position_y + side_length * x, side_length, side_length }.draw(Color(140, 140, 140));
                        //font(U"1").draw(position_x + 3 + side_length * y, position_y - 1 + side_length * x, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 2:
                        Rect{position_x + side_length * y, position_y + side_length * x, side_length, side_length }.draw(Color(100, 100, 100));
                        //font(U"2").draw(position_x + 3 + side_length * y, position_y - 1 + side_length * x, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 3:
                        Rect{position_x + side_length * y, position_y + side_length * x, side_length, side_length }.draw(Color(60, 60, 60));
                        //font(U"3").draw(position_x + 3 + side_length * y, position_y - 1 + side_length * x, ColorF{ 0.0, 0.0, 0.0 });
                        break;
                }
            }
        }
}

void check(map<int, int> &board_now, map<int, int> &board_finish, int &num){
    num = 0;
    for (int i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; i++){
        if (board_now[i] == board_finish[i]){
            num++;
        }
    }
}