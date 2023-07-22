#include "topological_sort_cycle_detection"
// Don't add any additional includes
/*
  N: Number of racers
  updates: Updates you have heard
*/
void DFS(std::vector<std::pair<int, int>> graph, int n, int v, std::vector<int>& arrival, std::vector<int>& departure, std::vector<int>& explored, int& time) {
    arrival[v] = time++;
    explored[v] = true;
    //int flag = 1;

    for (int i = 0; i < n; i++) { 
        for(unsigned int j = 0; j < graph.size(); j++){    
            if (graph[j].first == v && graph[j].second == i) {
                if(explored[i] == -1)
                    DFS(graph, n, i, arrival, departure, explored, time);
                     
            }
        }
    }

    departure[v] = time++;
}


std::vector<int> findCycle(int N, std::vector<std::pair<int, int>> updates){
    std::vector<int> arrival(N, -1);
    std::vector<int> departure(N, -1);
    std::vector<int> explored(N, -1);
    std::vector<int> result;
    int time = 0;

    for (int i = 0; i < N; i++) {
        result.empty();
        if (explored[i] == -1) 
           DFS(updates, N, i, arrival, departure, explored, time);
            
    }

    for (int i = 0; i < N; i++) {
        std::cout << "Vertex " << i << " (" << arrival[i] << ", " << departure[i] << ")\n";
    }
    std::vector<std::pair<int, int>> dummy = updates;

    

    for(unsigned int k = 0; k < updates.size(); k++){
    for (int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(i==j)
                continue;
            if(arrival[i]>arrival[j] && departure[j]>departure[i] && updates[k].first == i && updates[k].second == j){
                std::cout << arrival[i] << " " << departure[i] << " " << i << " " << arrival[j] << " " << departure[j]<< " " << j <<std::endl;
                for(int m = arrival[j]; m <= arrival[i]; m++){
                        for (int r = 0; r < N; r++){
                            if(arrival[r] == m ){
                                for (int x = 0; x < N; x++){
                                    if(arrival[x]==m+1 )
                                        for(unsigned int s = 0; s < updates.size(); s++){
                                            if(r == updates[s].first && x == updates[s].second)
                                                result.push_back(r);
                                        }
                                        
                                }
                            }
                        }
                }
                return result;
            }
        }
    }
     
    }

    return result;

}




std::pair<bool, std::vector<int>>
RaceResult(int N, std::vector<std::pair<int, int>> updates) {

    std::vector<int> in_degree(N, 0);

    int maxNode = 0;

    for (unsigned int j = 0;  j < updates.size(); j++){
            if(updates[j].second > maxNode)
                maxNode = updates[j].second;
            if(updates[j].first > maxNode)
                maxNode = updates[j].first;
    }

    bool is_dag = true;
 

    for (int u = 0; u <= maxNode; u++) {
        for (unsigned int i = 0;  i < updates.size(); i++){
            if(updates[i].second == u)
                in_degree[u]++;
        }
    }
 
    std::queue<int> q;
    for (int i = 0; i <= maxNode; i++)
        if (in_degree[i] == 0)
            q.push(i);
 
    int cnt = 0;
 
    std::vector<int> top_order;
 

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        top_order.push_back(u);
 

        for(unsigned int i = 0;  i < updates.size(); i++){
            if(updates[i].first == u){
                in_degree[updates[i].second]--;
                if(in_degree[updates[i].second] == 0)
                    q.push(updates[i].second);
            }    
        }
 
        cnt++;
    }

    if (cnt != maxNode + 1) {
        is_dag = false;
        std::vector<int> cycle = findCycle(N, updates);
    
    
        
        top_order = cycle;
        top_order.push_back(cycle[0]); 
    }




  return {is_dag, top_order};
}
