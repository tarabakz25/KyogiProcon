#include <vector>
#include <iostream>
#include "setting.hpp"
using namespace std;

vector<vector<int>> define_size(){
    vector<vector<int>> size(30, vector<int>(2));
    //size.at(0) = {1,1};
    size.at(0).at(0) = 1;
    size.at(0).at(1) = 1;
    
    for(int i=1,t=2; i<24; i++){
        size.at(i).at(0) = t;
        size.at(i).at(1) = t; 
        size.at(i+1).at(0) = t;
        size.at(i+1).at(1) = t;
        size.at(i+2).at(0) = t;
        size.at(i+2).at(1) = t;

        i += 2;
        t *= 2;
    }

    //抜き型サイズ表示用(あとで消す)
    /* for(int i=0; i < 25; i++){
        cout << "piece" << i << ";" << size.at(i).at(0) << endl;
    } */
    return size;
}

vector<vector<vector<int> > > define_nukigata(vector<vector<int>> size){ 
    vector<vector<vector<int> > > nukigata(25);
    for(int p = 0; p < 25; p++) {
        nukigata[p].resize(size[p][0], vector<int>(size[p][1], 0)); // それぞれのサイズに合わせて初期化
    }

    int p = 0;
    nukigata.at(p).at(0).at(0) = 1; //1*1のボード

    //2*2以上の定型抜き型(24個)を生成。
    p = 1;
    while(p<24){
        //typeI
        for(int i=0; i < size.at(p).at(0); i++){
            for(int j=0; j < size.at(p).at(1); j++){
                nukigata.at(p).at(i).at(j) = 1;
            }
        }
        p++;

        //typeII
        for(int i=0; i < size.at(p).at(0); i++){
            for(int j=0; j < size.at(p).at(1); j++){
                nukigata.at(p).at(i).at(j) = 1;
            }
            i++;
        }
        p++;

        //typeIII
        for(int j=0; j < size.at(p).at(1); j++){
            for(int i=0; i < size.at(p).at(0); i++){
                nukigata.at(p).at(i).at(j) = 1;
            }
            j++;
        }
        p++;
    }

    //抜き型表示用（仮。あとで消す）

    for(int i=0; i<10; i++){
        cout << "nukigata_num : " << i << endl;
        for(int j=0; j<size.at(i).at(0); j++){
            for(int k=0; k<size.at(i).at(1); k++){
                cout << "[" << nukigata.at(i).at(j).at(k) << "]";
            }    
            cout << endl;
        }
        cout << endl;
    }    
    return nukigata;
}




vector<int> katanuki(int piece_num, int x_min, int y_min, int direction, vector<vector<int>>& size, vector<vector<vector<int>>>& nukigata, vector<int> board, int BOARD_WIDTH, int BOARD_HEIGHT){
    
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
    cout << "型抜き範囲x:" << x_min << " - " << x_max <<endl;
    cout << "型抜き範囲y:" << y_min << " - " << y_max << endl;
    //cout << "抜き出したもの" << endl;
     

    //型抜き
    vector<int> kata_1d(256*256, 0);
    int kata_1d_in = 0;
    for(int i=0; i < size.at(piece_num).at(0); i++){
        for(int j=0; j < size.at(piece_num).at(1); j++){ 
            if(y_min + i >= BOARD_HEIGHT || x_min + j >= BOARD_WIDTH || y_min + i < 0 || x_min + j < 0){             //board_sizeまで到達（board_size番目に到達したら考えない。）

            }else{
                if(nukigata.at(piece_num).at(i).at(j) == 1){
                    kata_1d.at(kata_1d_in) = board.at(y_min+i * BOARD_WIDTH + (x_min + j)); //一次元配列に格納
                    
                    
                    board.at(y_min+i * BOARD_WIDTH + (x_min + j)) = 9; //抜かれた部分は9にする
    
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