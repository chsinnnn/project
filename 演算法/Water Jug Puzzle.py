# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系
import time
def water_bucket_puzzle(capacity_a, capacity_b, target):
    state = (0, 0)  # 初始化兩個水桶的水量，起始狀態都為 0，表示兩個水桶都是空的
    visited = set()  # 創建一個空集合，記錄已經訪問過的狀態
    actions = []  # 初始化一個空列表，存每一步的操作動作
    
    # 進入迴圈直到水桶 B 中的水量等於目標水量
    while state[1] != target:
        # 如果當前狀態已經訪問過，則False（表示無解）
        if state in visited:
            return False
        visited.add(state)  # 將當前狀態加到已訪問的狀態中
        
        
        if state[0] == 0:  # 如果水桶 A 是空的
            actions.append("Fill A")  # 將水桶 A 倒滿水
            state = (capacity_a, state[1])  # 更新狀態，水桶 A 現在滿了
        elif state[1] == capacity_b:  # 如果水桶 B 是滿的
            actions.append("Empty B")  # 將水桶 B 倒空
            state = (state[0], 0)  # 更新狀態，水桶 B 現在是空的
        elif state[0] > 0 and state[1] < capacity_b:  # 如果水桶 A 還有水且水桶 B 還沒滿
            pour_amount = min(state[0], capacity_b - state[1])  # 計算可以從水桶 A 倒到水桶 B 的最大水量
            actions.append("Pour A B")  # 將水從水桶 A 倒到水桶 B
            state = (state[0] - pour_amount, state[1] + pour_amount)  # 更新狀態，水桶 A 減少相應水量，水桶 B 增加相應水量
        elif state[1] == 0:  # 如果水桶 B 是空的
            actions.append("Fill B")  # 將水桶 B 倒滿水
            state = (state[0], capacity_b)  # 更新狀態，水桶 B 現在滿了
        elif state[0] == capacity_a:  # 如果水桶 A 是滿的
            actions.append("Empty A")  # 將水桶 A 倒空
            state = (0, state[1])  # 更新狀態，水桶 A 現在是空的
        elif state[1] > 0 and state[0] < capacity_a:  # 如果水桶 B 還有水且水桶 A 還沒滿
            pour_amount = min(state[1], capacity_a - state[0])  # 計算可以從水桶 B 倒到水桶 A 的最大水量
            actions.append("Pour B A")  # 將水從水桶 B 倒到水桶 A
            state = (state[0] + pour_amount, state[1] - pour_amount)  # 更新狀態，水桶 B 減少相應水量，水桶 A 增加相應水量
    
    actions.append("Success\n")  # 成功達到目標水量
    return actions


if __name__ == "__main__":
    start_time = time.time()
    print("INPUT：每組有三個數字，第一個數字為水桶 A 的容量，第二個數字為水桶 B 的容量，第三個數字為目標容量。 0 0 0 結束")
    results = []
    while True:
        capacity_a, capacity_b, target = map(int, input().split())
        if capacity_a == 0 and capacity_b == 0 and target == 0:
            break
        actions = water_bucket_puzzle(capacity_a, capacity_b, target)
        results.append(actions)

    print("\nOUTPUT：列出達到目標的步驟")
    for actions in results:
        if actions:
            for action in actions:
                print(action)
        else:
            print("Impossible")
    total_time = time.time() - start_time
    print("TIME:", total_time)
