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

void json_read(vector<int> &board_start, vector<int> &board_finish, int &BOARD_WIDTH, int &BOARD_HEIGHT){
    //今のところサンプルを読み込みますが、curlで読み込んだファイル(problem.json)も使えます！

    //jsonファイル存在するか確認。
    if (!filesystem::exists("./src/problem.json")) {
        std::cerr << "File does not exist at path: ./src/sample1.json" << std::endl;
        return;
    }

    // JSONファイルの読み込み
    std::ifstream ifs("./src/problem.json");

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
        std::cerr << "File is empty: ./src/problem.json" << std::endl;
        return;
    }
    //std::cout << "File content: " << str << std::endl; //中身表示

    ordered_json jobj = json::parse(str);
    std::cout << "(状態)問題をjson化できました。" << std::endl;

    //初期盤面、終了盤面取得
    vector<int> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard); 

    //各配列・変数に代入。
    board_start = startBoard;
    board_finish = goalBoard;

    //幅と高さ取得・代入。
    BOARD_WIDTH = jobj["board"]["width"].get<int>();
    BOARD_HEIGHT = jobj["board"]["height"].get<int>();
    cout << "(情報)" << "WIDTH"<<BOARD_WIDTH << " HWIGHT"<<BOARD_HEIGHT << endl;

    ofstream rec_file("./src/board_record.json");
    if (rec_file.is_open()) {
        rec_file << jobj.dump(4); //ファイルを入れる。
        rec_file.close();
        std::cout << "問題を読み込みました" << std::endl;
    }else{
        cout << "問題ファイルが開けませんでした。";
    }
}