#include <bits/stdc++.h>
#include <iostream>
#include <curl/curl.h>

#include <string> //read&write
#include <fstream>
#include <sstream>
#include <filesystem>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json; //順番を維持する（通常アルファベット順になるらしい）

void json_path_setting(){
    filesystem::current_path("../"); //カレントディレクトリをソースファイルに変更。
    cout << "(設定)Current path: " << filesystem::current_path().c_str() << endl; //カレントディレクトリを表示;
}


//コールバック関数
size_t Callback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t totalSize = size * nmemb;
    s->append((char*)contents, totalSize);
    return totalSize;
}


void receive_problem(string token){
    CURL* curl;
    CURLcode res; //リクエスト結果
    string readBuffer; //サーバーからのデータ保存

    curl = curl_easy_init();  //libcurlのイニシャライズ(初期化)
    if(curl){ //成功した、返されたら
        //URL設定
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/problem");

        //HTTPヘッダ設定
        struct curl_slist* headers = NULL;
        string tokenheader = "Procon-Token: " + token;
        headers = curl_slist_append(headers, tokenheader.c_str()); //ヘッダーリストにトークン追加
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // HTTPヘッダーを設定

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback); //レスポンスを受け取ったら、Callback()が呼ばれる。
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); //書き込まれる場所を指定

        res = curl_easy_perform(curl); //リクエスト実行
        
        if(res != CURLE_OK){ //リクエスト失敗？
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }else{
            //cout << "Response: " << readBuffer << endl;

            //アクセスタイムエラー（サーバが動いていない場合）なら飛ばす。
            if (readBuffer.find("AccessTimeError") != std::string::npos) {
                cout << "(状態) サーバーが動いていません。" << endl;
                return;
            } 

            filesystem::current_path("./"); //カレントディレクトリをソースファイルに変更。
            cout << "(設定)Current path: " << filesystem::current_path().c_str() << endl; //カレントディレクトリを表示;

            //jsonファイルとして書き込む
            ordered_json problem = ordered_json::parse(readBuffer);

            ofstream write_file("./problem.json");
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

void send_problem(string token){
    CURL* curl;
    CURLcode res; //リクエスト結果
    string readBuffer; //サーバーからのデータ保存

    ifstream sendfile("./answer.json");
    if (!sendfile.is_open()) {
        cerr << "送信用のファイルが開けません。" << endl;
        return;
    }

    json senddata;
    sendfile >> senddata; //開いたファイルをjson化する。
    sendfile.close();

    string json_senddata = senddata.dump(); //jsonを文字列化する。


    curl = curl_easy_init();  //libcurlのイニシャライズ(初期化)
    if(curl){ //成功した、返されたら
        //URL設定
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/answer");

        //HTTPヘッダ設定
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        string tokenheader = "Procon-Token: " + token;
        headers = curl_slist_append(headers, tokenheader.c_str()); //ヘッダーリストにトークン追加
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // HTTPヘッダーを設定

        // POSTリクエスト設定
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_senddata.c_str());

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
        }

        //リソース解放
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

    }
}

/*
        これで送信できます。
            send_problem("token1");
*/