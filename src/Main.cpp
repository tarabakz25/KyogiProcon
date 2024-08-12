#include <Siv3D.hpp> // Siv3D v0.6.15
#include <string.h>
#include "json.hpp"
//using namespace nlohman;
using namespace std;

int BOARD_WIDTH;
int BOARD_HEIGHT;

void Board_draw(int position_x, int position_y, int side_length, vector<int> &board_now, const Font& font);
void comparison(int block_type, int now_x, int now_y, int end_x, int end_y, vector<int>& board_now, vector<int>& board_finish, vector<int>& blockcheck_now, vector<int>& blockcheck_end);
vector<vector<int>> search_block(vector<int>& board_now, vector<int>& board_finish);

void Main()
{
	//gui setting
	//Scene::SetBackground(ColorF{ 0.0, 0.0, 0.0 });
	const int side_length = 14;
    const Font font{ FontMethod::MSDF, 12, Typeface::Bold };
	

    BOARD_WIDTH = 20;
    BOARD_HEIGHT = 20;

	vector<int> board_now (BOARD_WIDTH * BOARD_HEIGHT, 0);
    vector<int> board_finish (BOARD_WIDTH * BOARD_HEIGHT, 0);
    vector<vector<int>> blockcheck_result;

	//board_setting
	for(int i = 0; i < BOARD_HEIGHT; i++){
		for(int j = 0; j < BOARD_WIDTH; j++){
			board_now.at(BOARD_WIDTH * i + j) = rand() % 4;
		}
	}
    for(int i = 0; i < BOARD_HEIGHT; i++){
		for(int j = 0; j < BOARD_WIDTH; j++){
			board_finish.at(BOARD_WIDTH * i + j) = rand() % 4;
		}
	}

    blockcheck_result = search_block(board_now, board_finish);

    for(int i = 0; i < blockcheck_result.size(); i++){

        cout << "\033[33m";
        cout << "p" << blockcheck_result.at(i).at(0) <<" s" << blockcheck_result.at(i).at(1) << "," <<  blockcheck_result.at(i).at(2) << ",. e" << blockcheck_result.at(i).at(3) << "," << blockcheck_result.at(i).at(4) << "が一致" << endl;
        cout << "\033[37m";
        
    }


	
	while (System::Update())
	{
	 	//gui_drawing
        font(U"NOW").draw(10, 25, ColorF{ 1.0, 1.0, 1.0 });
        Board_draw(10, 50, side_length, board_now, font);
        font(U"FINISH").draw(50 + side_length * BOARD_WIDTH, 25, ColorF{ 1.0, 1.0, 1.0 });
        Board_draw(50 + 14 * BOARD_WIDTH, 50, side_length, board_finish, font);

        for(int i = 0; i < blockcheck_result.size(); i++){
             Rect{ 10 + side_length * blockcheck_result.at(i).at(1), 50 + side_length * blockcheck_result.at(i).at(2), side_length*blockcheck_result.at(i).at(0), side_length*blockcheck_result.at(i).at(0) }.drawFrame(0.8, 0.8, Palette::Red);
             Rect{ 50 + side_length * BOARD_WIDTH +  side_length * blockcheck_result.at(i).at(3), 50 + side_length * blockcheck_result.at(i).at(4), side_length*blockcheck_result.at(i).at(0), side_length*blockcheck_result.at(i).at(0) }.drawFrame(0.8, 0.8, Palette::Red);
        
        }

		
	}
}


void Board_draw(int position_x, int position_y, int side_length, vector<int> &board_now, const Font& font){

    for(int x = 0; x<BOARD_HEIGHT; x++){
            for(int y =0; y<BOARD_WIDTH; y++){
                switch(board_now.at(x + BOARD_WIDTH * y)){
                    case 0:
                        Rect{ position_x + side_length * x, position_y + side_length * y, side_length, side_length }.draw(Palette::White);
                        font(U"0").draw( position_x + 3 + side_length * x, position_y-1 + side_length * y, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 1:
                        Rect{ position_x + side_length * x, position_y + side_length * y, side_length, side_length }.draw(Palette::Pink);
                        font(U"1").draw( position_x + 3  + side_length * x, position_y-1 + side_length * y, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 2:
                        Rect{ position_x + side_length * x, position_y + side_length * y, side_length, side_length }.draw(Palette::Khaki);
                        font(U"2").draw( position_x + 3 + side_length * x, position_y-1 + side_length * y, ColorF{ 0.0, 0.0, 0.0 });
                        break;

                    case 3:
                        Rect{ position_x + side_length * x, position_y + side_length * y, side_length, side_length }.draw(Palette::Lightblue);
                        font(U"3").draw( position_x + 3  + side_length * x, position_y-1 + side_length * y, ColorF{ 0.0, 0.0, 0.0 });
                        break;
                }
            }
        }
}





void comparison(int block_type, int now_x, int now_y, int end_x, int end_y, vector<int>& board_now, vector<int>& board_finish, vector<int>& blockcheck_now, vector<int>& blockcheck_end, vector<vector<int>>& blockcheck_result){
    int count = 0;
    int use = 1; //干渉していたら不採用、最初は採用にする。

    for(int i=0; i<block_type; i++){
        for(int j=0; j<block_type; j++){
            if(blockcheck_now.at(now_x+j + BOARD_WIDTH*(now_y+i)) == 1){
                use = 0; //はじめ　他のブロックに干渉していたらふさいよう
                break;
            }
            if(blockcheck_end.at(end_x+j + BOARD_WIDTH*(end_y+i)) == 1){
                use = 0; //おわり　他のブロックに干渉していたらふさいよう
                break;
            }
            if(board_now.at(now_x+j + BOARD_WIDTH*(now_y+i)) == board_finish.at(end_x+j + BOARD_WIDTH*(end_y+i))){
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
                    blockcheck_now.at(now_x+j + BOARD_WIDTH*(now_y+i)) = 1;
                    blockcheck_end.at(end_x+j + BOARD_WIDTH*(end_y+i)) = 1;
                }
            }
            
            blockcheck_result.push_back({block_type, now_x, now_y, end_x, end_y});


        }
    }
    
    
}

vector<vector<int>> search_block(vector<int>& board_now, vector<int>& board_finish){
    vector<int> blockcheck_now(BOARD_WIDTH * BOARD_HEIGHT, 0); 
    vector<int> blockcheck_end(BOARD_WIDTH * BOARD_HEIGHT, 0);
    vector<vector<int>> blockcheck_result;
    int block_type = 4;

    //重複チェック用配列の初期化
    for(int i = 0; i<BOARD_HEIGHT; i++){
        for(int j = 0; j<BOARD_WIDTH; j++){
            blockcheck_now.at(j+ BOARD_WIDTH* i) = 0;
            //blockcheck_now.at(i).at(j) = 0;
        }
    }
    block_type = 2;

    for(int i = 0; i < BOARD_WIDTH - block_type + 1; i++){
        for(int j = 0; j < BOARD_HEIGHT - block_type + 1; j++){
            for(int k = 0; k < BOARD_WIDTH - block_type + 1; k++){
                for(int l = 0; l < BOARD_HEIGHT - block_type + 1; l++){
                    comparison(block_type, i, j, k, l, board_now, board_finish, blockcheck_now, blockcheck_end, blockcheck_result);
                }
            }
        }
    }
 

    return blockcheck_result;


    

}