#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <sstream>
#include "json.hpp"

using namespace std;
using ordered_json = nlohmann::ordered_json;  // ordered_jsonを使う

const int width = 128;
const int height = 128;
const int total_elements = width * height;
const int num_each = total_elements / 4;

// 乱数エンジンの初期化
std::random_device rd;
std::mt19937 g(rd());

// 配列を生成し、シャッフルする関数
std::vector<std::vector<int>> generateShuffledArray() {
    std::vector<int> elements;
    elements.reserve(total_elements);

    for (int i = 0; i < num_each; ++i) {
        elements.push_back(0);
        elements.push_back(1);
        elements.push_back(2);
        elements.push_back(3);
    }

    std::shuffle(elements.begin(), elements.end(), g);

    std::vector<std::vector<int>> array(height, std::vector<int>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            array[i][j] = elements[i * width + j];
        }
    }

    return array;
}

// 2D配列を1行ずつ文字列に変換する関数
std::vector<std::string> convertArrayToStringLines(const std::vector<std::vector<int>>& array) {
    std::vector<std::string> lines;
    for (const auto& row : array) {
        std::string line;
        for (int value : row) {
            line += std::to_string(value);  // 各値を文字列に変換し、連結
        }
        lines.push_back(line);  // 1行を文字列としてベクターに追加
    }
    return lines;
}

int main() {
    // startとgoalの配列を生成
    std::vector<std::vector<int>> start = generateShuffledArray();
    std::vector<std::vector<int>> goal = generateShuffledArray();

    ofstream write_file("/Users/itougakuto/siv3d_v0.6.15_macOS/examples/empty/src/sample1.json");
    if (!write_file.is_open()) {
        cerr << "ファイルを開けませんでした。" << endl;
        return 1;
    }

    // startとgoalの2D配列を文字列ベクターに変換
    std::vector<std::string> start_lines = convertArrayToStringLines(start);
    std::vector<std::string> goal_lines = convertArrayToStringLines(goal);

    // ordered_jsonを使って順序を保持
    ordered_json file;
    file["board"]["width"] = width;
    file["board"]["height"] = height;
    file["board"]["start"] = start_lines;
    file["board"]["goal"] = goal_lines;

    write_file << file.dump(4);  // インデントをつけてJSON書き込み
    write_file.close();

    return 0;
}
