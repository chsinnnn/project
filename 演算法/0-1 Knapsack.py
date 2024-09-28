# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系

def knapsack(W, wt, val, n):
    K = [[0 for w in range(W+1)] for i in range(n+1)]
    for i in range(n+1):
        for w in range(W+1):
            if i == 0 or w == 0:
                K[i][w] = 0
            elif wt[i-1] <= w:
                K[i][w] = max(val[i-1] + K[i-1][w-wt[i-1]],  K[i-1][w])
            else:
                K[i][w] = K[i-1][w]

    # 儲存最大Value
    res = K[n][W]
    print("Total Value =", res)
    
    w = W
    picked_items = []
    for i in range(n, 0, -1):
        if res <= 0:
            break
        # 當前物品被選中
        if res == K[i-1][w]:
            continue
        else:
            # 將選中的物品加入列表
            picked_items.append(i)
            res -= val[i-1]
            w -= wt[i-1]
    print("Items =", ', '.join(map(str, sorted(picked_items))))

# 測試
inputs = []
while True:
    W = int(input())  # 背包的重量限制
    if W == 0:  # 如果輸入為0，則結束輸入
        break
    n = int(input())  # 物品數量
    val = []
    wt = []
    for _ in range(n):
        w, v = map(int, input().split())
        val.append(v)
        wt.append(w)
    inputs.append((W, wt, val, n))

for W, wt, val, n in inputs:
    knapsack(W, wt, val, n)
