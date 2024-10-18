#include <vector>
#include <iostream>

#include <string> //read&write
#include <fstream>
#include <sstream>
#include <vector>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json; //順番を維持する（通常アルファベット順になるらしい）


void json_write(int time, int num1, int num2, int num3, int num4){
    ifstream oldfile("./src/export.json");
    ordered_json file;
    oldfile >> file;
    oldfile.close();

    file["n"] = time;
    file["ops"].push_back({{"p", num1}, {"x", num2}, {"y", num3}, {"s", num4}});

    ofstream write_file("./src/export.json");

    if (write_file.is_open()) {
        write_file << file.dump(4);
        write_file.close();
        std::cout << "解答JSONを更新しました。" << std::endl;
    }else{
        cout << "ファイルが開けませんでした。";
    }

}

void board_record(vector<int> board_now, int BOARD_HEIGHT, int BOARD_WIDTH){
    vector<string> board_str;
    for(int i = 0; i < BOARD_HEIGHT; i++){
        string row;
        for(int j = 0; j < BOARD_WIDTH; j++){
            row += '0' + board_now.at(BOARD_WIDTH * i + j);
        }
        cout << row << endl;
        board_str.push_back(row);
    }

    ifstream rec_file("./src/board_record.json");
    ordered_json board_rec;
    rec_file >> board_rec;

    board_rec["board"]["start"] = board_str; //jsonファイル書き換え

    ofstream write_file("./src/board_record.json");
    write_file << board_rec.dump(4);


    rec_file.close();
}


void json_write_reset(){
    ofstream write_file("./src/export.json");

    json file;
    file["n"] = 0;
    file["ops"] = json::array();

    cout << file.dump(4) << endl;

    if (write_file.is_open()) {
        write_file << file.dump(4);
        write_file.close();
        std::cout << "解答JSONを初期化しました。" << std::endl;
    }else{
        cout << "ファイルが開けませんでした。";
    }

}