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

file.write('"board": {\n')
file.write('"width": ')
file.write(str(x_div))
file.write(',\n')
file.write('"hight": ')
file.write(str(y_div))
file.write(',\n')

file.write('"start": [\n')
for y in range(y_div):
    file.write('"')
    for x in range(x_div):
        file.write(str(int(img_rnd_small[y][x]  / 64)))
        #file.write('x')
    file.write('"')
    if y < y_div - 1:
        file.write(',')
    file.write('\n')

file.write('],\n')


file.write('"goal": [\n')
for y in range(y_div):
    file.write('"')
    for x in range(x_div):
        file.write(str(int(img_small[y][x] / 64)))
        #print(int(img_small[y][x] / 64), ' ', end='')
    file.write('"')
    if y < y_div - 1:
        file.write(',')
    file.write('\n')
    #print('')

file.write(']\n')
file.write('},\n')

file.close()

print('Enterキーを押してください')

#以下は表示のための処理

#4値化
img_small_4value = np.copy(img_small)
img_rnd_4value = np.copy(img_rnd_small)
for y in range(y_div):
    for x in range(x_div):
        img_small_4value[y][x] = int(img_small[y][x] / 64) * 64 
        img_rnd_4value[y][x] = int(img_rnd_small[y][x] / 64) * 64 
img_small_4value = cv2.resize(img_small_4value, (w, h), interpolation=cv2.INTER_AREA)
img_rnd_4value = cv2.resize(img_rnd_4value, (w, h), interpolation=cv2.INTER_AREA)

# グレー画像とモザイク画像を横に並べる
img_gray_rgb = cv2.cvtColor(img_gray, cv2.COLOR_GRAY2BGR)
cv2.putText(img_gray_rgb, 'grayscale', (20, 50), cv2.FONT_HERSHEY_DUPLEX, 1, (255, 255, 0), 2)
img_mos_rgb = cv2.cvtColor(img_mos, cv2.COLOR_GRAY2BGR)
cv2.putText(img_mos_rgb, 'mosaic', (20, 50), cv2.FONT_HERSHEY_DUPLEX, 1, (255, 255, 0), 2)
img_disp1 = cv2.hconcat([img_gray_rgb, img_mos_rgb])

# ４値化したモザイクとランダム画像を横に並べる
img_small_4value = cv2.cvtColor(img_small_4value, cv2.COLOR_GRAY2BGR)
cv2.putText(img_small_4value, '4-value', (20, 50), cv2.FONT_HERSHEY_DUPLEX, 1, (255, 255, 0), 2)
img_rnd_4value = cv2.cvtColor(img_rnd_4value, cv2.COLOR_GRAY2BGR)
cv2.putText(img_rnd_4value, 'randomize', (20, 50), cv2.FONT_HERSHEY_DUPLEX, 1, (255, 255, 0), 2)
img_disp2 = cv2.hconcat([img_small_4value, img_rnd_4value])

#縦に並べて表示
img_disp = cv2.vconcat([img_disp1, img_disp2])
img_disp = cv2.resize(img_disp, None, fx=0.5, fy=0.5)
cv2.imshow('Image', img_disp)

# キーボード入力を待つ
cv2.waitKey(0)

# ウィンドウを閉じる
cv2.destroyAllWindows()
