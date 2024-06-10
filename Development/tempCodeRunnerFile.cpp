// JSONファイルの読み込み
    ifstream ifs("./sample2.json");
    string str((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    json jobj = json::parse(str);
    vector<vector<int>> startBoard, goalBoard;
    loadBoard(jobj, startBoard, goalBoard);
    int boardSide = jobj["board"]["width"].get<int>();
    int boardWarp = jobj["board"]["height"].get<int>();

	board = startBoard;