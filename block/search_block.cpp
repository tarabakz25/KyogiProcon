void comparison(int block_type, int now_x, int now_y, int end_x, int end_y){
    int count = 0;
    for(int i=0; i<block_type; i++){
        for(int j=0; j<block_type; j++){
            if(board.at(now_y+i).at(now_x+j) == finish_board.at(end_y+i).at(end_x+j)){
                count++;
            }
        }
    }

    if(count == block_type * block_type){
        cout << "\033[33m";
        cout << "p" << block_type <<" s" << now_x << "," <<  now_y << ",. e" << end_x << "," << end_y << "c" << count << endl;
        cout << "\033[37m";
    }
        //cout << "p" << block_type <<" s" << now_x << "," <<  now_y << ",. e" << end_x << "," << end_y << "c" << count << endl;
        //cout << "\033[37m";
    
}

void search_block(){
    int search_now_x;
    int search_now_y;
    int search_end_x;
    int search_end_y;
    int block_type = 4;

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

    block_type = 3;

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