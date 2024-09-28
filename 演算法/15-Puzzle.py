# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系

from queue import PriorityQueue
import time
def heuristic(puzzle, target):
    # 計算曼哈頓距離
    #abs(b%4 - g%4) + abs(b//4 - g//4)計算的是數字 i 在 puzzle 和 target 中的位置之間的曼哈頓距離。
    #曼哈頓距離是由兩點在標準坐標系上的絕對軸距之總和。
    return sum(abs(b%4 - g%4) + abs(b//4 - g//4)  
               for b, g in ((puzzle.index(i), target.index(i)) for i in range(1, 16)))
                #它會找到 i 在當前拼圖 puzzle 和目標拼圖 target 中的位置

def solve(puzzle, target):  # 定義解決拼圖的函數
    queue = PriorityQueue()  # 建立一個優先佇列
    queue.put((0, tuple(puzzle), []))  # 將拼圖放入佇列
    visited = set()  # 建立一個集合來存放已訪問的節點
    while puzzle != target:  # 當拼圖不等於目標狀態時
        (cost, puzzle, path) = queue.get()  # 從佇列中取出一個節點
        if puzzle in visited:  # 如果該節點已被訪問過
            continue  # 繼續下一次迴圈
        visited.add(puzzle)  # 將該節點加入已訪問的集合
        blank = puzzle.index(0)  # 找出空格的位置
        for d, move in ((-1, 'R'), (1, 'L'), (-4, 'D'), (4, 'U')):  # 定義移動的方向
            neighbor = blank + d  # 計算移動後的位置
            if -1 < neighbor < 16 and not (blank % 4 == 3 and d == 1) and not (blank % 4 == 0 and d == -1):  # 如果移動是合法的
                new_puzzle = list(puzzle)  # 建立一個新的拼圖
                new_puzzle[blank], new_puzzle[neighbor] = new_puzzle[neighbor], new_puzzle[blank]  # 進行移動
                queue.put((cost + 1 + heuristic(new_puzzle, target), tuple(new_puzzle), path + [(new_puzzle[blank], move)]))  # 將新的拼圖放入佇列
    return cost, path

if __name__ == "__main__":
    start_time = time.time()
    target = [i for i in range(1, 16)] + [0]
    num_puzzles = int(input("請輸入拼圖的數量: "))
    puzzles = []
    for i in range(1, num_puzzles + 1):
        print(f"\n請輸入第{i}個拼圖:")
        puzzle = []
        for _ in range(4):
            while True:
                row = list(map(int, input().split()))
                if len(row) == 4:
                    break
            puzzle.extend(row)
        puzzles.append(puzzle)
    for i, puzzle in enumerate(puzzles, 1):
        moves, path = solve(tuple(puzzle), tuple(target))
        print(f"\n15-Puzzle #{i}")
        print("Number of moves =", len(path))
        for j in range(0, len(path), 5):
            print(' '.join(f"{num:2}{direction}" for num, direction in path[j:j+5]))
    
    total_time = time.time() - start_time
    print("\nTIME:", total_time)