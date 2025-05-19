#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class Solution{
    public:
        int networkDelayTime(vector<vector<int>> &times,int N,int K){
            // 验证N和K
            if (N <= 0 || K <= 0 || K > N) {
                return -1; // 无效的N或K，无法继续
            }

            class Link{
                public:
                int target;
                int distance;
                int diatanceHaveReached;
                Link(int target,int distance){
                    this->target=target;
                    this->distance=distance;
                    this->diatanceHaveReached=0;
                }
                void updateByGameTick(int &index){
                    if (diatanceHaveReached>=distance){
                        index=this->target;
                    }else{
                        index=-1;
                    }
                }
            };
            
            class Node{
                public:
                int index;
                bool reached;
                vector<Link*> links;
                Node(int x){
                    this->reached=false;
                    this->index=x;
                }
                void setUpLink(int target,int distance){
                    Link * newLink = new Link(target,distance);
                    this->links.push_back(newLink);                    
                }
                void updateByGameTick(){
                    if(this->reached){
                        for (int i=0;i<links.size();i++){
                            links[i]->diatanceHaveReached++;
                        }
                    }
                }
            };

            class network{
                public:
                vector<Node*> nodes;
                Node * startPoint;

                network(vector<vector<int>> &times,int N,int K){
                    //创建节点
                    for(int i=1;i<=N;i++){
                        Node * newNode = new Node(i);
                        nodes.push_back(newNode);
                    }

                    // 连接线路 - 注意：只创建单向连接，保持有向图特性
                    for (int i=0;i<times.size();i++){
                        // times[i][0]是源节点，times[i][1]是目标节点，times[i][2]是延迟时间
                        int source = times[i][0]-1;
                        int target = times[i][1]-1;
                        int delay = times[i][2];
                        
                        this->nodes[source]->setUpLink(target, delay);
                        // 删除创建反向连接的代码
                    }

                    // 确定初始节点
                    this->startPoint=nodes[K-1];
                }

                bool isAllReached(){
                    for (int i=0;i<nodes.size();i++){
                        if(nodes[i]->reached==false){
                            return false;
                        }
                    }
                    return true;
                }

                int startGame(){
                    // 首先检查是否所有节点都可以从起点到达
                    // 这不能简单通过检查孤立节点来判断，需要运行算法然后检查是否所有节点都被到达
                    
                    int time = 0; // 从0开始计时更合理
                    this->startPoint->reached = true;
                    
                    // 设置最大迭代次数以避免无限循环
                    const int MAX_ITERATIONS = 10000;
                    int iterations = 0;
                    
                    while (!this->isAllReached() && iterations < MAX_ITERATIONS){
                        iterations++;
                        
                        // 存储本轮新到达的节点
                        vector<int> newly_reached;
                        
                        // 更新所有节点的到达状态
                        for(int i=0;i<this->nodes.size();i++){
                            if (this->nodes[i]->reached) {
                                for (int j=0;j<this->nodes[i]->links.size();j++){
                                    int x;
                                    this->nodes[i]->links[j]->updateByGameTick(x);
                                    if (x!=-1){
                                        newly_reached.push_back(x);
                                    }
                                }
                            }
                        }
                        
                        // 标记这一轮新到达的节点
                        for (int idx : newly_reached) {
                            this->nodes[idx]->reached = true;
                        }

                        // 传播 - 更新所有节点的链接距离
                        for(int i=0;i<this->nodes.size();i++){
                            nodes[i]->updateByGameTick();
                        }
                        
                        time++; // 每完成一轮更新，时间+1
                        
                        // 如果这一轮没有新节点被到达且仍有未到达的节点，说明存在无法到达的节点
                        if (newly_reached.empty() && !this->isAllReached()) {
                            return -1;
                        }
                    }
                    
                    // 检查是否因为达到最大迭代次数而退出
                    if (iterations >= MAX_ITERATIONS && !this->isAllReached()) {
                        return -1;
                    }
                    
                    return this->isAllReached() ? time : -1;
                }
                
                ~network() {
                    // 清理内存
                    for (auto node : nodes) {
                        for (auto link : node->links) {
                            delete link;
                        }
                        delete node;
                    }
                }
            };

            network myNetwork(times,N,K);
            return myNetwork.startGame();            
        }
};

int main(){
    freopen("in.txt","r",stdin);
    int N,K;
    cin>>N;
    cin>>K;
    cin.ignore(); // 消耗换行符

    vector<vector<int>> times;
    string line;
    while(getline(cin,line)){
        if (line.empty()) { // 如果读到空行则停止
            break;
        }
        stringstream ss(line);
        vector<int> row_vector;
        int u, v, w;
        if (ss >> u && ss >> v && ss >> w) { // 尝试读取三个整数
            row_vector.push_back(u);
            row_vector.push_back(v);
            row_vector.push_back(w);
            times.push_back(row_vector);
        } else {
            break; 
        }
    }
    
    Solution mySolution;
    int x=mySolution.networkDelayTime(times,N,K);
    cout<<x;
    return 0;
}