# 演算法分析機測
# 學號:11027104/11027133/11027149
# 姓名:侯如蓁/李若菱/游婕歆
# 中原大學資訊工程系

import heapq

def dijkstra(n, edges, start):
    # 建立圖的鄰接表
    graph = {i: {} for i in range(1, n+1)}
    for u, v, w in edges:
        if v in graph[u]:
            graph[u][v] = min(graph[u][v], w)
        else:
            graph[u][v] = w

        if u in graph[v]:
            graph[v][u] = min(graph[v][u], w)
        else:
            graph[v][u] = w

    # Dijkstra algorithm
    min_heap = [(0, start)]  # (distance, vertex)
    distances = {i: float('inf') for i in range(1, n+1)}
    distances[start] = 0
    visited = set()

    while min_heap:
        current_distance, u = heapq.heappop(min_heap)

        if u in visited:
            continue

        visited.add(u)

        for v in graph[u]:
            distance = current_distance + graph[u][v]
            if distance < distances[v]:
                distances[v] = distance
                heapq.heappush(min_heap, (distance, v))

    # 組合輸出結果
    result = []
    for vertex in range(1, n+1):
        if vertex != start:
            result.append(f"{start} to {vertex} = {distances[vertex]}")
    return result

def main():
    print("請依序輸入頂點個數、邊的個數、每條邊和其權重:")
    inputs = []
    while True:
        line = input()
        if line == "0":
            break
        inputs.append(line)

    n = int(inputs[0])
    edges_count = int(inputs[1])
    start = int(inputs[2])
    edges = []

    for i in range(3, 3 + edges_count):
        u, v, w = map(int, inputs[i].split())
        edges.append((u, v, w))

    # 計算最短路徑並輸出結果
    result = dijkstra(n, edges, start)
    for line in result:
        print(line)

if __name__ == "__main__":
    main()
