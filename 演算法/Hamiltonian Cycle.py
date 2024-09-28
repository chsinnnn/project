# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系
import time

def hamiltonian_cycle(graph, start, current, visited, path):
    # 將當前節點加入路徑中
    path.append(current + 1) # current=0所以從1開始
    
    # 如果已經訪問了所有頂點，且可以回到起點，則找到了漢密爾頓迴圈
    if len(path) == len(graph) and graph[current][start]:
        path.append(1) #再回到起點
        return True
    
    # 檢查與當前頂點相鄰的所有頂點
    for v in range(len(graph)):
        if graph[current][v] and not visited[v]:
            # 標記為已訪問
            visited[v] = True
            # 遞迴尋找下一個頂點
            if hamiltonian_cycle(graph, start, v, visited, path):
                return True
            # 如果沒有找到合適的路徑，則取消這個頂點
            visited[v] = False
            path.pop()
    
    return False

def main():
    start_time = time.time()
    while True:
        print("Input: ")
        vertices, edges = map(int, input().split())
        
        if vertices == 0 and edges == 0:
            break
        
        # 初始化圖形的鄰接矩陣
        graph = [[False] * vertices for _ in range(vertices)]
        
        # 建立鄰接矩陣
        for _ in range(edges):
            u, v = map(int, input().split())
            graph[u - 1][v - 1] = True
            graph[v - 1][u - 1] = True
        
        # 初始化訪問狀態，全部設為未訪問
        visited = [False] * vertices
        
        # 將起點設為已訪問
        visited[0] = True
        
        # 儲存路徑的列表
        path = []
        
        # 解決問題
        result = hamiltonian_cycle(graph, 0, 0, visited, path)
        print("Output: ")
        if result:
            print(" ".join(map(str, path))+"\n")
        else:
            print("No Hamiltonian cycle exists\n")
            
    
    total_time = time.time() - start_time
    print("\nTIME:", total_time)

if __name__ == "__main__":
    main()
