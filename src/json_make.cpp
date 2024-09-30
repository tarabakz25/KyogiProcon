#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

const int width = 64;
const int height = 129;
const int total_elements = width * height;
const int num_each = total_elements / 4;

// 乱数エンジンの初期化
std::random_device rd;
std::mt19937 g(rd());

// 配列を生成し、シャッフルする関数
std::vector<std::vector<int>> generateShuffledArray() {
    // 0, 1, 2, 3を均等に含む配列を作成
    std::vector<int> elements;
    elements.reserve(total_elements);

    for (int i = 0; i < num_each; ++i) {
        elements.push_back(0);
        elements.push_back(1);
        elements.push_back(2);
        elements.push_back(3);
    }

    // 配列をシャッフル
    std::shuffle(elements.begin(), elements.end(), g);

    // 二次元配列に変換
    std::vector<std::vector<int>> array(height, std::vector<int>(width));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            array[i][j] = elements[i * width + j];
        }
    }

    return array;
}

void printAsJson(const std::vector<std::vector<int>>& array, const std::string& name) {
    std::cout << "\"" << name << "\": [\n";
    for (size_t i = 0; i < array.size(); ++i) {
        std::cout << '"';
        for (size_t j = 0; j < array[i].size(); ++j) {
            std::cout << array[i][j];
        }
        std::cout << '"';
        if (i < array.size() - 1) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "]" << std::endl;
}

int main() {
    // startとgoalの配列を生成
    std::vector<std::vector<int>> start = generateShuffledArray();
    std::vector<std::vector<int>> goal = generateShuffledArray();

    // JSON形式で出力
    std::cout << "{\n";
    std::cout << "  \"board\": {\n";
    std::cout << "    \"width\": " << width << ",\n";
    std::cout << "    \"height\": " << height << ",\n";
    printAsJson(start, "start");
    std::cout << ",\n";
    printAsJson(goal, "goal");
    std::cout << "\n  }\n";
    std::cout << "}\n";

    return 0;
}
