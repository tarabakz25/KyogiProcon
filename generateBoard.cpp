#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "json.hpp"

// ボードのサイズを決定し、そのサイズのボードを生成する関数
std::vector<std::string> generate_board(int size) {
    std::random_device rd;
    std::mt19937 g(rd());
    
    int num_elements = size * size;
    std::vector<int> elements(num_elements);
    int value = 0;
    for (int i = 0; i < num_elements; i++) {
        elements[i] = value;
        value = (value + 1) % 4;  // 0から3の値を循環
    }
    
    // ボードをシャッフル
    std::shuffle(elements.begin(), elements.end(), g);
    
    // ボードデータを文字列の形式で格納
    std::vector<std::string> board(size);
    for (int i = 0; i < size; i++) {
        std::string row;
        for (int j = 0; j < size; j++) {
            row += std::to_string(elements[i * size + j]);
        }
        board[i] = row;
    }
    
    return board;
}

int main() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> size_dist(16, 128);
    int size = size_dist(g);  // 両方のボードのサイズを一致させる

    auto start_board = generate_board(size);
    auto goal_board = generate_board(size);

    nlohmann::json json_output;
    json_output["board"]["width"] = size;
    json_output["board"]["height"] = size;
    json_output["board"]["start"] = start_board;
    json_output["board"]["goal"] = goal_board;

    std::cout << json_output.dump(4) << std::endl;

    return 0;
}