#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

int board_size_width =  6;  //盤面の大きさ
int board_size_height = 6;
int MOVE = 1;

vector<vector<int>> board(board_size_height, vector<int>(board_size_width)); //型番号、縦、横の順番
vector<vector<int>> finish_board(board_size_height, vector<int>(board_size_width));
vector<vector<vector<int>>> saved_board(1000000, vector<vector<int>>(board_size_height,vector<int>(board_size_width,0)));

vector<vector<vector<int>>> nukigata(30, vector<vector<int>>(256, vector<int>(256, 0)));
vector<vector<int>> size(30, vector<int>(2));


void define_nukigata(){
    size.at(0) = {1,1};
    
    for(int i=1,t=2; i<24; i++){
        size.at(i) = {t,t};
        size.at(i+1) = {t,t};
        size.at(i+2) = {t,t};
       
        //抜き型サイズ表示用(あとで消す)
        //cout << "piece" << i << ";" << size.at(i).at(0) << endl;
        //cout << "piece" << i+1 << ";" << size.at(i+1).at(0) << endl;
        //cout << "piece" << i+2 << ";" << size.at(i+2).at(0) << endl;
        i += 2;
        t *= 2;
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
    /*
    string str;
    cout << "Do you want to show Nukigatas? [Yes:y, No:other] ";
    cin >> str;
    if(str == "y"){
        for(int i=0; i<25; i++){
            cout << "nukigata_num : " << i << endl;
            for(int j=0; j<size.at(i).at(0); j++){
                for(int k=0; k<size.at(i).at(1); k++){
                    cout << "[" << nukigata.at(i).at(j).at(k) << "]";
                }    
                cout << endl;
            }
            cout << endl;
        }    
    }
    */
}    

void random_start_board(int num[4]){

    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            board.at(i).at(j) = rand() % 4;
            //board.at(i).at(j) = (i+j) % 4;
             num[board.at(i).at(j)] += 1;
        }
    }
}

void make_start_board(int num[4]){

    int k = 0;
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            board.at(i).at(j) = k % 4;
            num[board.at(i).at(j)] += 1;
            k++;
        }
    }
}

void make_finish_board(int num[4]){
    int add = 0;
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            if(num[add] == 0){
                add++;
            }
            finish_board.at(i).at(j) = add;
            num[finish_board.at(i).at(j)] -= 1;
        }
    }
}


void random_finish_board(int num[4]){
    int random;
    int shuffle1[2] ={0};
    int shuffle2[2] ={0};

    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            random = rand() % 4;
            while(num[random] == 0){
                 random = rand() % 4;
            }
            finish_board.at(i).at(j) = random;
            num[finish_board.at(i).at(j)] -= 1;
        }
    }
    //shuffle
    for(int k=0; k<20; k++){    //シャッフル回数。多いほど混ざりやすい？
        for(int i=0; i<board_size_height; i++){
            for(int j=0; j<board_size_width; j++){
                shuffle1[0] = rand() % board_size_height;
                shuffle1[1] = rand() % board_size_width;
                shuffle2[0] = rand() % board_size_height;
                shuffle2[1] = rand() % board_size_width;
                int save = finish_board.at(shuffle1[0]).at(shuffle1[1]);

                finish_board.at(shuffle1[0]).at(shuffle1[1]) = finish_board.at(shuffle2[0]).at(shuffle2[1]);
                finish_board.at(shuffle2[0]).at(shuffle2[1]) = save;
            }
        }
    }    
}

void board_save(int save_num){
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            saved_board.at(save_num).at(i).at(j) = board.at(i).at(j);
        }
    }    
}

void board_road(int road_num){
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            board.at(i).at(j) = saved_board.at(road_num).at(i).at(j);
        }
    }    
}


void show_first_board(){
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            cout << "[" << board.at(i).at(j) << "]";
        }
        cout << endl;
    }
}

void show_board(int light){
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            if(board.at(i).at(j) == finish_board.at(i).at(j) && light == 1){
                cout <<  "\x1b[33m" << "[" << board.at(i).at(j) << "]" << "\x1b[m" ;  //マッチしているものは黄色に　
            }else{
                cout << "[" << board.at(i).at(j) << "]";
            }    
        }
        cout << endl;
    }
}

//あとで消す
void show_saved_board(int saved_num, int light){
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            if(saved_board.at(saved_num).at(i).at(j) == finish_board.at(i).at(j) && light == 1){
                cout <<  "\x1b[33m" << "[" << saved_board.at(saved_num).at(i).at(j) << "]" << "\x1b[m" ;  //マッチしているものは黄色に　
            }else{
                cout << "[" << saved_board.at(saved_num).at(i).at(j) << "]";
            }    
        }
        cout << endl;
    }
}

void show_finish_board(){
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            cout << "[" << finish_board.at(i).at(j) << "]";
               
        }
        cout << endl;
    }
}

int count_match(){
    int x = 0;
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            if(board.at(i).at(j) == finish_board.at(i).at(j)){
                x++;
            }
        }
    }
    return x;
}

int compare_with0(){ //ID0(最初のノード)と比較する（100%の場合変化していない）
    int x = 0;
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            if(board.at(i).at(j) == saved_board.at(0).at(i).at(j)){
                x++;
            }
        }
    }
    return 100*(x+0.0) / (board_size_width * board_size_height);
}


void katanuki(int piece_num, int x_min, int y_min, int direction){
    
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
            if(y_min + i >= board_size_height || x_min + j >= board_size_width || y_min + i < 0 || x_min + j < 0){             //board_sizeまで到達（board_size番目に到達したら考えない。）

            }else{
                if(nukigata.at(piece_num).at(i).at(j) == 1){
                    kata_1d.at(kata_1d_in) = board.at(y_min + i).at(x_min + j); //一次元配列に格納
                    
                    
                    board.at(y_min + i).at(x_min + j) = 9; //抜かれた部分は9にする
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
        for(int i=0; i<board_size_height; i++){
            for(int j=0; j<board_size_width; j++){
                if(i != 0){
                    if(board.at(i).at(j) != 9 && board.at(i-1).at(j) == 9){
                        board.at(i-1).at(j) = board.at(i).at(j);
                        board.at(i).at(j) = 9;
                        i -=1;
                        j -=1;
                    }
                }
                
            }
        } 
        break;

    case 1: //下方向
        //cout << "下方向" << endl;
        for(int i=board_size_height-1; i>-1; i--){
            for(int j=0; j<board_size_width; j++){
                if(i != board_size_height-1){
                    if(board.at(i).at(j) != 9 && board.at(i+1).at(j) == 9){
                        board.at(i+1).at(j) = board.at(i).at(j);
                        board.at(i).at(j) = 9;
                        i +=1;
                        j -=1;
                    }
                }
                
            }
        } 
        break;

    case 2: //左方向
        //cout << "左方向" << endl;
        for(int j=0; j<board_size_width; j++){
            for(int i=0; i<board_size_height; i++){
                if(j != 0){
                    if(board.at(i).at(j) != 9 && board.at(i).at(j-1) == 9){
                        board.at(i).at(j-1) = board.at(i).at(j);
                        board.at(i).at(j) = 9;
                        i -=1;
                        j -=1;   
                    }                   
                }
            }
        } 
        break;    

    case 3: //右方向
        //cout << "右方向" << endl;
        for(int j=board_size_width-1; j>-1; j--){
            for(int i=0; i<board_size_height; i++){
                if(j != board_size_width-1){
                    if(board.at(i).at(j) != 9 && board.at(i).at(j+1) == 9){
                        board.at(i).at(j+1) = board.at(i).at(j);
                        board.at(i).at(j) = 9;
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
    for(int i=0; i<board_size_height; i++){
        for(int j=0; j<board_size_width; j++){
            if(board.at(i).at(j) == 9){
                board.at(i).at(j) = kata_1d.at(fill_num);
                fill_num++;

            }
        }
    }
}


void start_and_finish(){


    int used_num[4] ={0};
    int used_num_counter[4] = {0};

    srand(time(NULL));
    //random_start_board(used_num);  最初の盤面をランダムに生成する
    make_start_board(used_num); ////最初の盤面を規則的に生成

    for(int i = 0; i<4; i++){
        used_num_counter[i] = used_num[i] ; //使用数字カウンターにコピー
    }
    
    //random_finish_board(used_num);  //ランダム
    make_finish_board(used_num);  //最終状態を生成する


	cout << "0: " <<used_num_counter[0] << endl;
    cout << "1: " <<used_num_counter[1] << endl;
    cout << "2: " <<used_num_counter[2] << endl;
    cout << "3: " <<used_num_counter[3] << endl;
}


void show_setting_board(){
	define_nukigata(); //抜き型を生成。
	//start_and_finish();

    //表示用 
    cout << "\x1b[31m" << "\x1b[1m" << "start" << "\x1b[m" << endl;
    show_first_board();
    cout << endl << "\x1b[31m" << "\x1b[1m" << "finish" << "\x1b[m" << endl;
    show_finish_board();
	cout << "\n";

    //操作前画面表示
    cout << "0手目(操作前)" << endl;
    show_board(1);
    cout << "\x1b[33m" << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;


}