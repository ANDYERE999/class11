#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int N, int K) {
        // 创建邻接表表示图
        vector<vector<pair<int, int>>> graph(N + 1); // 节点编号从1开始
        for (const auto& time : times) {
            int u = time[0]; // 源节点
            int v = time[1]; // 目标节点
            int w = time[2]; // 传输时间
            graph[u].push_back({v, w});
        }
        
        // 使用Dijkstra算法找最短路径
        vector<int> dist(N + 1, INT_MAX); // 存储从K到各节点的最短距离
        dist[K] = 0; // 起点到自己的距离为0
        
        // 优先队列，按距离排序，存储{距离, 节点}对
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, K});
        
        while (!pq.empty()) {
            int d = pq.top().first; // 当前距离
            int node = pq.top().second; // 当前节点
            pq.pop();
            
            if (d > dist[node]) continue; // 已有更短路径，跳过
            
            // 遍历当前节点的所有邻居
            for (auto& neighbor : graph[node]) {
                int next_node = neighbor.first;
                int weight = neighbor.second;
                
                // 如果找到更短路径，更新距离并加入队列
                if (dist[node] + weight < dist[next_node]) {
                    dist[next_node] = dist[node] + weight;
                    pq.push({dist[next_node], next_node});
                }
            }
        }
        
        // 找出最长的最短路径
        int max_dist = 0;
        for (int i = 1; i <= N; i++) {
            if (dist[i] == INT_MAX) return -1; // 有节点无法到达
            max_dist = max(max_dist, dist[i]);
        }
        
        return max_dist;
    }
};

int main() {
    int N, K;
    cin >> N >> K;
    
    vector<vector<int>> times;
    int u, v, w;
    while (cin >> u >> v >> w) {
        times.push_back({u, v, w});
    }
    
    Solution solution;
    int result = solution.networkDelayTime(times, N, K);
    
    cout << result << endl;
    
    return 0;
}
