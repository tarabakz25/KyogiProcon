#include <bits/stdc++.h>
#include <chrono>
#include <unistd.h>
#include <algorithm>
#include <math.h>

#include <string> //read&write
#include <fstream>
#include <sstream>


#include "board_setting.cpp"
#include "evaluation.cpp"

#include "json.hpp"
using namespace std;
using json = nlohmann::json;


using namespace std;

// 文字列からベクトルに変換する関数
vector<int> stringToVector(const string &str)
{
    vector<int> row;
    for (char c : str)
    {
        row.push_back(c - '0'); // 文字を整数に変換
    }
    return row;
}

// JSONからボードを読み込む関数
int loadBoard(const json &jobj, vector<vector<int>> &startBoard, vector<vector<int>> &goalBoard)
{
    for (const auto &line : jobj["board"]["start"])
    {
        startBoard.push_back(stringToVector(line.get<string>()));
    }
    for (const auto &line : jobj["board"]["goal"])
    {
        goalBoard.push_back(stringToVector(line.get<string>()));
    }
    return 0;
}

vector<vector<int>> compare_vartical(vector<vector<int>> compare1, vector<vector<int>> compare2, int size_x, int size_y){
	vector<vector<int>> numbers1(size_x, vector<int>(4, 0));
	vector<vector<int>> numbers2(size_x, vector<int>(4, 0));
    vector<vector<int>> result(size_x, vector<int>(4, 0));

	for(int i=0; i<size_x; i++){
		for(int j=0; j<size_y; j++){
			numbers1.at(i).at(compare1.at(j).at(i)) += 1; //横i列目を縦に見て、0の数を0番目、1の数を1番目...というように格納。
			numbers2.at(i).at(compare2.at(j).at(i)) += 1;
		}
		/*   cout << numbers1.at(i).at(0) << " ";
		cout << numbers1.at(i).at(1) << " ";
		cout << numbers1.at(i).at(2) << " ";
		cout << numbers1.at(i).at(3) << "　";
		cout << numbers2.at(i).at(0) << " ";
		cout << numbers2.at(i).at(1) << " ";
		cout << numbers2.at(i).at(2) << " ";
		cout << numbers2.at(i).at(3) << "\n";  */
        
        result.at(i).at(0) = abs(numbers1.at(i).at(0) - numbers2.at(i).at(0));
        result.at(i).at(1) = abs(numbers1.at(i).at(1) - numbers2.at(i).at(1));
        result.at(i).at(2) = abs(numbers1.at(i).at(2) - numbers2.at(i).at(2));
        result.at(i).at(3) = abs(numbers1.at(i).at(3) - numbers2.at(i).at(3));
	}
    return result;
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


	// JSONファイルの読み込み
    ifstream ifs("./sample1.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json jobj = json::parse(str);
    vector<vector<int>> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard);
    int boardSide = jobj["board"]["width"].get<int>();
    int boardWarp = jobj["board"]["height"].get<int>();

	board = startBoard;
	finish_board = goalBoard;

    show_setting_board(); //初期状態の表示


    cout << "かみやまひらがなこうせんは　たまねぎを　はなった！" << endl;
    cout << "＞＞「くらえッ　タマネギ！」" << endl;
    board_save(0);

    //実行時間計測用
    std::chrono::system_clock::time_point  start, end; 
    start = std::chrono::system_clock::now(); // 計測開始時間
    //ここまで

    //t


    //型抜き
    /*------------------------------------------------------------------------------------------------------
    | katanuki関数で型抜きを行えます。                                                                          |
    |                                                                                                       |
    | katanuki(型番号, 型抜きする場所の左上のx座標, 型抜きする場所の左上のy座標, 詰める方向(0:上, 1:下, 2:左, 3:右));    |
    | で呼び出してください。                                                                                    |
    ------------------------------------------------------------------------------------------------------*/
    
/* 
    katanuki(0, 1, 1, 1); 
    show_board(1);
    cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
    MOVE++;

    katanuki(1, 5, 2, 3); 
    show_board(1);
    cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
    MOVE++;
 */

	int result_before;
	int result_after;
	
	int check_x = 0;
	while(check_x < board_size_width){
		int shift_y = 0;
		while(shift_y < board_size_height){
				vector<vector<int>> vartical_check = compare_vartical(board, finish_board, board_size_width, board_size_height);
				/* cout << vartical_check.at(check_x).at(0) << "\n";
				cout << vartical_check.at(check_x).at(1) << "\n";
				cout << vartical_check.at(check_x).at(2) << "\n";
				cout << vartical_check.at(check_x).at(3) << "\n"; */

				result_before = vartical_check.at(check_x).at(0) + vartical_check.at(check_x).at(1) + vartical_check.at(check_x).at(2) + vartical_check.at(check_x).at(3);
				cout << result_before << "\n";


				board_save(1);
				cout << "shift_y = " << shift_y << "\n";
				katanuki(0, check_x, shift_y, 2);  //試す
				
				vartical_check = compare_vartical(board, finish_board, board_size_width, board_size_height); //評価
				/* cout << vartical_check.at(check_x).at(0) << "\n";
				cout << vartical_check.at(check_x).at(1) << "\n";
				cout << vartical_check.at(check_x).at(2) << "\n";
				cout << vartical_check.at(check_x).at(3) << "\n"; */

				result_after = vartical_check.at(check_x).at(0) + vartical_check.at(check_x).at(1) + vartical_check.at(check_x).at(2) + vartical_check.at(check_x).at(3);
				cout << result_after << "\n";

				if(result_before > result_after){
					//手を確定させる
					cout << "確定" << endl;
					show_board(1);
					cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
					MOVE++;
					result_before = result_after;

				}else{
					//もとに戻って無かった事にする。
					board_road(1);
				}
			
			if(result_after == 0){
				break;
			}

			shift_y += 1;

		}
		cout << "after"<<shift_y << endl;

		check_x++;
	}  





	

	vector<vector<int>> vartical_check = compare_vartical(board, finish_board, board_size_width, board_size_height);
				/* cout << vartical_check.at(check_x).at(0) << "\n";
				cout << vartical_check.at(check_x).at(1) << "\n";
				cout << vartical_check.at(check_x).at(2) << "\n";
				cout << vartical_check.at(check_x).at(3) << "\n"; */

	
	  

	
	
	//ここでwhileとじる。


    check_x = 0;
    int check_y = 0;

    for(int i=0; i<board_size_width; i++){
        for(int j=0; j<board_size_height; j++){
            while(board.at(j).at(i) != finish_board.at(j).at(i)){
                katanuki(0, i, j, 0); 
                show_board(1);
                cout << "\x1b[33m" << MOVE << "手目"   << "MATCH : " << 100*(count_match()+0.0) / (board_size_width * board_size_height) << "%" << "\x1b[m" << endl << endl;
                MOVE++; 
            }
        }
    } 
    





    //実行時間計測用
    end = std::chrono::system_clock::now();  // 計測終了時間
    double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count(); //処理に要した時間をマイクロ秒に変換
    cout << elapsed / 1000 << "ms" << "\n";
    //ここまで
}