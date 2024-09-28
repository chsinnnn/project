# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系

import numpy as np
import cv2
from collections import deque

# 讀取輸入影像
file_name = input("Enter file name: ")
input_image = cv2.imread(file_name)

# 確保影像大小為 500x500 像素
input_image = cv2.resize(input_image, (500, 500))

# 將影像轉換為灰度影像
gray_image = cv2.cvtColor(input_image, cv2.COLOR_BGR2GRAY)

# 將灰度影像轉換為二值影像
_, binary_image = cv2.threshold(gray_image, 127, 255, cv2.THRESH_BINARY)

# 定義每個小方格的大小
cell_size = 20

# 創建迷宮的網格表示，大小為 25x25
maze = np.zeros((25, 25), dtype=np.uint8)

# 根據灰度影像填充迷宮網格
for i in range(25):
    for j in range(25):
        cell = 0
        # 檢查每個小方格的四個邊
        if i > 0 and np.all(binary_image[i*cell_size, j*cell_size:(j+1)*cell_size] == 255):
            cell |= 1  # 上方有白色邊緣，不能向上移動
        if i < 24 and np.all(binary_image[(i+1)*cell_size-1, j*cell_size:(j+1)*cell_size] == 255):
            cell |= 2  # 下方有白色邊緣，不能向下移動
        if j > 0 and np.all(binary_image[i*cell_size:(i+1)*cell_size, j*cell_size] == 255):
            cell |= 4  # 左方有白色邊緣，不能向左移動
        if j < 24 and np.all(binary_image[i*cell_size:(i+1)*cell_size, (j+1)*cell_size-1] == 255):
            cell |= 8  # 右方有白色邊緣，不能向右移動
        maze[i, j] = cell

# 定義起點和終點
start = (0, 0)
end = (24, 24)

# 使用 BFS 尋找最短路徑
def bfs(maze, start, end):
    rows, cols = maze.shape
    queue = deque([(start, [start])])
    visited = set()
    visited.add(start)
    
    directions = [((0, 1), 8, 4), ((1, 0), 2, 1), ((0, -1), 4, 8), ((-1, 0), 1, 2)]
    
    while queue:
        (current, path) = queue.popleft()
        if current == end:
            return path
        
        x, y = current
        for direction, bit, rev_bit in directions:
            next_cell = (x + direction[0], y + direction[1])
            if 0 <= next_cell[0] < rows and 0 <= next_cell[1] < cols and next_cell not in visited:
                if not (maze[x, y] & bit or maze[next_cell[0], next_cell[1]] & rev_bit):  # 判斷是否可以移動到下一個方格
                    queue.append((next_cell, path + [next_cell]))
                    visited.add(next_cell)
    return None

# 找到最短路徑
path = bfs(maze, start, end)

# 在原始影像上繪製路徑
output_image = input_image.copy()
if path:
    for i in range(1, len(path)):
        cv2.line(output_image, (path[i-1][1] * cell_size + cell_size // 2, path[i-1][0] * cell_size + cell_size // 2), 
                 (path[i][1] * cell_size + cell_size // 2, path[i][0] * cell_size + cell_size // 2), (255, 0, 0), 2)
    print("Path found")
else:
    print("No path found")

# 保存輸出影像
output_file_name = file_name.replace(".bmp", "sol.bmp")
cv2.imwrite(output_file_name, output_image)

print(f"Output saved to {output_file_name}")
