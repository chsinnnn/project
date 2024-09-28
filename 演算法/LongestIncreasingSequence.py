# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系

def find_lis(arr):
    if not arr:
        return 0, []

    n = len(arr)
    dp = [1] * n  # dp[i] 表示以 arr[i] 結尾的LIS的長度
    predecessor = [-1] * n  # 記錄前一個元素的索引

    for i in range(1, n):
        for j in range(i):
            if arr[i] > arr[j] and dp[i] < dp[j] + 1:
                dp[i] = dp[j] + 1
                predecessor[i] = j

    # 找到最大的LIS長度和對應的結尾索引
    max_length = max(dp)
    lis_end_index = dp.index(max_length)

    # 重建LIS序列
    lis_sequence = []
    k = lis_end_index
    while k != -1:
        lis_sequence.append(arr[k])
        k = predecessor[k]

    lis_sequence.reverse()
    return max_length, lis_sequence

def process_input(input_data):
    for line in input_data.strip().split('\n'):
        if line.strip() == '0':
            break
        arr = list(map(int, line.strip().split()))
        length, lis = find_lis(arr)
        print(f"Length of LIS = {length}")
        print(f"LIS = <{', '.join(map(str, lis))}>")
        print()

# 讀取使用者輸入
input_data = ""
print("請輸入陣列（每組資料換行，輸入0結束）：")
while True:
    line = input()
    if line.strip() == '0':
        input_data += "0\n"
        break
    input_data += line + "\n"

# 處理並輸出結果
process_input(input_data)






