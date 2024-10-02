#include <vector>
#include <iostream>
#include "setting.hpp"

#include <string> //read&write
#include <fstream>
#include <sstream>

#include "json.hpp"


using namespace std;
using json = nlohmann::json;

vector<vector<int>> define_size(){
    //返却値宣言
    vector<vector<int>> size(25, vector<int> (2));

    int b = 0;  //配列の要素指定変数
    size.at(b) = {1, 1};

    b++;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 3; j++){
            size.at(b) = {2 << i, 2 << i}; //ビット操作で256まで計算
            b++;    //配列の要素を更新
        }
    }

    return (size);
}

vector<vector<vector<int>>>  define_nukigata(vector<vector<int>> size){
    //返却値宣言
    vector<vector<vector<int>>> nukigata(25, vector<vector<int>> (256, vector<int> (256, 0)));

    //一番目の抜き型を初期化
    nukigata.at(0).at(0).at(0) = 1;

    //二~二十五番目の抜き型を初期化
    for (int i = 0; i < 8; i++){
        for (int j = 1; j <= 3; j++){
            if (j == 1){
                for (int k = 0; k < 2 << i; k++){
                    for (int l = 0; l < 2 << i; l++){
                        nukigata.at(i * 3 + j).at(k).at(l) = 1;
                    }
                }
            }
            else if (j == 3){
                for (int k = 0; k < 2 << i; k++){
                    for (int l = 0; l < 2 << i; l += 2){
                        nukigata.at(i * 3 + j).at(k).at(l) = 1;
                    }
                }
            }
            else if (j == 2){
                for (int k = 0; k < 2 << i; k += 2){
                    for (int l = 0; l < 2 << i; l++){
                        nukigata.at(i * 3 + j).at(k).at(l) = 1;
                    }
                }
            }
        }
    }

    return (nukigata);
}

vector<vector<vector<int>>>  addnukigata(vector<vector<vector<int>>> nukigata){

    string read_jsonpath = "./src/problem.json";

    // JSONファイルの読み込み
    std::ifstream ifs(read_jsonpath);

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    json data;
    try {
        data = json::parse(str);
    } catch (json::parse_error& e) {
        cout << "一般抜き型抜き型データを取得できませんでした。 " << e.what() << endl;
        return nukigata; // パースエラー時はそのままnukigataを返す
    }

    const auto& patterns = data["general"]["patterns"];


    for (const auto &pattern : patterns) { //jsonの要素回くり返す。
        int p = pattern.value("p", -1);  // パターンの番号
        int width = pattern.value("width", 0);  // 幅
        int height = pattern.value("height", 0);  // 高さ

        if (p < 0) {
            cerr << "Invalid pattern number: " << p << endl;
            continue; // 無効なパターン番号はスキップ
        }

        // cellsフィールドから抜き型を作成
        vector<vector<int>> new_pattern(height, vector<int>(width, 0));
        const auto& cells = pattern["cells"];
        for (int i = 0; i < height; i++) {
            const string &line = cells[i];
            for (int j = 0; j < width; j++) {
                new_pattern[i][j] = (line[j] == '1') ? 1 : 0;
            }
        }

        // 作成した抜き型をnukigataに追加
        if (p >= nukigata.size()) {
            nukigata.resize(p + 1, vector<vector<int>>(256, vector<int>(256, 0))); // 必要に応じてサイズを拡張
        }
        nukigata[p] = new_pattern;  // 既存のnukigataリストに追加
    }

    return nukigata;
}

vector<int> katanuki(int piece_num, int x_min, int y_min, int direction, vector<vector<int>>& size, vector<vector<vector<int>>>& nukigata, vector<int> board, int BOARD_WIDTH, int BOARD_HEIGHT){
    
    //cout << MOVE << "手目" << endl;

    /* for(int i=0; i<size.at(piece_num).at(0); i++){
        for(int j=0; j<size.at(piece_num).at(1); j++){
            cout << "[" << nukigata.at(piece_num).at(i).at(j) << "]";
        }
        cout << endl;
    }  */

    //cout  << "抜き型No." <<  piece_num << "を適用" << endl;
    int x_max = x_min + size.at(piece_num).at(0) - 1;
    int y_max = y_min + size.at(piece_num).at(1) - 1;
    
    
    //範囲表示。
    //cout << "型抜き範囲x:" << x_min << " ~ " << x_max <<endl;
    //cout << "型抜き範囲y:" << y_min << " ~ " << y_max << endl;
    //cout << "抜き出したもの" << endl;

    //方向表示
    //cout << "型抜き方向" << direction << endl;

    //型抜き
    vector<int> kata_1d(256*256, 0);
    int kata_1d_in = 0;
    for(int i=0; i < size.at(piece_num).at(0); i++){
        for(int j=0; j < size.at(piece_num).at(1); j++){ 
            if(y_min + i >= BOARD_HEIGHT || x_min + j >= BOARD_WIDTH || y_min + i < 0 || x_min + j < 0){             //board_sizeまで到達（board_size番目に到達したら考えない。）

            }else{
                if(nukigata.at(piece_num).at(i).at(j) == 1){
                    kata_1d.at(kata_1d_in) = board.at((y_min+i) * BOARD_WIDTH + (x_min + j)); //一次元配列に格納
                    
                    board.at((y_min+i) * BOARD_WIDTH + (x_min + j)) = 9; //抜かれた部分は9にする
    
                    //cout << "[" << kata_1d.at(kata_1d_in)  << "]" ; 
                    kata_1d_in++;
                }
            }
        }
       // cout << endl;
    }

   // cout << endl << "方向:";
    switch (direction)
    {
    case 0: //上方向
        //cout << "上方向" << endl;
        for(int i=0; i<BOARD_HEIGHT; i++){
            for(int j=0; j<BOARD_WIDTH; j++){
                if(i != 0){
                    if(board.at(i * BOARD_WIDTH + j) != 9 && board.at((i-1)* BOARD_WIDTH +j) == 9){
                        board.at((i-1)* BOARD_WIDTH + j) = board.at(i*BOARD_WIDTH + j);
                        board.at(i*BOARD_WIDTH + j) = 9;
                        i -=1;
                        j -=1;
                    }
                }
                
            }
        } 
        break;

    case 1: //下方向
        //cout << "下方向" << endl;
        for(int i=BOARD_HEIGHT-1; i>-1; i--){
            for(int j=0; j<BOARD_WIDTH; j++){
                if(i != BOARD_HEIGHT-1){
                    if(board.at(i* BOARD_WIDTH + j) != 9 && board.at((i+1)*BOARD_WIDTH + j) == 9){
                        board.at((i+1)*BOARD_WIDTH +j) = board.at(i*BOARD_WIDTH +j);
                        board.at(i*BOARD_WIDTH +j) = 9;
                        i +=1;
                        j -=1;
                    }
                }
                
            }
        } 
        break;

    case 2: //左方向
        //cout << "左方向" << endl;
        for(int j=0; j<BOARD_WIDTH; j++){
            for(int i=0; i<BOARD_HEIGHT; i++){
                if(j != 0){
                    if(board.at(i* BOARD_WIDTH +j) != 9 && board.at(i*BOARD_WIDTH +(j-1)) == 9){
                        board.at(i*BOARD_WIDTH +(j-1)) = board.at(i*BOARD_WIDTH + j);
                        board.at(i*BOARD_WIDTH + j) = 9;
                        i -=1;
                        j -=1;   
                    }                   
                }
            }
        } 
        break;    

    case 3: //右方向
        //cout << "右方向" << endl;
        for(int j=BOARD_WIDTH-1; j>-1; j--){
            for(int i=0; i<BOARD_HEIGHT; i++){
                if(j != BOARD_WIDTH-1){
                    if(board.at(i*BOARD_WIDTH +j) != 9 && board.at(i*BOARD_WIDTH + (j+1)) == 9){
                        board.at(i*BOARD_WIDTH + (j+1)) = board.at(i*BOARD_WIDTH +j);
                        board.at(i*BOARD_WIDTH +j) = 9;
                        i -=1;
                        j +=1;   
                    }                   
                }
            }
        } 
        break;            
    default:
        break;
    }

    //空いているところ（9）に抜いた部分を入れる
    int fill_num = 0;
    for(int i=0; i<BOARD_HEIGHT; i++){
        for(int j=0; j<BOARD_WIDTH; j++){
            if(board.at(i* BOARD_WIDTH +j) == 9){
                board.at(i* BOARD_WIDTH + j) = kata_1d.at(fill_num);
                fill_num++;

            }
        }
    }

    return board;
}

// nukigataの内容を見やすく出力する関数
void printNukigata(vector<vector<vector<int>>>& nukigata) {
    for (size_t i = 25; i < nukigata.size(); i++) {
        cout << "Pattern " << i << ":\n";
        for (const auto& row : nukigata[i]) {
            for (const auto& cell : row) {
                cout << cell << " ";
            }
            cout << "\n";
        }
        cout << "------------------------\n"; // パターンの区切り
    }
}