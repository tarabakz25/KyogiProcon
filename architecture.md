:::mermaid
graph TD;
title[競技プロコンアーキテクチャ];
a((start)) --> b[時間の計測を開始]
b --> c[Json読み込み]
c --> d[盤面をそれぞれstartB, goalBに格納]
d --> e[matchRateを計算]
e --> f[matchRateが100になるまでソートアルゴリズムを繰り返す]
f --> g[ソートアルゴリズムを実行]
g --> h[ソートアルゴリズムの結果を出力]
h --> i[時間の計測を終了]
i --> j[結果を出力]
j --> k((end))

A[[ソートアルゴリズム]]

B((start)) --> C{startBとgoalBの指定地点が一致するか}

C --> |Yes| D[次の指定地点に移動]
D --> E{指定地点が最終か}
E --> |Yes| F((end))
E --> |No| C

C --> |No| G[指定地点と同様の数字を探索]

G --> H[指定地点と同様の数字をstartBとgoalBで探索]

H --> I[縦横それぞれの移動回数をiCount, jCountに格納]
I --> J[katanuki関数を呼び出す]
J --> K[katanuki関数の結果を出力]
K --> L[次の指定地点に移動]
L --> E
