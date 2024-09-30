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