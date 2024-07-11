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