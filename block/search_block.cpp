vector<vector<int>> blockcheck_now(board_size_height, vector<int>(board_size_width)); //型番号、縦、横の順番
vector<vector<int>> blockcheck_end(board_size_height, vector<int>(board_size_width));

void comparison(int block_type, int now_x, int now_y, int end_x, int end_y){
    int count = 0;
    int use = 1; //干渉していたら不採用、最初は採用にする。

    for(int i=0; i<block_type; i++){
        for(int j=0; j<block_type; j++){
            if(blockcheck_now.at(now_y+i).at(now_x+j) == 1){
                use = 0; //はじめ　他のブロックに干渉していたらふさいよう
                break;
            }
            if(blockcheck_end.at(end_y+i).at(end_x+j) == 1){
                use = 0; //おわり　他のブロックに干渉していたらふさいよう
                break;
            }
            if(board.at(now_y+i).at(now_x+j) == finish_board.at(end_y+i).at(end_x+j)){
                count++;
            }
        }
        if(use == 0) break;
    }

    if(use == 1){
        if(count == block_type * block_type){
            //ブロックを作った場所を埋めておく
            for(int i=0; i<block_type; i++){
                for(int j=0; j<block_type; j++){
                    blockcheck_now.at(now_y+i).at(now_x+j) = 1;
                    blockcheck_end.at(end_y+i).at(end_x+j) = 1;
                }
            }

            cout << "\033[33m";
            cout << "p" << block_type <<" s" << now_x << "," <<  now_y << ",. e" << end_x << "," << end_y << "が一致" << endl;
            cout << "\033[37m";
        }
    }
    
    
}

void search_block(){
    int search_now_x;
    int search_now_y;
    int search_end_x;
    int search_end_y;
    int block_type = 4;

    //重複チェック用配列の初期化
    for(int i = 0; i<board_size_height; i++){
        for(int j = 0; j<board_size_width; j++){
            blockcheck_now.at(i).at(j) = 0;
            blockcheck_now.at(i).at(j) = 0;
        }
    }
    block_type = 2;

    search_now_x = 0;
    search_now_y = 0;
    search_end_x = 0;
    search_end_y = 0;

    for(int i = 0; i < board_size_width - block_type + 1; i++){
        for(int j = 0; j < board_size_height - block_type + 1; j++){
            for(int k = 0; k < board_size_width - block_type + 1; k++){
                for(int l = 0; l < board_size_height - block_type + 1; l++){
                    comparison(block_type, i, j, k, l);
                }
            }
        }
    }
    cout << "\n";


    

}