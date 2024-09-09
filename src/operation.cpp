#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <vector>

#include "setting.hpp"

pair<int, int> operation_search(vector<int> &board_now, vector<int> &board_finish, pair<int, int> &address){
    pair<int, int> correct_address;
    for (int i = address.second; i < BOARD_WIDTH; i++){
        if (board_finish.at(address.second + address.first * BOARD_WIDTH) == board_now.at(i + address.first * BOARD_WIDTH)){
            correct_address = {i, address.first};
        }
    }
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