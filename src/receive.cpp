#include <iostream>
#include <curl/curl.h>

#include <string> //read&write
#include <fstream>
#include <sstream>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json; //順番を維持する（通常アルファベット順になるらしい）



//コールバック関数
size_t Callback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t totalSize = size * nmemb;
    s->append((char*)contents, totalSize);
    return totalSize;
}


void receive_problem(){
    CURL* curl;
    CURLcode res; //リクエスト結果
    string readBuffer; //サーバーからのデータ保存

    curl = curl_easy_init();  //libcurlのイニシャライズ(初期化)
    if(curl){ //成功した、返されたら
        //URL設定
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/problem");

        //HTTPヘッダ設定
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Procon-Token: token1"); //ヘッダーリストにトークン追加
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // HTTPヘッダーを設定

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback); //レスポンスを受け取ったら、Callback()が呼ばれる。
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); //書き込まれる場所を指定

        res = curl_easy_perform(curl); //リクエスト実行
        
        if(res != CURLE_OK){ //リクエスト失敗？
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }else{
            cout << "Response: " << readBuffer << endl;

            //アクセスタイムエラー（サーバが動いていない場合）なら飛ばす。
            if (readBuffer.find("AccessTimeError") != std::string::npos) {
                cout << "(状態) サーバーが動いていません。" << endl;
                return;
            } 

            
            //jsonファイルとして書き込む
            ordered_json problem = ordered_json::parse(readBuffer);

            ofstream write_file("./src/problem.json");
            if (write_file.is_open()) {
                write_file << problem.dump(4); //からファイルを入れて消す。
                write_file.close();
                std::cout << "問題を読み込みました" << std::endl;
            }else{
                cout << "問題ファイルが開けませんでした。";
            }

        }

        //リソース解放
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

    }

    


}