#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include "setting.hpp"

pair<int, int> operation_search(vector<int> &board_now, vector<int> &board_finish, pair<int, int> &address){
    pair<int, int> correct_address;
    //同行に目標ピースが存在するかを探す
    for (int i = address.first; i < BOARD_WIDTH; i++){
        if (board_finish.at(address.first + address.second * BOARD_WIDTH) == board_now.at(i + address.second * BOARD_WIDTH)){
            correct_address = {i, address.second};
            return (correct_address);
        }
    }
    //同列に目標ピースが存在するかを探す
    for (int i = address.second; i < BOARD_HEIGHT; i++){
        if (board_finish.at(address.first + address.second * BOARD_WIDTH) == board_now.at(address.first + i * BOARD_WIDTH)){
            correct_address = {address.first, i};
            return (correct_address);
        }
    }
    //左上から順に探す。
    for (int i = address.second + 1; i < BOARD_HEIGHT; i++){
        for (int j = 0; j < BOARD_WIDTH; j++){
            //同列はすでにサーチ済みなので処理を飛ばす
            if (j == address.first){
                continue;
            }
            if (board_finish.at(address.first + address.second * BOARD_WIDTH) == board_now.at(j + i * BOARD_WIDTH)){
                correct_address = {j, i};
                return (correct_address);
            }
        }
    }
    //もし正しいブロックがなかったら-1を返す。→エラー表示
    correct_address = {-1, -1};
    return (correct_address);
}

void operation_move_line(int &time, vector<int> &board_now, vector<int> baord_finish, pair<int, int> &address, pair<int, int> correct_piece, vector<vector<int>> &size, vector<vector<vector<int>>> nukigata){
    int n;
    vector<int> num = {0, 0, 0, 2};

    katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_HEIGHT, BOARD_WIDTH);
}

void operation_move_line2(int &time, vector<int> &board_now, vector<int> baord_finish, pair<int, int> &address, pair<int, int> correct_piece, vector<vector<int>> &size, vector<vector<vector<int>>> nukigata){
    int n;
    vector<int> num = {0, 0, 0, 3};

    katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_HEIGHT, BOARD_WIDTH);
}

void operation_move_column(int &time, vector<int> &board_now, vector<int> baord_finish, pair<int, int> &address, pair<int, int> correct_piece, vector<vector<int>> &size, vector<vector<vector<int>>> nukigata){
    int n;
    vector<int> num = {0, 0, 0, 0};

    katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_HEIGHT, BOARD_WIDTH);
}

void operation(int &time, vector<int> &board_now, vector<int> &board_finish, pair<int, int> &address, vector<vector<int>> &size, vector<vector<vector<int>>> &nukigata){
    pair<int, int> correct_piece;
    while(1){
        correct_piece = operation_search(board_now, board_finish, address);
        if (correct_piece.first == -1 && correct_piece.second == -1){
            time = -1;
            break;
        }
        if (correct_piece == address){
            break;
        }
        else if (correct_piece.first == address.first){
            operation_move_column(time, board_now, board_finish, address, correct_piece, size, nukigata);
        }
        else if (correct_piece.second == address.second){
            operation_move_line(time, board_now, board_finish, address, correct_piece, size, nukigata);
        }
        else {
            if (address.first > correct_piece.first){
                pair<int, int> addresses = {address.first, correct_piece.second};
                operation_move_line2(time, board_now, board_finish, addresses, correct_piece, size, nukigata);
            }
            else {
                pair<int, int> addresses = {address.first, correct_piece.second};
                operation_move_line(time, board_now, board_finish, addresses, correct_piece, size, nukigata);
            }
        }
        time++;
    }
}