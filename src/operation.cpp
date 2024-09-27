#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <bitset>

#include "setting.hpp"

pair<int, int> operation_search(vector<int> &board_now, vector<int> &board_finish, pair<int, int> &address){
    pair<int, int> correct_address;
    //同行に目標ピースが存在するかを探す
    for (int i = address.first; i < BOARD_WIDTH; i++){
        if (board_finish.at(address.first + address.second * BOARD_WIDTH) == board_now.at(i + address.second * BOARD_WIDTH)){
            correct_address = {i, address.second};
            //cout << "同行" << endl;
            return (correct_address);
        }
    }
    //同列に目標ピースが存在するかを探す
    for (int i = address.second; i < BOARD_HEIGHT; i++){
        if (board_finish.at(address.first + address.second * BOARD_WIDTH) == board_now.at(address.first + i * BOARD_WIDTH)){
            correct_address = {address.first, i};
            //cout << "同列" << endl;
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
                //cout << "その他" << endl;
                return (correct_address);
            }
        }
    }
    //もし正しいブロックがなかったら-1を返す。→エラー表示
    correct_address = {-1, -1};
    return (correct_address);
}

void operation_move_line(int &time, vector<int> &board_now, vector<int> &baord_finish, pair<int, int> &address, pair<int, int> &correct_piece, vector<vector<int>> &size, vector<vector<vector<int>>> &nukigata, vector<int> &num){
    if (address.first == 0){
        num = {22, correct_piece.first - 256, address.second, 2};
        board_now = katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
        return;
    }
    int n;  //目的座標と目的ピースのx座標の差を読み取る
    num = {0, address.first, address.second, 2};    //型抜き関数用(2は左抜き)

    n = correct_piece.first - address.first;    //目標座標と目的ピースの座標差を保持

    bitset<9> difference(n); //2進数に変換
    string differences = difference.to_string();    //char型に変換

    //8-iビット目の二進数が1だった場合、3(8-i)-2の抜き型=該当の大きさの正方形前選択抜き型を適応するように設定
    for (int i = 0; i < 8; i++){
        if (differences.at(i) == '1'){
            num.at(0) = (7 - i) * 3 + 1;
        }
        else {
            continue;
        }
        board_now = katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
        time++; //手数更新
        json_write(time, num.at(0), num.at(1), num.at(2), num.at(3));
        //cout << time << "手目" << endl;
    }
    if (differences.at(8) == '1'){
        num.at(0) = 0;
        board_now = katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
        time++; //手数更新
        json_write(time, num.at(0), num.at(1), num.at(2), num.at(3));
        //cout << time << "手目" << endl;
    }
}

void operation_move_line2(int &time, vector<int> &board_now, vector<int> &board_finish, pair<int, int> &address, pair<int, int> &correct_piece, vector<vector<int>> &size, vector<vector<vector<int>>> &nukigata, vector<int> &num){
    if (address.first == BOARD_WIDTH - 1){
        num = {22, correct_piece.first + 1, address.second, 3};
        board_now = katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
        return;
    }
    int n;  //目的座標と目的ピースのx座標の差を読み取る
    num = {0, 0, address.second, 3};    //型抜き関数用(3は右抜き)

    n = address.first - correct_piece.first;    //目標座標よりも左にある。

    bitset<9> difference(n); //2進数に変換
    string differences = difference.to_string();    //char型に変換

    //8-iビット目の二進数が1だった場合、3(8-i)-2の抜き型=該当の大きさの正方形前選択抜き型を適応するように設定
    for (int i = 0; i < 8; i++){
        if (differences.at(i) == '1'){
            num.at(0) = (7 - i) * 3 + 1;
        }
        else {
            continue;
        }
        num.at(1) = correct_piece.first + 1; //抜き型を現在の目標ピースの位置の右隣に適応
        board_now = katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
        time++; //手数更新
        json_write(time, num.at(0), num.at(1), num.at(2), num.at(3));
        int n = 2;
        int j = i;
        while (7 - j > 0) {
            n *= 2;
            j++;
        }
        correct_piece.first += n;
        //cout << time << "手目" << endl;
        /*for (int i = 0; i < BOARD_HEIGHT; i++){
            for (int j = 0; j < BOARD_WIDTH; j++){
                if (address.first == j && address.second == i){
                    cout << "\033[33m" << board_now.at(i * BOARD_WIDTH + j) << "\033[0m" << " ";
                }
                else    cout << board_now.at(i * BOARD_WIDTH + j) << " ";
            }
            cout << endl;
        }*/
    }
    if (differences.at(8) == '1'){
        num.at(0) = 0;
        num.at(1) = correct_piece.first + 1; //抜き型を現在の目標ピースの位置の右隣に適応
        board_now = katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
        time++; //手数更新
        json_write(time, num.at(0), num.at(1), num.at(2), num.at(3));
        //cout << time << "手目" << endl;
    }
}

void operation_move_column(int &time, vector<int> &board_now, vector<int> &board_finish, pair<int, int> &address, pair<int, int> &correct_piece, vector<vector<int>> &size, vector<vector<vector<int>>> &nukigata, vector<int> &num){
    int n;  //目的座標と目的ピースのy座標の差を読み取る
    num = {0, address.first, address.second, 0};    //型抜き関数用(0は上抜き)

    n = correct_piece.second - address.second;  //y座標の差を取得

    bitset<9> difference(n);    //二進数に変換
    string differences = difference.to_string();    //char型に変換

    //8-iビット目の二進数が1だった場合、3(8-i)-2の抜き型=該当の大きさの正方形前選択抜き型を適応するように設定
    for (int i = 0; i < 8; i++){
        if (differences.at(i) == '1'){
            num.at(0) = (7 - i) * 3 + 1;
        }
        else {
            continue;
        }
        board_now = katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
        time++; //手数更新
        json_write(time, num.at(0), num.at(1), num.at(2), num.at(3));
        //cout << time << "手目" << endl;
    }
    if (differences.at(8) == '1'){
        num.at(0) = 0;
        board_now = katanuki(num.at(0), num.at(1), num.at(2), num.at(3), size, nukigata, board_now, BOARD_WIDTH, BOARD_HEIGHT);
        time++; //手数更新
        json_write(time, num.at(0), num.at(1), num.at(2), num.at(3));
        //cout << time << "手目" << endl;
    }
}

void operation(int &time, vector<int> &board_now, vector<int> &board_finish, pair<int, int> &address, vector<vector<int>> &size, vector<vector<vector<int>>> &nukigata, vector<int> &num){
    pair<int, int> correct_piece;
    while(1){
        correct_piece = operation_search(board_now, board_finish, address);
        //cout << address.first << "," << address.second << endl;
        //cout << correct_piece.first << "," << correct_piece.second << endl; //目標ピースの座標を表示
        if (correct_piece == address){
            break;
        }
        else if (correct_piece.first == address.first){
            operation_move_column(time, board_now, board_finish, address, correct_piece, size, nukigata, num);
        }
        else if (correct_piece.second == address.second){
            operation_move_line(time, board_now, board_finish, address, correct_piece, size, nukigata, num);
        }
        else {
            if (address.first > correct_piece.first){
                pair<int, int> addresses = {address.first, correct_piece.second};
                //cout << addresses.first << "," << addresses.second << ':' << 1 << endl;
                operation_move_line2(time, board_now, board_finish, addresses, correct_piece, size, nukigata, num);
            }
            else {
                pair<int, int> addresses = {address.first, correct_piece.second};
                //cout << addresses.first << "," << addresses.second << ':' << 2 << endl;
                operation_move_line(time, board_now, board_finish, addresses, correct_piece, size, nukigata, num);
            }
        }
        /*for (int i = 0; i < BOARD_HEIGHT; i++){
            for (int j = 0; j < BOARD_WIDTH; j++){
                if (address.first == j && address.second == i){
                    cout << "\033[33m" << board_now.at(i * BOARD_WIDTH + j) << "\033[0m" << " ";
                }
                else    cout << board_now.at(i * BOARD_WIDTH + j) << " ";
            }
            cout << endl;
        }*/
    }
}