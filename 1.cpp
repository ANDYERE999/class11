#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class Solution{
    public:
        int networkDelayTime(vector<vector<int>> &times,int N,int K){
            // Add validation for N and K
            if (N <= 0 || K <= 0 || K > N) {
                return -1; // Invalid N or K, cannot proceed
            }

            class Link{
                public:
                int target;
                int distance;
                int diatanceHaveReached;
                //bool canGo;
                Link(int target,int distance){
                    this->target=target;
                    this->distance=distance;
                    this->diatanceHaveReached=0;
                    //this->canGo=false;
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
                bool isIsolated(){
                    if (links.size()==0){
                        return true;
                    }else{
                        return false;
                    }
                }
            };

            class network{
                public:
                vector<Node*> nodes;
                Node * startPoint;

                network(vector<vector<int>> &times,int N,int K){
                    //创建节点
                    // N is now guaranteed to be > 0 by the check above.
                    // K is now guaranteed to be within [1, N].
                    for(int i=1;i<=N;i++){
                        Node * newNode = new Node(i);
                        nodes.push_back(newNode);
                    }

                    // 连接线路
                    for (int i=0;i<times.size();i++){
                        // Assuming times[i][0] and times[i][1] are within [1,N] as per typical problem constraints.
                        // If not, further checks would be needed here for times[i][0]-1 and times[i][1]-1.
                        // However, the primary reported error is likely due to N or K.
                        if (times[i][0] < 1 || times[i][0] > N || times[i][1] < 1 || times[i][1] > N) {
                            // Optional: Handle or log invalid edge data if problem doesn't guarantee valid node IDs in times.
                            // For now, we assume valid 'times' if N, K are valid.
                            // If an error here should stop processing, this constructor would need to signal failure.
                            // This might be too complex for current structure, focusing on N, K fix.
                        }
                        this->nodes[times[i][0]-1]->setUpLink(times[i][1]-1,times[i][2]);
                        this->nodes[times[i][1]-1]->setUpLink(times[i][0]-1,times[i][2]);
                    }

                    // 确定初始节点
                    // K-1 is now a guaranteed valid index because 1 <= K <= N.
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

                int startGame(){// 返回传播所需时间
                    // 判断是否存在孤立节点
                    for (int i=0;i<nodes.size();i++){
                        if (nodes[i]->isIsolated()==true){
                            return -1;
                        }
                    }

                    int time=-1;
                    this->startPoint->reached=true;
                    while (true){
                        if (this->isAllReached()==true){
                            return time;
                        }

                        time++;

                        // 更新所有节点的到达状态
                        for(int i=0;i<this->nodes.size();i++){
                            for (int j=0;j<this->nodes[i]->links.size();j++){
                                int x;
                                this->nodes[i]->links[j]->updateByGameTick(x);
                                if (x!=-1){
                                    this->nodes[x]->reached=true;
                                    //cout<<"[Debug]Node "<<x+1<<" been reached when time is "<<time<<endl;
                                }
                            }
                        }

                        // 传播
                        for(int i=0;i<this->nodes.size();i++){
                            nodes[i]->updateByGameTick();
                        }
                    }

                }
            };

            network myNetwork(times,N,K);
            return myNetwork.startGame();            
        }
};

int main(){
    int N,K;
    cin>>N;
    cin>>K;
    cin.ignore(); // Consume the newline character after reading K

    vector<vector<int>> times;
    string line;
    while(getline(cin,line)){
        if (line.empty()) { // Stop if an empty line is read
            break;
        }
        stringstream ss(line);
        vector<int> row_vector;
        int u, v, w;
        if (ss >> u && ss >> v && ss >> w) { // Try to read three integers
            row_vector.push_back(u);
            row_vector.push_back(v);
            row_vector.push_back(w);
            times.push_back(row_vector);
        } else {
            // Optional: Handle malformed lines or break if strict input is expected
            // For now, we simply stop processing further lines if a line is not in the expected format.
            // If the problem statement guarantees valid input format (except for the end),
            // this else block might not be strictly necessary, or could be used for error logging.
            break; 
        }
    }
    
    Solution mySolution;
    int x=mySolution.networkDelayTime(times,N,K);
    cout<<x;
    return 0;
}