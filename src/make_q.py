# 画像を読み込む
import cv2
import numpy as np
import pathlib
import sys
import os
import random

#ファイル名入力
jpeg_name = input('JPEGファイル名:')

#画像ファイル読み込み
if not os.path.isfile(jpeg_name):
    print(jpeg_name, 'がありません')
    sys.exit()
img_rgb = cv2.imread(jpeg_name)

#分割数入力
x_div = int(input('横分割数:'))
y_div = int(input('縦分割数:'))

#グレースケール化
img_gray = cv2.cvtColor(img_rgb, cv2.COLOR_BGR2GRAY)

h = img_gray.shape[0]
w = img_rgb.shape[1]

#画像をモザイク化
img_small = cv2.resize(img_gray, (x_div, y_div)) #sizeと同じ値に画像を縮小
img_mos = cv2.resize(img_small, (w, h), interpolation=cv2.INTER_AREA) #元のサイズに拡大

#画像をランダムにする
img_rnd_small = np.copy(img_small)
random.seed()
for i in range(x_div * y_div * 10):
    x1 = random.randrange(0, x_div)
    x2 = random.randrange(0, x_div)
    y1 = random.randrange(0, y_div)
    y2 = random.randrange(0, y_div)
    img_rnd_small[y1][x1], img_rnd_small[y2][x2] = img_rnd_small[y2][x2], img_rnd_small[y1][x1]
img_rnd = cv2.resize(img_rnd_small, (w, h), interpolation=cv2.INTER_AREA) #元のサイズに拡大

#出力ファイル名生成
json_name = pathlib.PurePath(jpeg_name).stem + '.json'
 
#jsonファイル出力       
file = open(json_name, 'w')

file.write('{\n')  
file.write('    "board": {\n')
file.write('        "width": ')
file.write(str(x_div))
file.write(',\n')
file.write('        "height": ')
file.write(str(y_div))
file.write(',\n')

file.write('        "start": [\n')
for y in range(y_div):
    file.write('            "')
    for x in range(x_div):
        file.write(str(int(img_rnd_small[y][x] / 64)))
        #file.write('x')
    file.write('"')
    if y < y_div - 1:
        file.write(',')
    file.write('\n')

file.write('        ],\n')


file.write('        "goal": [\n')
for y in range(y_div):
    file.write('            "')
    for x in range(x_div):
        file.write(str(int(img_small[y][x] / 64)))
        #print(int(img_small[y][x] / 64), ' ', end='')
    file.write('"')
    if y < y_div - 1:
        file.write(',')
    file.write('\n')
    #print('')

file.write('        ]\n')
file.write('    }\n')
file.write('}')

file.close()

print('Enterキーを押してください')

# 画像をウィンドウで表示
img_disp = cv2.hconcat([img_mos, img_rnd])
img_disp = cv2.resize(img_disp, None, fx=0.8, fy=0.8)
cv2.imshow('Image', img_disp)

# キーボード入力を待つ
cv2.waitKey(0)

# ウィンドウを閉じる
cv2.destroyAllWindows()