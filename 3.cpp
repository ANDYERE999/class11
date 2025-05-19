//
// Created by 666 on 25-5-18.
//
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
using namespace std;

const int MAXV = 100;
const int INF = 0x3f3f3f3f;

class MatGraph{
    public:
        vector<vector<int>> data;
        int nodesCount;
        int linesCount;

        MatGraph(int nodesCount, int linesCount){
            this->linesCount=linesCount;
            this->nodesCount=nodesCount;
            // 初始化矩阵大小为nodesCount x nodesCount
            data.resize(nodesCount, vector<int>(nodesCount, -1));
        }
        void setUp(const vector<string> data){
            for (int i=0;i<data.size();i++){
                stringstream ss(data[i]);
                vector<int> tempLine;
                string myStr;
                while(ss>>myStr){
                    tempLine.push_back(stoi(myStr)==0?-1:stoi(myStr));
                }
                this->data[i] = tempLine;
            }
        }
        void modify(string s,int indexToModify){
            stringstream ss(s);
            int m;  // 联系人数量
            ss >> m;
            
            for(int i = 0; i < m; i++) {
                int target, distance;
                ss >> target >> distance;
                target--;  // 转换为0-based索引
                this->data[indexToModify][target] = distance;
            }
        }
};
/*
void dumpstack(int currentPoint,int distanceAccumulated,vector<int> dist,vector<int> path,vector<bool> nodesHaveReached,int startpoint,int endpoint){
    if (currentPoint=startpoint){
        cout<<distanceAccumulated;
        return;
    }
    if (distanceAccumulated>=INF){
        cout<<-1;
        return;
    }
    dumpstack(path[currentPoint],distanceAccumulated+)

    "如果只需要知道最短距离，不需要知道路径，不需要这个递归"
}
*/
int outputOfDijkstra(vector<int> dist,vector<int> path,vector<bool> nodesHaveReached,int startpoint){// 这个输出函数后面可以根据需求更改
    int longestDistance=0;
    bool hasUnreachable=false;
    for (int i=0;i<dist.size();i++){
        if (dist[i] >= INF){
            hasUnreachable = true;
            break;
        }
        if (dist[i] > longestDistance){
            longestDistance = dist[i];
        }
    }
    
    return hasUnreachable ? INF : longestDistance;
}

int Dijkstra(const MatGraph &graph,int startpoint){
    vector<int> dist(graph.nodesCount, INF);
    vector<int> path(graph.nodesCount, -1);
    vector<bool> nodesHaveReached(graph.nodesCount, false);
    
    // 初始化
    dist[startpoint] = 0;
    
    for (int i=0;i<graph.nodesCount;i++){
        // 找到未被访问的dist中最小的值
        int u = -1;
        int minDist = INF;
        for (int j=0;j<graph.nodesCount;j++){
            if (!nodesHaveReached[j] && dist[j] < minDist){
                minDist = dist[j];
                u = j;
            }
        }
        
        if (u == -1) break;  // 没有可达节点
        nodesHaveReached[u] = true;
        
        // 更新相邻节点的距离
        for (int v=0;v<graph.nodesCount;v++){
            if (!nodesHaveReached[v] && graph.data[u][v] != -1){
                int newDist = dist[u] + graph.data[u][v];
                if (newDist < dist[v]){
                    dist[v] = newDist;
                    path[v] = u;
                }
            }
        }
    }
    
    return outputOfDijkstra(dist,path,nodesHaveReached,startpoint);
}

int main(){
    freopen("in.txt","r",stdin);
    string myStr;
    getline(cin,myStr);
    int x=stoi(myStr);
    MatGraph myGraph(x,114514);//这里和边的条数无关，可以乱填
    for (int i=0;i<x;i++){
        getline(cin,myStr);
        if(myStr.empty()) {
            break;
        }
        myGraph.modify(myStr,i);
    }
    
    int shortest=INF;
    int shortestStartPoint=-1;
    
    for (int i=0;i<x;i++){
        int tempVal=Dijkstra(myGraph,i);//加速
        if (shortest>tempVal){
            shortestStartPoint=i;
            shortest=tempVal;
        }
    }
    
    if (shortest==INF){
        cout<<"disjoint";
    }else{
        cout<<shortestStartPoint+1<<" "<<shortest;
    }
    
    return 0;
}
