#include <vector>
#include <iostream>
#include <filesystem>

#include <string> //read&write
#include <fstream>
#include <sstream>
#include <vector>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//jsonファイル読み込み系
// 文字列からベクトルに変換する関数(一列分を返す)
map<int, int> stringToMap(const string &str)
{
    map<int, int> row;
    int i = 0;
    for (char c : str)
    {
        row[i] = (c - '0'); // 文字を整数に変換
        i++;
    }
    return row;
}

// JSONからボードを読み込む関数
int loadBoard(const json &jobj, map<int, int> &startBoard, map<int, int> &goalBoard)
{
    int i = 0;
    for (const auto &line : jobj["board"]["start"]){
        map<int, int> row = stringToMap(line.get<string>()); //一列分をrowに格納
        for (int j = 0; j < row.size(); j++){
            startBoard[i] = row[j]; //.insert()で一次元に変換。
            i++;
            j++;
        }
    }

    i = 0;
    for (const auto &line : jobj["board"]["goal"])
    {
        map<int, int> row = stringToMap(line.get<string>()); //一列分をrowに格納
        for (int j = 0; j < row.size(); j++){
            goalBoard[i] = row[j]; //.insert()で一次元に変換。
            i++;
            j++;
        }
    }
    return 0;
}

void json_path_setting(){
    filesystem::current_path("../"); //カレントディレクトリをソースファイルに変更。
    cout << "(設定)Current path: " << filesystem::current_path().c_str() << endl; //カレントディレクトリを表示;
}

void json_read(map<int, int> &board_start, map<int, int> &board_finish, int &BOARD_WIDTH, int &BOARD_HEIGHT){
    //今のところサンプルを読み込みますが、curlで読み込んだファイル(problem.json)も使えます！

    //jsonファイル存在するか確認。
    if (!filesystem::exists("/Users/itougakuto/siv3d_v0.6.15_macOS/examples/empty/src/daibutu.json")) {
        std::cerr << "File does not exist at path: ./src/sample1.json" << std::endl;
        return;
    }

    // JSONファイルの読み込み
    std::ifstream ifs("/Users/itougakuto/siv3d_v0.6.15_macOS/examples/empty/src/daibutu.json");

    //ファイル開け成功したか
    if (!ifs.is_open()) {
        std::cerr << "Failed to open the file!!!" << std::endl;
        return;
    } else {
        std::cout << "(状態)問題を開けました" << std::endl;
    }

    // ファイル内容を読み込み成功したか
    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    if (str.empty()) {
        std::cerr << "File is empty: ./src/sample1.json" << std::endl;
        return;
    }
    //std::cout << "File content: " << str << std::endl; //中身表示

    json jobj = json::parse(str);
    std::cout << "(状態)問題をjson化できました。" << std::endl;

    //初期盤面、終了盤面取得
    map<int, int> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard); 

    //各配列・変数に代入。
    board_start = startBoard;
    board_finish = goalBoard;

    //幅と高さ取得・代入。
    BOARD_WIDTH = jobj["board"]["width"].get<int>();
    BOARD_HEIGHT = jobj["board"]["height"].get<int>();
    cout << "(情報)" << "WIDTH"<<BOARD_WIDTH << " HWIGHT"<<BOARD_HEIGHT << endl;
}