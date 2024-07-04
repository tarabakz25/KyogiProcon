#include <bits/stdc++.h>
#include <chrono>
#include <unistd.h>
#include <algorithm>
#include <math.h>

#include <string> //read&write
#include <fstream>
#include <sstream>


#include "board_setting_0.4.1.cpp"
#include "evaluation_0.2.0.cpp"

vector<vector<double>> choices(100000, vector<double>(6));
vector<int> ranking(100,{-1});  //top10, depth5

vector<vector<double>> Choices_All(200, vector<double>(6));
vector<int> Ranking_All(200,{-1});
vector<double> Ranking_Score(200, {-1});
int Ranking_in = 0;


int SEARCH_WIDTH = 1000; //探索するノードの数 //冒険の数
int SELECT = 10;    //選別される数　    //視野の広さ
int SEARCH_DEPTH =3;                      //思考力
using namespace std;


void search_next(int road_num){
    int kata_num;
    int katanuki_x;
    int katanuki_y;
    int move_direction;
    int root_node_num;

    root_node_num = 0;

    for(int i=0; i<SEARCH_WIDTH; i++){
        kata_num = rand() %  8;
        katanuki_x = rand() % (board_size_width*2) - board_size_width;
        katanuki_y = rand() % (board_size_height*2) - board_size_height;
        move_direction = rand() % 4;
        
        board_road(road_num);
        katanuki(kata_num, katanuki_x, katanuki_y, move_direction); 


        if(compare_with0() != 100){ //ID0(最初の手)と比べて、変更点があるならOK、ないならやり直し
            //show_board(1);
            //cout << "\x1b[33m" << MOVE << "手目(候補)"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
            choices.at(root_node_num).at(0) = kata_num;
            choices.at(root_node_num).at(1) = katanuki_x;
            choices.at(root_node_num).at(2) = katanuki_y;
            choices.at(root_node_num).at(3) = move_direction;
            choices.at(root_node_num).at(4) = 100*(count_match()+0.000) / (board_size_width * board_size_height);

            choices.at(root_node_num).at(5) =  Evaluation(root_node_num);  //評価関数 Evaluation(候補リストの中の番号);
            
            
            //候補表示用
            //cout << "Option" << root_node_num << " Kata_num:" <<choices.at(root_node_num).at(0) << " X:" <<choices.at(root_node_num).at(1) << " Y:" <<choices.at(root_node_num).at(2) << " Direc:" <<choices.at(root_node_num).at(3) << " MATCH:" <<choices.at(root_node_num).at(4) + 0.0<< endl;
            
            root_node_num++;
        }else{
            i--;
        }

        
    }
}

void select_higher(){
    int input_rank;
    int highest_num;
    int highest_data;
    
    //ランキング初期化
    for (int i = 0; i < 100; i++){
        ranking.at(i) = -1;
    }
    

    //1位(配列の0番目)を決める
    input_rank = 0;
    highest_num = 0;
    highest_data = -1;
    for(int i=0; i<SEARCH_WIDTH; i++){
        if(choices.at(i).at(5) > highest_data){
            highest_num = i;
            highest_data = choices.at(highest_num).at(5);
        }
    }
    ranking.at(input_rank) = highest_num;

    for(int n=0; n<SELECT-1; n++){
        //n+2位(配列のn+1番目)
        input_rank = n+1;
        highest_num = 0;
        highest_data = -1;
        for(int i=0; i<SEARCH_WIDTH; i++){
            if(choices.at(i).at(5) > highest_data){
                int exists = 0;
                for(int j=0; j<ranking.size(); j++){
                    if(i == ranking.at(j)){
                        exists++;
                    }
                }
                if(exists == 0){
                    highest_num = i;
                    highest_data = choices.at(highest_num).at(5);
                }
            }   
        }
        ranking.at(input_rank) = highest_num;
    }

}        

double Search(int parent_id, int parentdepth){
    int my_depth = parentdepth+1;

    int start_sibling;
    start_sibling = (pow(SELECT, my_depth) -1) / (SELECT  -1) ; //開始位置を設定
    start_sibling += SELECT * (parent_id - (((pow(SELECT, parentdepth)) - 1) / (SELECT-1)));


    board_road(parent_id);
    search_next(parent_id); //次の1手をランダムに作る searchnext(今の手のid)


    
    select_higher();

    

    //ランキング上位の手をセーブ

    for(int i=0; i<SELECT; i++){
        board_road(parent_id);
        katanuki(choices.at(ranking.at(i)).at(0), choices.at(ranking.at(i)).at(1), choices.at(ranking.at(i)).at(2), choices.at(ranking.at(i)).at(3));
        board_save(start_sibling + i);

        if(parentdepth == 0){  //親ノードが初手(ID0)の時、この手は1手目候補のためとっておく
            Choices_All.at(i).at(0) = choices.at(i).at(0);
            Choices_All.at(i).at(1) = choices.at(i).at(1);
            Choices_All.at(i).at(2) = choices.at(i).at(2);
            Choices_All.at(i).at(3) = choices.at(i).at(3);
            Choices_All.at(i).at(4) = choices.at(i).at(4);
            Choices_All.at(i).at(5) = choices.at(i).at(5);
            //cout << "初手" << choices.at(i).at(0);
        }

    }

    double Ev_sum = 0;

    if(my_depth == SEARCH_DEPTH){ //底についた
        //評価値を合計
        for(int i=0; i<SELECT; i++){
            Ev_sum += choices.at(ranking.at(i)).at(5);
            //for(int j=0; j<my_depth; j++){cout << "-";}
            
            if(choices.at(ranking.at(i)).at(4) == 100){ //100%の手が見つかったら絶対それにする。
                Ev_sum += 10000;
                //cout << start_sibling+i <<"this node is "  << choices.at(ranking.at(i)).at(5) + 10000  << endl;
            }else{
                //cout << start_sibling+i <<"this node is "  << choices.at(ranking.at(i)).at(5)  << endl;
            }
            
        }
        if(parent_id <= SELECT){
            //for(int j=0; j<parentdepth; j++){cout << "-";}
            //cout << parent_id << "<------this node is"  << Ev_sum / SELECT << endl; //平均をとる
            Ranking_All.at(Ranking_in) = parent_id;
            Ranking_Score.at(Ranking_in) = Ev_sum / SELECT;  //パラメータ調整必要
            Ranking_in++;

        }else{
            //for(int j=0; j<parentdepth; j++){cout << "-";}
            //cout << parent_id <<"this node is "  << Ev_sum / SELECT << endl; //平均をとる
        }
        return Ev_sum / SELECT;


    }else{  //まだ下に伸ばせる
        for(int i; i<SELECT; i++){
            Ev_sum += Search(start_sibling+i, my_depth); //下を探す
            if(choices.at(ranking.at(i)).at(4) == 100){ //100%の手なら絶対それにする。
                Ev_sum += 100000;
            }
        }

        if(parent_id <= SELECT && parent_id != 0){
            //for(int j=0; j<parentdepth; j++){cout << "-";}
            //cout << parent_id << "<------this node is"  << Ev_sum / SELECT << endl; //平均をとる
            Ranking_All.at(Ranking_in) = parent_id;
            Ranking_Score.at(Ranking_in) = Ev_sum / SELECT;
            Ranking_in++;

        }else{
            //for(int j=0; j<parentdepth-1; j++){cout << "-";}
            //cout << parent_id <<"this node is "  << Ev_sum / SELECT << endl; //平均をとる
        }
        
        return Ev_sum  / SELECT;
    }
}




int main(){
    string input_csv_file_path = "";
    string output_csv_file_path = "test.csv";

    ifstream ifs_csv_file(input_csv_file_path  );

    ofstream outputfile(output_csv_file_path );
    outputfile << "move";
    outputfile << ",";
    outputfile << "score";
    outputfile << ",";
    outputfile << "match";
    outputfile << ",";
    outputfile << "\n";

    board_set();
    //system("clear");

    //実行時間計測用
    std::chrono::system_clock::time_point  start, end; 
    start = std::chrono::system_clock::now(); // 計測開始時間
    //ここまで



    //型抜き
    /*------------------------------------------------------------------------------------------------------
    | katanuki関数で型抜きを行えます。                                                                          |
    |                                                                                                       |
    | katanuki(型番号, 型抜きする場所の左上のx座標, 型抜きする場所の左上のy座標, 詰める方向(0:上, 1:下, 2:左, 3:右));    |
    | で呼び出してください。                                                                                    |
    ------------------------------------------------------------------------------------------------------*/
    
    /* 
    katanuki(6, 2, 2, 1); 
    show_board(1);
    cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
    MOVE++;

    katanuki(2, 5, 2, 3); 
    show_board(1);
    cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
    MOVE++;
    */

    Ranking_in = 0;
    board_save(0);
    

    int parentdepth = 0;
    int parent_id =0;

    

    //100%になるまで無限ループ
    while(100*(count_match()+0.0) / (board_size_width * board_size_height) != 100){

        //ランキング初期化
        for (int i = 0; i < 100; i++){
            Ranking_All.at(i) = -1;
            Ranking_Score.at(i) = -1;
        }
        Ranking_in = 0;

        //1手目選択肢初期化
        for (int i = 0; i < 100; i++){
            for(int j=0; j < 6; j++){
                Choices_All.at(i).at(j) = -1;
            }
            
        }

        

        Search(0,0);

        //探索結果、1手目の評価値を表示
        for(int j=0; j<SELECT; j++){
            //バランス調整
            Ranking_Score.at(j) = Ranking_Score.at(j) + (Choices_All.at(j).at(5) + 0.0 ) * 1.5; //初手のマッチ度を評価に含める。

            cout << "候補"<<Ranking_All.at(j)-1 << " 最終評価値:" << Ranking_Score.at(j);
            cout <<" 型番号:" <<Choices_All.at(j).at(0) << " X:" << Choices_All.at(j).at(1) << " Y:" <<Choices_All.at(j).at(2) << " Direc:" <<Choices_All.at(j).at(3)  <<" MATCH:" <<Choices_All.at(j).at(4) + 0.0  <<" (初手のみで評価)SCORE:"<< Choices_All.at(j).at(5) + 0.0 << "\n";
        }

        //1位を決める
        int input_rank = 0;
        int highest_num = 0;
        int highest_data = -1;

        int DESICION;
        for(int i=0; i<SELECT; i++){
            if(Ranking_Score.at(i) > highest_data){
                highest_num = i;
                highest_data = Ranking_Score.at(i);
            }
        }
        DESICION = highest_num;
        cout << "DESICION:" << DESICION << "\n";

        //system("clear");

        //条件を出しておく
        cout << "SEARCH_WIDTH:" << SEARCH_WIDTH <<"\n";
        cout << "NODE_SELECT:" << SELECT << "\n";
        cout << "SEARCH_DEPTH:" << SEARCH_DEPTH << "\n";


        board_road(0);
        //決定した手で型抜き
        katanuki(Choices_All.at(DESICION).at(0), Choices_All.at(DESICION).at(1), Choices_All.at(DESICION).at(2), Choices_All.at(DESICION).at(3));
        show_board(1);
        cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << "\n";
        MOVE++;

        board_save(0);



        cout << "SCORE:" << Ranking_Score.at(DESICION)<< "\n";
        outputfile << MOVE;
        outputfile << ",";
        outputfile << Ranking_Score.at(DESICION);
        outputfile << ",";
        outputfile << 100*(count_match()+0.0) / (board_size_width * board_size_height);
        outputfile << "\n";
        
        
        sleep(0);
        
            

        //system("clear");
        //表示用

        /* 
        cout << "1st:" << "候補" << ranking.at(0) << " 型番号:" <<choices.at(ranking.at(0)).at(0) << " X:" <<choices.at(ranking.at(0)).at(1) << " Y:" <<choices.at(ranking.at(0)).at(2) << " Direc:" <<choices.at(ranking.at(0)).at(3) << "\n" <<" MATCH:" <<choices.at(ranking.at(0)).at(4) + 0.0  << " SCORE:" << choices.at(ranking.at(0)).at(5) + 0.0<< endl;
        cout << "2nd:" << "候補" << ranking.at(1) << " 型番号:" <<choices.at(ranking.at(1)).at(0) << " X:" <<choices.at(ranking.at(1)).at(1) << " Y:" <<choices.at(ranking.at(1)).at(2) << " Direc:" <<choices.at(ranking.at(1)).at(3) << "\n" <<" MATCH:" <<choices.at(ranking.at(1)).at(4) + 0.0 << " SCORE:" << choices.at(ranking.at(1)).at(5) + 0.0<< endl;
        cout << "3rd:" << "候補" << ranking.at(2) << " 型番号:" <<choices.at(ranking.at(2)).at(0) << " X:" <<choices.at(ranking.at(2)).at(1) << " Y:" <<choices.at(ranking.at(2)).at(2) << " Direc:" <<choices.at(ranking.at(2)).at(3) << "\n" <<" MATCH:" <<choices.at(ranking.at(2)).at(4) + 0.0 << " SCORE:" << choices.at(ranking.at(2)).at(5) + 0.0<< endl;
        */
 


        
    }



    //実行時間計測用
    end = std::chrono::system_clock::now();  // 計測終了時間
    double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(); //処理に要した時間をマイクロ秒に変換
    cout << elapsed / 1000 << "ms" << "\n";
    //ここまで
}