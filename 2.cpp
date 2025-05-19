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
            data.resize(nodesCount, vector<int>(nodesCount, 0));
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


}
*/
void outputOfDijkstra(vector<int> dist,vector<int> path,vector<bool> nodesHaveReached,int startpoint,int endpoint){// 这个输出函数后面可以根据需求更改
    if (dist[endpoint]>=INF){
        cout<<-1;
    }else{
        cout<<dist[endpoint];
    }
}

void Dijkstra(const MatGraph &graph,int startpoint,int endpoint){// 传参的时候记得减去1
    vector<int> dist(graph.nodesCount);
    vector<int> path(graph.nodesCount);
    vector<bool> nodesHaveReached(graph.nodesCount,false);
    // 初始化
    nodesHaveReached[startpoint]=true;
    for (int i=0;i<dist.size();i++){
        if (i==startpoint){
            dist[i]=0;
            path[i]=-1;
        }else{
            if(graph.data[startpoint][i]==-1){
                dist[i]=INF;
                path[i]=-1;
            }else{
                dist[i]=graph.data[startpoint][i];
                path[i]=startpoint;
            }
        }
    }
    // 正式开始算法
    for (int i=0;i<graph.nodesCount;i++){
        // 找到未被访问的dist中最小的值作为基本路径
        int shortestBaseDistance=INF;
        int indexOfShortestBaseDistance=-1;
        for (int j=0;j<dist.size();j++){
            if (!nodesHaveReached[j] && dist[j]<shortestBaseDistance){
                shortestBaseDistance=dist[j];
                indexOfShortestBaseDistance=j;
            }
        }
        
        if (indexOfShortestBaseDistance==-1) break;  // 如果没有找到可达的节点，退出循环
        
        nodesHaveReached[indexOfShortestBaseDistance]=true;
        // 更新dist中与找到的base点毗邻且距离减少的节点，同时更新path
        for (int j=0;j<graph.nodesCount;j++){
            if (!nodesHaveReached[j] && 
                graph.data[indexOfShortestBaseDistance][j]!=-1 && 
                shortestBaseDistance!=INF && 
                dist[indexOfShortestBaseDistance] + graph.data[indexOfShortestBaseDistance][j] < dist[j]){
                dist[j] = dist[indexOfShortestBaseDistance] + graph.data[indexOfShortestBaseDistance][j];
                path[j] = indexOfShortestBaseDistance;
            }
        }
    }

    outputOfDijkstra(dist,path,nodesHaveReached,startpoint,endpoint);
    
}

int main(){
    int x, y;
    cin >> x >> y;
    MatGraph myGraph(x, y);
    vector<string> data;
    // 直接用cin读取邻接矩阵
    for (int i = 0; i < x; i++) {
        string row;
        for (int j = 0; j < x; j++) {
            int val;
            cin >> val;
            row += to_string(val);
            if (j != x - 1) row += " ";
        }
        data.push_back(row);
    }
    int startpoint, endpoint;
    cin >> startpoint >> endpoint;
    myGraph.setUp(data);
    Dijkstra(myGraph, startpoint - 1, endpoint - 1);
    return 0;
}
