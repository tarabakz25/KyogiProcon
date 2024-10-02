//#include <Siv3D.hpp> // Siv3D v0.6.15
#include <string>
#include <stdlib.h>
#include <chrono>

#include <string> //read&write
#include <fstream>
#include <sstream>
#include <filesystem>

#include <vector>
#include "json.hpp"
#include "board_setting.cpp"
#include "json_read.cpp"
#include "json_write.cpp"
#include "operation.cpp"

#include "receive.cpp"

#include "setting.hpp"
//using namespace nlohman;
using namespace std;
using json = nlohmann::json;

int BOARD_WIDTH;
int BOARD_HEIGHT;

//プロトタイプ宣言
//void Board_draw(int position_x, int position_y, int side_length, vector<int> &board_now, const Font& font);
//void comparison(int block_type, int now_x, int now_y, int end_x, int end_y, vector<int>& board_now, vector<int>& board_finish, vector<int>& blockcheck_now, vector<int>& blockcheck_end, vector<vector<int>>& blockcheck_result);
void board_sort_search(vector<int>& board_now, vector<int>&board_finish, pair<pair<int, int>, pair<int, int>> &sort_result); //ところp
//void number_draw_now (int number, bool num, vector<int> &board_now, int side_length);
//void number_draw_finish (int number, bool num, vector<int> &board_finish, int side_length);
void check(vector<int> &board_now, vector<int> &board_finish, int &num);    //ところp
//void check_result_draw (vector<int> &board_now, vector<int> &board_finish, int side_length);

vector<vector<int>> define_size();
vector<vector<vector<int> > > define_nukigata(vector<vector<int>> size);
vector<int> katanuki(int piece_num, int x_min, int y_min, int direction, vector<vector<int>>& size, vector<vector<vector<int>>>& nukigata, vector<int> board, int BOARD_WIDTH, int BOARD_HEIGHT);

//jsonファイル読み込み系
// 文字列からベクトルに変換する関数(一列分を返す)

int main()
{
    auto now = std::chrono::high_resolution_clock::now();
	//gui setting
	//Scene::SetBackground(ColorF{ 0.0, 0.0, 0.0 });
	//const int side_length = 14;
    //const Font font{ FontMethod::MSDF, 12, Typeface::Bold };
    float persent;
    float sort_persent;
    int number;
    int sort_num;
    int time = 0;
    
    // ウィンドウを自由にサイズ変更可能に設定
    //Window::SetStyle(WindowStyle::Sizable);
    // ウィンドウサイズを 800x600 に設定
    //Window::Resize(800, 800);

    //TextEditState te1{ U"0,0,0" };// デフォルトのテキストを設定する
    //String entered_text;

    vector<int> board_start (BOARD_WIDTH * BOARD_HEIGHT, 0);
	vector<int> board_now (BOARD_WIDTH * BOARD_HEIGHT, 0);
    vector<int> board_finish (BOARD_WIDTH * BOARD_HEIGHT, 0);
    vector<vector<int>> blockcheck_result;
    pair<pair<int, int>, pair<int, int>> sort_result;

    sort_result.first = {0, 0};
    sort_result.second = {-1, 0};

	//board_setting(random)
	/*for(int i = 0; i < BOARD_HEIGHT; i++){
		for(int j = 0; j < BOARD_WIDTH; j++){
			board_start.at(BOARD_WIDTH * i + j) = rand() % 4;
		}
	}
    for(int i = 0; i < BOARD_HEIGHT; i++){
		for(int j = 0; j < BOARD_WIDTH; j++){
			board_finish.at(BOARD_WIDTH * i + j) = rand() % 4;
		}*/

    json_path_setting();

    receive_problem();

    //jsonファイル用の設定と、jsonファイルの読み込み。
    json_read(board_start, board_finish, BOARD_WIDTH, BOARD_HEIGHT);

    //書き出し用ファイルをリセット
    json_write_reset();

    board_now = board_start;

    //block_check
    /*blockcheck_result = search_block(board_now, board_finish); //2*2ブロック探す
    for(size_t i = 0; i < blockcheck_result.size(); i++){
        cout << "\033[33m";
        cout << "p" << blockcheck_result.at(i).at(0) <<" s" << blockcheck_result.at(i).at(1) << "," <<  blockcheck_result.at(i).at(2) << ",. e" << blockcheck_result.at(i).at(3) << "," << blockcheck_result.at(i).at(4) << "が一致" << endl;
        cout << "\033[37m";
    }
*/

    vector<vector<int>> size = define_size();
    vector<vector<vector<int> > > nukigata = define_nukigata(size);

    //board_now = katanuki(1, 0, 0, 2, size, nukigata, board_start, BOARD_WIDTH, BOARD_HEIGHT);
    //[0:up 1:down 2:left 3:right]

    /*bool zero = 0;
    bool one = 0;
    bool two = 0;
    bool three = 0;*/

	//描画開始
	while (1/*System::Update()*/){
        double timer = 293000; //五分ピッタリ==293000
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - now;
        if (elapsed.count() > timer){
            //サーバー送信用プログラムを入れておきたい。
            break;
        }
	 	//gui_drawing
        /*font(U"FINISH").draw(90 + side_length * BOARD_WIDTH, 15, ColorF{ 1.0, 1.0, 1.0 });
        Board_draw(90 + side_length * BOARD_WIDTH, 70, side_length, board_finish, font);

        font(U"NOW").draw(40, 15, ColorF{ 1.0, 1.0, 1.0 });
        Board_draw(40, 70, side_length, board_now, font);*/

        /*
        for(size_t i = 0; i < blockcheck_result.size(); i++){
            Rect{ 40 + side_length * blockcheck_result.at(i).at(1), 70 + side_length * blockcheck_result.at(i).at(2), side_length*blockcheck_result.at(i).at(0), side_length*blockcheck_result.at(i).at(0) }.drawFrame(0.8, 0.8, Palette::Red);
            Rect{ 90 + side_length * BOARD_WIDTH +  side_length * blockcheck_result.at(i).at(3), 70 + side_length * blockcheck_result.at(i).at(4), side_length*blockcheck_result.at(i).at(0), side_length*blockcheck_result.at(i).at(0) }.drawFrame(0.8, 0.8, Palette::Red);
        }
        */

        check(board_now, board_finish, number);

        board_sort_search(board_now, board_finish, sort_result);
        if (sort_result.second.second >= BOARD_HEIGHT){
            cout << "finish!" << endl;
            cout << time << "手" << endl;
            break;
        }
        //cout << sort_result.second.first << ',' << sort_result.second.second << endl;
        /*Rect{40, 70, side_length * sort_result.first.first, side_length * (sort_result.first.second + 1)}.drawFrame(0.8, 0.8, Palette::Green);
        if (sort_result.second.first >= 0){
            Rect{40, 70 + side_length * (sort_result.first.second + 1), side_length * sort_result.second.first, side_length * 1}.drawFrame(0.8, 0.8, Palette::Green);
        }*/

        //Print << te1.active; // アクティブかどうか
		//Print << te1.text; // 入力されたテキスト (String)

		//SimpleGUI::TextBox(te1, Vec2{ 90 + side_length * BOARD_WIDTH, 100 + side_length * BOARD_HEIGHT});
        /*font(U"フォーマット「num,x,y」で入力してください。").draw(90 + side_length * BOARD_WIDTH, 135 + side_length * BOARD_HEIGHT, ColorF{1,1,1});
        font(U"num, x, yは変数です。").draw(90 + side_length * BOARD_WIDTH, 147 + side_length * BOARD_HEIGHT, ColorF{1,1,1});*/

        /*sort_persent = ((float)sort_num / (BOARD_HEIGHT * BOARD_WIDTH)) * 100;
        font(U"ソート一致率{}%"_fmt(sort_persent)).draw(20, Vec2{90 + side_length * BOARD_WIDTH, 170 + side_length * BOARD_HEIGHT}, ColorF{1,1,1});
        persent = ((float)number / (BOARD_HEIGHT * BOARD_WIDTH)) * 100;
        font(U"一致率{}%"_fmt(persent)).draw(20, Vec2{90 + side_length * BOARD_WIDTH, 200 + side_length * BOARD_HEIGHT}, ColorF{1,1,1});
        font(U"{}手型抜き操作済み"_fmt(time)).draw(20, Vec2{90 + side_length * BOARD_WIDTH, 230 + side_length * BOARD_HEIGHT}, ColorF{1,1,1});*/
		
        //数字キー入力で各種対応数字を強調
        /*if (KeyA.down()){
            if (zero == 0){
                zero = 1;
            }
            else if (zero == 1){
                zero = 0;
            }
        }
        if (KeyB.down()){
            if (one == 0){
                one = 1;
            }
            else if (one == 1){
                one = 0;
            }
        }
        if (KeyC.down()){
            if (two == 0){
                two = 1;
            }
            else if (two == 1){
                two = 0;
            }
        }
        if (KeyD.down()){
            if (three == 0){
                three = 1;
            }
            else if (three == 1){
                three = 0;
            }
        }*/

        /*if (KeyEscape.down()){
            break;
        }*/

        /*if (KeyTab.pressed()){
            check_result_draw(board_now, board_finish, side_length);    //最終ボードと一致しているすべてのピースを表示する
        }*/

        //表示関数
        /*number_draw_now(0, zero, board_now, side_length);
        number_draw_now(1, one, board_now, side_length);
        number_draw_now(2, two, board_now, side_length);
        number_draw_now(3, three, board_now, side_length);
        number_draw_finish(0, zero, board_finish, side_length);
        number_draw_finish(1, one, board_finish, side_length);
        number_draw_finish(2, two, board_finish, side_length);
        number_draw_finish(3, three, board_finish, side_length);*/
        
        vector<int> num(4);

        //矢印キー入力で型抜き方向を決定
        /*if(KeyRight.down()){
            num4 = 3;
        }
        else if (KeyLeft.down()){
            num4 = 2;
        }
        else if (KeyUp.down()){
            num4 = 0;
        }
        else if (KeyDown.down()){
            num4 = 1;
        }*/

        /*if(KeyEnter.down()){
            time++;
            entered_text = te1.text;

            te1.clear();
            //Print << U"Entered text: " << entered_text;

            Array<String> tokens = entered_text.split(U',');
            int num1 = Parse<int>(tokens[0]);
            int num2 = Parse<int>(tokens[1]);
            int num3 = Parse<int>(tokens[2]);

            cout << "num1: " << num1 << endl;
            cout << "num2: " << num2 << endl;
            cout << "num3: " << num3 << endl;
            cout << "num4: " << num4 << endl;

            board_now = katanuki(num1, num2, num3, num4, size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
            check(board_now, board_finish, num);
        }*/
        if (sort_result.second.first < BOARD_WIDTH - 1){
            if (sort_result.second.second % 2 == 0){
                if (sort_result.second.second != BOARD_HEIGHT + 1) sort_result.second.second++;
                else sort_result.second.first++;
            }
            else {
                sort_result.second.first++;
                sort_result.second.second--;
            }
        }
        else {
            if (sort_result.second.second % 2 == 0){
                sort_result.second.second++;
            }
            else {
                sort_result.second.first = 0;
                sort_result.second.second++;
            }
        }
        operation(time, board_now, board_finish, sort_result.second, size, nukigata, num);
        if (sort_result.second.first == 0){
            if (sort_result.second.second % 2 == 1){
                sort_result.second.second--;
            }
            else if (sort_result.second.first == 0 && sort_result.second.second == 0){
                
            }
            else {
                sort_result.second.first = BOARD_WIDTH - 1;
                sort_result.second.second--;
            }
        }
        else {
            if (sort_result.second.second % 2 == 1){
                sort_result.second.second--;
            }
            else {
                if (sort_result.second.second == BOARD_HEIGHT - 1){

                }
                else sort_result.second.second++;
                sort_result.second.first--;
            }
        }
        if (time < 0){
            cout << "error" << endl;
            break;
        }
	}
}

//盤面の表示
/*void Board_draw(int position_x, int position_y, int side_length, vector<int> &board_now, const Font& font){
    for(int x = 0; x<BOARD_HEIGHT; x++){
        if (x == 0){
            for (int j = 0; j < BOARD_WIDTH; j++){
                if (j == 0){
                    font(j % 10).draw(position_x + 3 + side_length * j , position_y - 1 - side_length * 2, ColorF(255, 255, 255));
                }
                else if (j % 10 == 0){
                    font(j).draw(position_x + 3 + side_length * j , position_y - 1 - side_length * 3, ColorF(255, 255, 255));
                }
                else {
                    font(j % 10).draw(position_x + 3 + side_length * j , position_y - 1 - side_length * 2, ColorF(255, 255, 255));
                }
            }
            for (int j = 0; j < BOARD_HEIGHT; j++){
                if (j == 0){
                    font(j % 10).draw(position_x + 3 - side_length * 2 , position_y - 1 + side_length * j, ColorF(255, 255, 255));
                }
                else if (j % 100 == 0){
                    font(j).draw(position_x + 3 - side_length * 3 , position_y - 1 + side_length * j, ColorF(255, 255, 255));
                }
                else if (j % 10 == 0){
                    font(j).draw(position_x + 3 - side_length * 2.5 , position_y - 1 + side_length * j, ColorF(255, 255, 255));
                }
                else {
                    font(j % 10).draw(position_x + 3 - side_length * 2 , position_y - 1 + side_length * j, ColorF(255, 255, 255));
                }
            }
        }
            for(int y =0; y<BOARD_WIDTH; y++){
                switch(board_now.at(x * BOARD_WIDTH + y)){
                    case 0:
                        Rect{position_x + side_length * y, position_y + side_length * x, side_length, side_length }.draw(Palette::White);
                        font(U"0").draw(position_x + 3 + side_length * y, position_y - 1 + side_length * x, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 1:
                        Rect{position_x + side_length * y, position_y + side_length * x, side_length, side_length }.draw(Palette::Pink);
                        font(U"1").draw(position_x + 3 + side_length * y, position_y - 1 + side_length * x, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 2:
                        Rect{position_x + side_length * y, position_y + side_length * x, side_length, side_length }.draw(Palette::Khaki);
                        font(U"2").draw(position_x + 3 + side_length * y, position_y - 1 + side_length * x, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 3:
                        Rect{position_x + side_length * y, position_y + side_length * x, side_length, side_length }.draw(Palette::Lightblue);
                        font(U"3").draw(position_x + 3 + side_length * y, position_y - 1 + side_length * x, ColorF{ 0.0, 0.0, 0.0 });
                        break;
                }
            }
        }
}*/

//ブロックに分けて探す
/*vector<vector<int>> search_block(vector<int>& board_now, vector<int>& board_finish){
    vector<int> blockcheck_now(BOARD_WIDTH * BOARD_HEIGHT, 0); 
    vector<int> blockcheck_end(BOARD_WIDTH * BOARD_HEIGHT, 0);
    vector<vector<int>> blockcheck_result;
    int block_type;

    //重複チェック用配列の初期化
    for(int i = 0; i<BOARD_HEIGHT; i++){
        for(int j = 0; j<BOARD_WIDTH; j++){
            blockcheck_now.at(j+ BOARD_WIDTH* i) = 0;
            //blockcheck_now.at(i).at(j) = 0;
        }
    }
    block_type = 2; // 2*2で探す。

    for(int i = 0; i < BOARD_WIDTH - block_type + 1; i++){
        for(int j = 0; j < BOARD_HEIGHT - block_type + 1; j++){
            for(int k = 0; k < BOARD_WIDTH - block_type + 1; k++){
                for(int l = 0; l < BOARD_HEIGHT - block_type + 1; l++){
                    //重複チェック 始点(i,j) 終点(k,l)
                    comparison(block_type, i, j, k, l, board_now, board_finish, blockcheck_now, blockcheck_end, blockcheck_result);
                }
            }
        }
    }

    return blockcheck_result;
}*/

//重複チェック
/*void comparison(int block_type, int now_x, int now_y, int end_x, int end_y, vector<int>& board_now, vector<int>& board_finish, vector<int>& blockcheck_now, vector<int>& blockcheck_end, vector<vector<int>>& blockcheck_result){
    int count = 0;
    int use = 1; //干渉していたら不採用、最初は採用にする。

    for(int i=0; i<block_type; i++){
        for(int j=0; j<block_type; j++){
            if(blockcheck_now.at(now_x+j + BOARD_WIDTH*(now_y+i)) == 1){
                use = 0; //はじめ　他のブロックに干渉していたらふさいよう
                break;
            }
            if(blockcheck_end.at(end_x+j + BOARD_WIDTH*(end_y+i)) == 1){
                use = 0; //おわり　他のブロックに干渉していたらふさいよう
                break;
            }
            if(board_now.at(now_x+j + BOARD_WIDTH*(now_y+i)) == board_finish.at(end_x+j + BOARD_WIDTH*(end_y+i))){
                count++;
            }
        }
        if(use == 0) break;
    }

    if(use == 1){
        if(count == block_type * block_type){
            //ブロックを作った場所を埋めておく
            for(int i=0; i<block_type; i++){
                for(int j=0; j<block_type; j++){
                    blockcheck_now.at(now_x+j + BOARD_WIDTH*(now_y+i)) = 1;
                    blockcheck_end.at(end_x+j + BOARD_WIDTH*(end_y+i)) = 1;
                }
            }
            
            blockcheck_result.push_back({block_type, now_x, now_y, end_x, end_y});


        }
    }
    
    
}*/

void board_sort_search(vector<int>& board_now, vector<int>&board_finish, pair<pair<int, int>, pair<int, int>> &sort_result){
    //sort_result.firstでBORAD_WIDTH×(n*mますまで一致している場合のn-1行目まで)の長方形分の正誤判定を出力する
    //sort/result.secondでn行目のmますまでの一列の正誤判定を出力する
    if (sort_result.second.first == -1){
        sort_result.second.first = 0;
    }

    bool a = 0;
    //2マスずつ見ていく
    for (int i = sort_result.second.second; i < BOARD_HEIGHT; i += 2){
        for (int j = 0; j < BOARD_WIDTH; j++){
            if (board_now.at(i * BOARD_WIDTH + j) == board_finish.at(i * BOARD_WIDTH + j)){
                sort_result.second = {j, i};
                if (board_now.at((i + 1) * BOARD_WIDTH + j) == board_finish.at((i + 1) * BOARD_WIDTH + j)){
                    sort_result.second = {j, i + 1};
                }
                else {
                    return;
                }
            }
            else {
                if (i == 0 && j == 0){
                    sort_result.second.first = -1;
                }
                a = 1;
                break;
            }
        }
        if (a == 1){
            break;
        }
    }
    if (a == 0){
        sort_result.second = {BOARD_WIDTH, BOARD_HEIGHT};
        sort_result.first = {0, 0};
    }
    else {
        if (sort_result.second.second <= 0){
            sort_result.first = {0, 0};
        }
        else {
            sort_result.first = {BOARD_WIDTH, sort_result.second.second - 1};
        }
    }
}

/*void number_draw_now (int number, bool num, vector<int> &board_now, int side_length){
    if (num == 0){

    }
    else if (num == 1){
        for (int i = 0; i < BOARD_HEIGHT; i++){
            for (int j = 0; j < BOARD_WIDTH; j++){
                if (board_now.at(i * BOARD_WIDTH + j) == number){
                    if (number == 0){
                        Rect{40 + side_length * j, 135 + side_length * BOARD_HEIGHT + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Black);
                    }
                    else if (number == 1){
                        Rect{40 + side_length * j, 135 + side_length * BOARD_HEIGHT + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Red);
                    }
                    else if (number == 2){
                        Rect{40 + side_length * j, 135 + side_length * BOARD_HEIGHT + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Yellow);
                    }
                    else if (number == 3){
                        Rect{40 + side_length * j, 135 + side_length * BOARD_HEIGHT + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Blue);
                    }
                }
            }
        }
    }
}*/

/*void number_draw_finish (int number, bool num, vector<int> &board_finish, int side_length){
    if (num == 0){

    }
    else if (num == 1){
        for (int i = 0; i < BOARD_HEIGHT; i++){
            for (int j = 0; j < BOARD_WIDTH; j++){
                if (board_finish.at(i * BOARD_WIDTH + j) == number){
                    if (number == 0){
                        Rect{40 + side_length * j, 70 + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Black);
                    }
                    else if (number == 1){
                        Rect{40 + side_length * j, 70 + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Red);
                    }
                    else if (number == 2){
                        Rect{40 + side_length * j, 70 + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Yellow);
                    }
                    else if (number == 3){
                        Rect{40 + side_length * j, 70  + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Blue);
                    }
                }
            }
        }
    }
}*/

void check(vector<int> &board_now, vector<int> &board_finish, int &num){
    num = 0;
    for (int i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; i++){
        if (board_now.at(i) == board_finish.at(i)){
            num++;
        }
    }
}

/*void check_result_draw (vector<int> &board_now, vector<int> &board_finish, int side_length){
    for (int i = 0; i < BOARD_HEIGHT; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            if (board_now.at(i * BOARD_WIDTH + j) == board_finish.at(i * BOARD_WIDTH + j)){
                Rect{40 + side_length * j, 135 + side_length * BOARD_HEIGHT + side_length * i, side_length, side_length}.drawFrame(0.8, 0.8, Palette::Purple);
            }
        }
    }
}*/