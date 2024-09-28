# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系

import time
# DisjointSet 類別用於實現不相交集合資料結構
class DisjointSet:
    # 初始化函數，設定每個元素的父節點為-1，表示各自為一個集合
    def __init__(self, size):
        self.parent = [-1] * size

    # 查找函數，用於查找元素所在的集合
    def find(self, i):
        if self.parent[i] == -1:
            return i
        return self.find(self.parent[i])

    # 合併函數，用於合併兩個集合
    def union(self, i, j):
        i_parent = self.find(i)
        j_parent = self.find(j)
        if i_parent != j_parent:
            self.parent[i_parent] = j_parent


# find_connected_components 函數用於找到連通元並計算其面積
def find_connected_components(image):
    rows = len(image)
    cols = len(image[0])
    ds = DisjointSet(rows * cols)

    # 定義內部函數，用於檢查像素的有效性
    def is_valid(row, col):
        return 0 <= row < rows and 0 <= col < cols

    # 使用深度優先搜索找到連通元並將其合併
    for i in range(rows):
        for j in range(cols):
            if image[i][j] == 1:
                index = i * cols + j
                neighbors = [
                    (i-1, j), (i+1, j), (i, j-1), (i, j+1),
                    (i-1, j-1), (i-1, j+1), (i+1, j-1), (i+1, j+1)
                ]
                for neighbor in neighbors:
                    ni, nj = neighbor
                    if is_valid(ni, nj) and image[ni][nj] == 1:
                        neighbor_index = ni * cols + nj
                        ds.union(index, neighbor_index)

    # 計算每個連通元的面積
    component_areas = {}
    for i in range(rows):
        for j in range(cols):
            if image[i][j] == 1:
                index = i * cols + j
                parent_index = ds.find(index)
                if parent_index not in component_areas:
                    component_areas[parent_index] = []
                component_areas[parent_index].append((i, j))

    areas = sorted(component_areas.values(), key=len)
    unique_areas = len(areas)

    return unique_areas, areas

# main 函數用於控制程式流程，輸入影像並呼叫函數進行處理
# main 函數用於控制程式流程，輸入影像並呼叫函數進行處理
def main():
    start_time = time.time()
    image_count = 0
    while True:
        size = input().split()
        if size[0] == '0' and size[1] == '0':
            break
        rows = int(size[0])
        cols = int(size[1])
        image_count += 1
        image = []
        for _ in range(rows):
            row = list(map(int, input().strip()))
            image.append(row)
        unique_areas, areas = find_connected_components(image)
        print(f"Image #{image_count}")
        if unique_areas == 0:
            print("Number of Connected Components = 0")
            print("Connected Component #1 Area = 0")
        else:
            print(f"Number of Connected Components = {unique_areas}")
            for i, area in enumerate(areas, start=1):
                print(f"Connected Component #{i} Area = {len(area)}")
            
    total_time = time.time() - start_time
    print("\nTIME:", total_time)

if __name__ == "__main__":
    main()
