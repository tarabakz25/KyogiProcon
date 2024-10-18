#include <vector>
#include <iostream>
#include "setting.hpp"
using namespace std;

vector<vector<int>> define_size(){
    //返却値宣言
    vector<vector<int>> size(25, vector<int> (2));

    int b = 0;  //配列の要素指定変数
    size.at(b) = {1, 1};

    b++;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 3; j++){
            size.at(b) = {2 << i, 2 << i}; //ビット操作で256まで計算
            b++;    //配列の要素を更新
        }
    }

    return (size);
}

vector<vector<vector<int>>>  define_nukigata(vector<vector<int>> size){
    //返却値宣言
    vector<vector<vector<int>>> nukigata(25, vector<vector<int>> (256, vector<int> (256, 0)));

    //一番目の抜き型を初期化
    nukigata.at(0).at(0).at(0) = 1;

    //二~二十五番目の抜き型を初期化
    for (int i = 0; i < 8; i++){
        for (int j = 1; j <= 3; j++){
            if (j == 1){
                for (int k = 0; k < 2 << i; k++){
                    for (int l = 0; l < 2 << i; l++){
                        nukigata.at(i * 3 + j).at(k).at(l) = 1;
                    }
                }
            }
            else if (j == 3){
                for (int k = 0; k < 2 << i; k++){
                    for (int l = 0; l < 2 << i; l += 2){
                        nukigata.at(i * 3 + j).at(k).at(l) = 1;
                    }
                }
            }
            else if (j == 2){
                for (int k = 0; k < 2 << i; k += 2){
                    for (int l = 0; l < 2 << i; l++){
                        nukigata.at(i * 3 + j).at(k).at(l) = 1;
                    }
                }
            }
        }
    }

    return (nukigata);
}

vector<int> katanuki(int piece_num, int x_min, int y_min, int direction, vector<vector<int>>& size, vector<vector<vector<int>>>& nukigata, vector<int> board, int BOARD_WIDTH, int BOARD_HEIGHT){
    vector<vector<int>> unplug(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, -1));  // 抜き出す数字
    vector<vector<int>> push(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, -1));    // 寄せる数字
    
    vector<vector<int> > board_2d(BOARD_HEIGHT, vector<int>(BOARD_WIDTH));
    int k = 0;
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board_2d[i][j] = board.at(k);
            k++;
        }
    }

    vector<vector<int>> sB = board_2d;

    int n = pow(2,(piece_num + 2)/3); //型抜き正方形の幅・高さ

    int height_diff;

    switch(direction){
        
        case 0:
            height_diff = BOARD_HEIGHT - y_min - n;
            for(int di = 0; di < n; di++){
                int curtRow = y_min + di;
                if (x_min + n >= BOARD_WIDTH) copy(sB[curtRow].begin() + x_min, sB[curtRow].end(), unplug[di].begin());
                else copy(sB[curtRow].begin() + x_min, sB[curtRow].begin() + x_min + n, unplug[di].begin());
            }
            for(int di = 0; di < height_diff; di++){
                int curtRow = y_min + di;
                if (x_min + n >= BOARD_WIDTH) copy(sB[curtRow + n].begin() + x_min, sB[curtRow + n].end(), push[di].begin());
                else copy(sB[curtRow + n].begin() + x_min, sB[curtRow + n].begin() + x_min + n, push[di].begin());
            }
            for(int di = 0; di < height_diff; di++){
                if (push[di][0] == -1) break;
                for(int dj = 0; dj < n; dj++){
                    if (push[di][dj] == -1) break;
                    else sB[y_min + di][x_min + dj] = push[di][dj];
                }
            }
            for(int di = 0; di < n; di++){
                if (unplug[di][0] == -1) break;
                for(int dj = 0; dj < n; dj++){
                    if (unplug[di][dj] == -1) break;
                    else sB[y_min + height_diff + di][x_min + dj] = unplug[di][dj];
                }
            }
            break;

        case 1:
            height_diff = y_min;
            for(int di = 0; di < n; di++){
                int curtRow = y_min + di;
                if (curtRow < 0 || curtRow >= BOARD_HEIGHT) continue; // 範囲外チェック
                if (x_min + n >= BOARD_WIDTH) copy(sB[curtRow].begin() + x_min, sB[curtRow].end(), unplug[di].begin());
                else copy(sB[curtRow].begin() + x_min, sB[curtRow].begin() + x_min + n, unplug[di].begin());
            }
            for(int di = 0; di < height_diff; di++){
                int curtRow = y_min -height_diff + di;
                if (curtRow < 0 || curtRow >= BOARD_HEIGHT) continue; // 範囲外チェック
                if (x_min + n >= BOARD_WIDTH) copy(sB[curtRow].begin() + x_min, sB[curtRow].end(), push[di].begin());
                else copy(sB[curtRow].begin() + x_min, sB[curtRow].begin() + x_min + n, push[di].begin());
            }
            for(int di = 0; di < height_diff; di++){
                if (push[di][0] == -1) break;
                for(int dj = 0; dj < n; dj++){
                    if (push[di][dj] == -1) break;
                    else sB[y_min - height_diff + di][x_min + dj] = push[di][dj];
                }
            }
            for(int di = 0; di < n; di++){
                if (unplug[di][0] == -1) break;
                int targetRow = y_min - n + di;
                if (targetRow < 0 || targetRow >= BOARD_HEIGHT) continue;
                for(int dj = 0; dj < n; dj++){
                    if (unplug[di][dj] == -1) break;
                    else sB[y_min - n + di][x_min + dj] = unplug[di][dj];
                }
            }
            break;

        case 2:
            for(int di = 0; di < n; di++){
                int curtRow = y_min + di;
                if (curtRow >= BOARD_HEIGHT)
                    break;
                else {
                    copy(sB[curtRow].begin() + x_min, sB[curtRow].begin() + x_min + n, unplug[di].begin());
                    sB[curtRow].erase(sB[curtRow].begin() + x_min, sB[curtRow].begin() + x_min + n);
                    sB[curtRow].insert(sB[curtRow].end(), unplug[di].begin(), unplug[di].begin() + n);
                }
            }
            break;

        case 3:
        //使わない？
            break;
        
    }
    board_2d = sB;

    k = 0;
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board.at(k) = board_2d[i][j];
            k++;
        }
    }

    return board;

    

    
    
    //cout << MOVE << "手目" << endl;

    /* for(int i=0; i<size.at(piece_num).at(0); i++){
        for(int j=0; j<size.at(piece_num).at(1); j++){
            cout << "[" << nukigata.at(piece_num).at(i).at(j) << "]";
        }
        cout << endl;
    }  */

    cout  << "抜き型No." <<  piece_num << "を適用" << endl;
    int x_max = x_min + size.at(piece_num).at(0) - 1;
    int y_max = y_min + size.at(piece_num).at(1) - 1;
    
    
    //範囲表示。
    cout << "型抜き範囲x:" << x_min << " ~ " << x_max <<endl;
    cout << "型抜き範囲y:" << y_min << " ~ " << y_max << endl;
    //cout << "抜き出したもの" << endl;
     

    //型抜き
    vector<int> kata_1d(256*256, 0);
    int kata_1d_in = 0;
    for(int i=0; i < size.at(piece_num).at(0); i++){
        for(int j=0; j < size.at(piece_num).at(1); j++){ 
            if(y_min + i >= BOARD_HEIGHT || x_min + j >= BOARD_WIDTH || y_min + i < 0 || x_min + j < 0){             //board_sizeまで到達（board_size番目に到達したら考えない。）

            }else{
                if(nukigata.at(piece_num).at(i).at(j) == 1){
                    kata_1d.at(kata_1d_in) = board.at((y_min+i) * BOARD_WIDTH + (x_min + j)); //一次元配列に格納
                    
                    board.at((y_min+i) * BOARD_WIDTH + (x_min + j)) = 9; //抜かれた部分は9にする
    
                    //cout << "[" << kata_1d.at(kata_1d_in)  << "]" ; 
                    kata_1d_in++;
                }
            }
        }
       // cout << endl;
    }

   // cout << endl << "方向:";
    switch (direction)
    {
    case 0: //上方向
        //cout << "上方向" << endl;
        for(int i=0; i<BOARD_HEIGHT; i++){
            for(int j=0; j<BOARD_WIDTH; j++){
                if(i != 0){
                    if(board.at(i * BOARD_WIDTH + j) != 9 && board.at((i-1)* BOARD_WIDTH +j) == 9){
                        board.at((i-1)* BOARD_WIDTH + j) = board.at(i*BOARD_WIDTH + j);
                        board.at(i*BOARD_WIDTH + j) = 9;
                        i -=1;
                        j -=1;
                    }
                }
                
            }
        } 
        break;

    case 1: //下方向
        //cout << "下方向" << endl;
        for(int i=BOARD_HEIGHT-1; i>-1; i--){
            for(int j=0; j<BOARD_WIDTH; j++){
                if(i != BOARD_HEIGHT-1){
                    if(board.at(i* BOARD_WIDTH + j) != 9 && board.at((i+1)*BOARD_WIDTH + j) == 9){
                        board.at((i+1)*BOARD_WIDTH +j) = board.at(i*BOARD_WIDTH +j);
                        board.at(i*BOARD_WIDTH +j) = 9;
                        i +=1;
                        j -=1;
                    }
                }
                
            }
        } 
        break;

    case 2: //左方向
        //cout << "左方向" << endl;
        for(int j=0; j<BOARD_WIDTH; j++){
            for(int i=0; i<BOARD_HEIGHT; i++){
                if(j != 0){
                    if(board.at(i* BOARD_WIDTH +j) != 9 && board.at(i*BOARD_WIDTH +(j-1)) == 9){
                        board.at(i*BOARD_WIDTH +(j-1)) = board.at(i*BOARD_WIDTH + j);
                        board.at(i*BOARD_WIDTH + j) = 9;
                        i -=1;
                        j -=1;   
                    }                   
                }
            }
        } 
        break;    

    case 3: //右方向
        //cout << "右方向" << endl;
        for(int j=BOARD_WIDTH-1; j>-1; j--){
            for(int i=0; i<BOARD_HEIGHT; i++){
                if(j != BOARD_WIDTH-1){
                    if(board.at(i*BOARD_WIDTH +j) != 9 && board.at(i*BOARD_WIDTH + (j+1)) == 9){
                        board.at(i*BOARD_WIDTH + (j+1)) = board.at(i*BOARD_WIDTH +j);
                        board.at(i*BOARD_WIDTH +j) = 9;
                        i -=1;
                        j +=1;   
                    }                   
                }
            }
        } 
        break;            
    default:
        break;
    }

    //空いているところ（9）に抜いた部分を入れる
    int fill_num = 0;
    for(int i=0; i<BOARD_HEIGHT; i++){
        for(int j=0; j<BOARD_WIDTH; j++){
            if(board.at(i* BOARD_WIDTH +j) == 9){
                board.at(i* BOARD_WIDTH + j) = kata_1d.at(fill_num);
                fill_num++;

            }
        }
    }

    return board;
}