# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系

import heapq
from collections import defaultdict

# 定義霍夫曼編碼函數
def encode(frequency):
    # 建立一個minheap，並將每個字元的頻率和編碼存入heap中
    heap = [[weight, [symbol, ""]] for symbol, weight in frequency.items()]
    heapq.heapify(heap)
    while len(heap) > 1:
        # 從堆中取出兩個最小
        lo = heapq.heappop(heap)
        hi = heapq.heappop(heap)
        # 更新這兩個節點的編碼
        for pair in lo[1:]:
            pair[1] = '0' + pair[1]
        for pair in hi[1:]:
            pair[1] = '1' + pair[1]
        # 將這兩個節點合併後放回heap中
        heapq.heappush(heap, [lo[0] + hi[0]] + lo[1:] + hi[1:])
    # 從heap中取出最後一個，並將其編碼排序後返回
    return sorted(heapq.heappop(heap)[1:], key=lambda p: (len(p[-1]), p))

def decode(encoded_data, huff_codes):
    decoded_data = ""
    temp = ""
    reverse_huff = {v: k for k, v in huff_codes.items()}
    for d in encoded_data:
        temp += d
        # 如果當前的編碼存在於霍夫曼編碼表中，則將其解碼為對應的字元
        if temp in reverse_huff:
            decoded_data += reverse_huff[temp]
            temp = ""
    return decoded_data

# 主程式開始
inputs = []
while True:
    n = int(input())  # 字元數量
    if n == 0:  # 如果輸入為0，則結束輸入
        break
    data = []
    for _ in range(n):  # 根據字元數量，輸入每個字元和其頻率
        char, freq = input().split()
        freq = int(freq)
        data.append((char, freq))
    encoded_data = input()  # 輸入要解碼的二元碼
    inputs.append((data, encoded_data))  # 將輸入的資料存入list

# 處理所有的輸入
print("OUTPUT:\n")
for data, encoded_data in inputs:
    frequency = {char: freq for char, freq in data}  # 計算每個字元的頻率
    huff_codes = {char: code for char, code in encode(frequency)}  # 產生霍夫曼編碼
    decoded_data = decode(encoded_data, huff_codes)  # 解碼二元碼
    for char, code in sorted(huff_codes.items()):  # 按照字母順序輸出每個字元的霍夫曼碼
        print(f"{char} = {code}")
    print(f"Decode = {decoded_data}\n")  # 輸出解碼結果





