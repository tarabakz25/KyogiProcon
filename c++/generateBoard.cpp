#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

// ボードをランダムに生成する関数
std::vector<std::string> generateRandomBoard(int width, int height) {
    std::vector<std::string> board(height);
    for (int i = 0; i < height; ++i) {
        board[i].resize(width);
        for (int j = 0; j < width; ++j) {
            board[i][j] = '0' + rand() % 4; // 0から3のランダム値
        }
    }
    return board;
}

// ボードをJSON形式で出力する関数
void printBoardJson(const std::vector<std::string>& start, const std::vector<std::string>& goal) {
    int height = start.size();
    int width = start[0].size();

    std::cout << "\"board\": {\n";
    std::cout << "  \"width\": " << width << ",\n";
    std::cout << "  \"height\": " << height << ",\n";
    std::cout << "  \"start\": [\n";
    for (int i = 0; i < height; ++i) {
        std::cout << "    \"" << start[i] << "\"" << (i < height - 1 ? "," : "") << "\n";
    }
    std::cout << "  ],\n";
    std::cout << "  \"goal\": [\n";
    for (int i = 0; i < height; ++i) {
        std::cout << "    \"" << goal[i] << "\"" << (i < height - 1 ? "," : "") << "\n";
    }
    std::cout << "  ]\n";
    std::cout << "}\n";
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // 乱数の初期化
    const int width = 16;
    const int height = 16;

    std::vector<std::string> start = generateRandomBoard(width, height);
    std::vector<std::string> goal = generateRandomBoard(width, height);

    printBoardJson(start, goal);
    return 0;
}
