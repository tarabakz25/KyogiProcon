#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include "json.hpp"
#include "setting.hh"

using namespace std;
using json = nlohmann::json;

struct Operation {
    int p;
    int s;
    int x;
    int y;
};

vector<vector<int>> loadBoard(const json& j, const string& key) {
    vector<vector<int>> board;
    for (const auto& line : j["board"][key]) {
        vector<int> row;
        for (char c : line.get<string>()) {
            row.push_back(c - '0');
        }
        board.push_back(row);
    }
    return board;
}

void applyOperation(vector<vector<int>>& board, const Operation& op) {
    int size = 1 << ((op.p - 1) / 3 + 1);
    int direction = op.s;

    if (direction == 0) { // Up
        vector<int> temp(size);
        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) {
                temp[i] = board[op.y + i][op.x + j];
            }
            for (int i = 0; i < board.size() - size; ++i) {
                board[op.y + i][op.x + j] = board[op.y + i + size][op.x + j];
            }
            for (int i = 0; i < size; ++i) {
                board[board.size() - size + i][op.x + j] = temp[i];
            }
        }
    } else if (direction == 2) { // Left
        for (int i = 0; i < size; ++i) {
            vector<int> temp(size);
            for (int j = 0; j < size; ++j) {
                temp[j] = board[op.y + i][op.x + j];
            }
            for (int j = 0; j < board[0].size() - size; ++j) {
                board[op.y + i][op.x + j] = board[op.y + i][op.x + j + size];
            }
            for (int j = 0; j < size; ++j) {
                board[op.y + i][board[0].size() - size + j] = temp[j];
            }
        }
    } else if (direction == 3) { // Right
        for (int i = 0; i < size; ++i) {
            vector<int> temp(size);
            for (int j = 0; j < size; ++j) {
                temp[j] = board[op.y + i][op.x + j];
            }
            for (int j = board[0].size() - 1; j >= size; --j) {
                board[op.y + i][j] = board[op.y + i][j - size];
            }
            for (int j = 0; j < size; ++j) {
                board[op.y + i][j] = temp[j];
            }
        }
    }
}

bool checkAnswer(const vector<vector<int>>& start, const vector<vector<int>>& goal, const vector<Operation>& operations) {
    vector<vector<int>> current = start;
    
    for (const auto& op : operations) {
        applyOperation(current, op);
    }

    return current == goal;
}

int main() {
    ifstream ifs(SORT_FILE);
    json j = json::parse(ifs);

    vector<vector<int>> start = loadBoard(j, "start");
    vector<vector<int>> goal = loadBoard(j, "goal");

    ifstream answer_file("answer.json");
    json answer = json::parse(answer_file);

    vector<Operation> operations;
    for (const auto& op : answer["ops"]) {
        operations.push_back({op["p"], op["s"], op["x"], op["y"]});
    }

    bool is_correct = checkAnswer(start, goal, operations);

    cout << (is_correct ? "The answer is correct!" : "The answer is incorrect.") << endl;

    return 0;
}
