#include <Siv3D.hpp> // Siv3D v0.6.15
#include <string>
#include <stdlib.h>

#include <string> //read&write
#include <fstream>
#include <sstream>

#include <vector>
#include "json.hpp"
#include "board_setting.cpp"

#include "setting.hpp"
//using namespace nlohman;
using namespace std;
using json = nlohmann::json;

int BOARD_WIDTH;
int BOARD_HEIGHT;

//プロトタイプ宣言
void Board_draw(int position_x, int position_y, int side_length, vector<int> &board_now, const Font& font);
void comparison(int block_type, int now_x, int now_y, int end_x, int end_y, vector<int>& board_now, vector<int>& board_finish, vector<int>& blockcheck_now, vector<int>& blockcheck_end, vector<vector<int>>& blockcheck_result);
vector<vector<int>> search_block(vector<int>& board_now, vector<int>& board_finish);


vector<vector<int>> define_size();
vector<vector<vector<int> > > define_nukigata(vector<vector<int>> size);
vector<int> katanuki(int piece_num, int x_min, int y_min, int direction, vector<vector<int>>& size, vector<vector<vector<int>>>& nukigata, vector<int> board, int BOARD_WIDTH, int BOARD_HEIGHT);

//jsonファイル読み込み系
// 文字列からベクトルに変換する関数(一列分を返す)
vector<int> stringToVector(const string &str)
{
    vector<int> row;
    for (char c : str)
    {
        row.push_back(c - '0'); // 文字を整数に変換
    }
    return row;
}

// JSONからボードを読み込む関数
int loadBoard(const json &jobj, vector<int> &startBoard, vector<int> &goalBoard)
{
    for (const auto &line : jobj["board"]["start"])
    {
        vector<int> row = stringToVector(line.get<string>()); //一列分をrowに格納
        startBoard.insert(startBoard.end(), row.begin(), row.end()); //.insert()で一次元に変換。
    }

    for (const auto &line : jobj["board"]["goal"])
    {
        vector<int> row = stringToVector(line.get<string>());
        goalBoard.insert(goalBoard.end(), row.begin(), row.end());
    }
    return 0;
}



void Main()
{
	//gui setting
	//Scene::SetBackground(ColorF{ 0.0, 0.0, 0.0 });
	const int side_length = 14;
    const Font font{ FontMethod::MSDF, 12, Typeface::Bold };
    
    // ウィンドウを自由にサイズ変更可能に設定
    Window::SetStyle(WindowStyle::Sizable);
    // ウィンドウサイズを 800x600 に設定
    Window::Resize(800, 800);

    TextEditState te1{ U"0,0,0" };// デフォルトのテキストを設定する
    String entered_text;

    vector<int> board_start (BOARD_WIDTH * BOARD_HEIGHT, 0);
	vector<int> board_now (BOARD_WIDTH * BOARD_HEIGHT, 0);
    vector<int> board_finish (BOARD_WIDTH * BOARD_HEIGHT, 0);
    vector<vector<int>> blockcheck_result;

	//board_setting(random)
    /* 
	for(int i = 0; i < BOARD_HEIGHT; i++){
		for(int j = 0; j < BOARD_WIDTH; j++){
			board_start.at(BOARD_WIDTH * i + j) = rand() % 4;
		}
	}
    for(int i = 0; i < BOARD_HEIGHT; i++){
		for(int j = 0; j < BOARD_WIDTH; j++){
			board_finish.at(BOARD_WIDTH * i + j) = rand() % 4;
		}
	}
     */
    std::filesystem::current_path("../"); //カレントディレクトリをソースファイルに変更。
    cout << "Current path: " << std::filesystem::current_path().c_str() << endl; //カレントディレクトリを表示;

    //jsonファイル存在するか確認。
    if (!std::filesystem::exists("./src/sample1.json")) {
        std::cerr << "File does not exist at path: ./src/sample1.json" << std::endl;
        return;
    }

    // JSONファイルの読み込み
    std::ifstream ifs("./src/sample1.json");

    //ファイル開け成功したか
    if (!ifs.is_open()) {
        std::cerr << "Failed to open the file!!!" << std::endl;
        return;
    } else {
        std::cout << "File opened successfully!" << std::endl;
    }

    // ファイル内容を読み込み成功したか
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    if (str.empty()) {
        std::cerr << "File is empty: ./src/sample1.json" << std::endl;
        return;
    }
    //std::cout << "File content: " << str << std::endl; //中身表示

    json jobj = json::parse(str);
    std::cout << "JSON parsed successfully!" << std::endl;

    //初期盤面、終了盤面取得
    vector<int> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard); 
    board_start = startBoard;
    board_finish = goalBoard;

    //幅と高さ取得
    BOARD_WIDTH = jobj["board"]["width"].get<int>();
    BOARD_HEIGHT = jobj["board"]["height"].get<int>();
    cout << "WIDTH"<<BOARD_WIDTH << " HWIGHT"<<BOARD_HEIGHT << endl;


    board_now = board_start;

    //block_check
    blockcheck_result = search_block(board_now, board_finish); //2*2ブロック探す
    for(size_t i = 0; i < blockcheck_result.size(); i++){
        cout << "\033[33m";
        cout << "p" << blockcheck_result.at(i).at(0) <<" s" << blockcheck_result.at(i).at(1) << "," <<  blockcheck_result.at(i).at(2) << ",. e" << blockcheck_result.at(i).at(3) << "," << blockcheck_result.at(i).at(4) << "が一致" << endl;
        cout << "\033[37m";
    }

    vector<vector<int>> size = define_size();
    vector<vector<vector<int> > > nukigata = define_nukigata(size);

    //board_now = katanuki(1, 0, 0, 2, size, nukigata, board_start, BOARD_WIDTH, BOARD_HEIGHT);
    //[0:up 1:down 2:left 3:right]



	//描画開始
	while (System::Update()){
	 	//gui_drawing
        font(U"START").draw(40, 15, ColorF{ 1.0, 1.0, 1.0 });
        Board_draw(40, 70, side_length, board_start, font);

        font(U"NOW").draw(40, 80 + side_length * BOARD_HEIGHT, ColorF{ 1.0, 1.0, 1.0 });
        Board_draw(40, 135 + side_length * BOARD_HEIGHT, side_length, board_now, font);

        font(U"FINISH").draw(90 + side_length * BOARD_WIDTH, 15, ColorF{ 1.0, 1.0, 1.0 });
        Board_draw(90 + 14 * BOARD_WIDTH, 70, side_length, board_finish, font);

        for(size_t i = 0; i < blockcheck_result.size(); i++){
             Rect{ 40 + side_length * blockcheck_result.at(i).at(1), 70 + side_length * blockcheck_result.at(i).at(2), side_length*blockcheck_result.at(i).at(0), side_length*blockcheck_result.at(i).at(0) }.drawFrame(0.8, 0.8, Palette::Red);
             Rect{ 90 + side_length * BOARD_WIDTH +  side_length * blockcheck_result.at(i).at(3), 70 + side_length * blockcheck_result.at(i).at(4), side_length*blockcheck_result.at(i).at(0), side_length*blockcheck_result.at(i).at(0) }.drawFrame(0.8, 0.8, Palette::Red);
        
        }

        //Print << te1.active; // アクティブかどうか
		//Print << te1.text; // 入力されたテキスト (String)

		SimpleGUI::TextBox(te1, Vec2{ 90 + side_length * BOARD_WIDTH, 100 + side_length * BOARD_HEIGHT});

        int num4;

        //矢印キー入力で型抜き方向を決定
        if(KeyRight.down()){
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
        }

        if(KeyEnter.down()){
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

        }
	}
}

//盤面の表示
void Board_draw(int position_x, int position_y, int side_length, vector<int> &board_now, const Font& font){
    for(int x = 0; x<BOARD_HEIGHT; x++){
        if (x == 0){
            for (int j = 0; j < BOARD_WIDTH; j++){
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
                if (j == 0){
                    font(j % 10).draw(position_x + 3 + side_length * j , position_y - 1 - side_length * 2, ColorF(255, 255, 255));
                }
                else if (j % 10 == 0){
                    font(j).draw(position_x + 3 + side_length * j , position_y - 1 - side_length * 3, ColorF(255, 255, 255));
                    font(0).draw(position_x + 3 + side_length * j , position_y - 1 - side_length * 2, ColorF(255, 255, 255));
                }
                else {
                    font(j % 10).draw(position_x + 3 + side_length * j , position_y - 1 - side_length * 2, ColorF(255, 255, 255));
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
}

//ブロックに分けて探す
vector<vector<int>> search_block(vector<int>& board_now, vector<int>& board_finish){
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
}


//重複チェック
void comparison(int block_type, int now_x, int now_y, int end_x, int end_y, vector<int>& board_now, vector<int>& board_finish, vector<int>& blockcheck_now, vector<int>& blockcheck_end, vector<vector<int>>& blockcheck_result){
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
    
    
}