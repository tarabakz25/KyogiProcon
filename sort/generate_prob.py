import json
import random
from collections import Counter

# ボードの初期状態をランダム生成する関数（0〜3の数が指定通り）
def generate_random_board_with_counts(width, height, counts):
    board = []
    total_cells = width * height
    values = []

    # countsに基づいて0〜3の値をリストに追加
    for value, count in counts.items():
        values.extend([value] * count)

    # リストをシャッフルしてランダム配置
    random.shuffle(values)

    # ボードに変換
    for i in range(height):
        row = ''.join(map(str, values[i * width:(i + 1) * width]))
        board.append(row)

    return board

# 比較的「綺麗」な最終状態のボードを生成する関数
def generate_goal_board_with_counts(width, height, counts):
    board = []
    values = []

    # countsに基づいて0〜3の値をリストに追加
    for value, count in counts.items():
        values.extend([value] * count)

    # ある程度規則的に並べる（ここではブロックごとに）
    block_size = width // len(counts)
    for i in range(len(counts)):
        row_values = values[i * block_size * height:(i + 1) * block_size * height]
        for j in range(0, len(row_values), width):
            board.append(''.join(map(str, row_values[j:j + width])))

    return board

# 0〜3の各値の個数を計算する関数
def calculate_value_counts(width, height):
    total_cells = width * height
    base_count = total_cells // 4
    counts = {i: base_count for i in range(4)}

    # 余りがあれば均等に追加
    remainder = total_cells % 4
    for i in range(remainder):
        counts[i] += 1

    return counts

# 一般抜き型のランダム生成関数
def generate_random_patterns(num_patterns, max_width, max_height):
    patterns = []
    for i in range(num_patterns):
        width = random.randint(1, min(max_width, 256))  # 抜き型の幅をランダムに決定
        height = random.randint(1, min(max_height, 256))  # 抜き型の高さをランダムに決定
        cells = [''.join(str(random.randint(0, 1)) for _ in range(width)) for _ in range(height)]
        patterns.append({
            "p": i + 25,  # 一般抜き型は25番から始まる
            "width": width,
            "height": height,
            "cells": cells
        })
    return patterns

# JSON形式の問題データを生成する関数
def create_problem_json(board_width, board_height, start_board, goal_board, patterns):
    problem = {
        "board": {
            "width": board_width,
            "height": board_height,
            "start": start_board,
            "goal": goal_board
        },
        "general": {
            "n": len(patterns),
            "patterns": patterns
        }
    }
    return json.dumps(problem, indent=2)

# メイン関数
def main():
    # ユーザーが指定するボードのサイズ（1以上に制限）
    board_width = int(input("ボードの幅（1〜256）を入力してください: "))
    board_height = int(input("ボードの高さ（1〜256）を入力してください: "))

    # 0〜3の各値の数を計算
    counts = calculate_value_counts(board_width, board_height)

    # ボードの初期状態と比較的綺麗な最終状態を生成（0〜3の数は同じ）
    start_board = generate_random_board_with_counts(board_width, board_height, counts)
    goal_board = generate_goal_board_with_counts(board_width, board_height, counts)

    # ランダムに一般抜き型を生成（例として5個）
    num_patterns = 5
    patterns = generate_random_patterns(num_patterns, board_width, board_height)

    # JSONデータ作成
    problem_json = create_problem_json(board_width, board_height, start_board, goal_board, patterns)
    
    # ファイル名を指定して保存（例: "problem_XxY.json" 形式で保存）
    file_name = f"problem_{board_width}x{board_height}.json"
    with open(file_name, "w") as f:
        f.write(problem_json)
    
    print(f"ランダム生成されたJSONファイルが作成されました: {file_name}")

if __name__ == "__main__":
    main()
