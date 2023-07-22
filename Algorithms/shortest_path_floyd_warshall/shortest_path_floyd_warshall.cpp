#include "shortest_path_floyd_warshall.h"

void buildPath (int u, int v, std::vector<std::vector<int> >& p, std::vector<int>& pathz) {
  if (p[u][v] == -1)
        return;
 
    // Storing the path in a vector
    pathz = { u };
    while (u != v) {
        u = p[u][v];
        pathz.push_back(u);
    }
}

void FindRoute(int n, std::vector<Road> roads, int s, int d, int x, int y) {
    // You can change these variables. These are here for demo only.
    std::vector<int> path;
    int cost = INT_MAX;
    // Write your code here...
    int vecSize = -1;
    for(unsigned int i = 0; i < roads.size(); i++){
        vecSize = std::max(vecSize, std::max(roads[i].endpoints.first, roads[i].endpoints.second));
    }


    std::vector<std::vector<int> > roadMatrix( vecSize+1, std::vector<int>(vecSize+1));

    for(int i = 0; i <= vecSize; i++){
        for(int j = 0; j <= vecSize; j++){
                roadMatrix[i][j] = INT_MAX;
        }
    }

    for(Road road : roads){
                    roadMatrix[road.endpoints.first][road.endpoints.second] = road.time;
                    roadMatrix[road.endpoints.second][road.endpoints.first] = road.time;
    }
    // we might need to declare another matrix to store initial routes.
    std::vector<std::vector<int> > predecessorMatrix( vecSize+1, std::vector<int>(vecSize+1));

    for(int i = 0; i <= vecSize; i++){
        for(int j = 0; j <= vecSize; j++){
            if(roadMatrix[i][j] == INT_MAX)
                predecessorMatrix[i][j] = -1;
            else    {
                predecessorMatrix[i][j] = j;
            }

        }
    }



    for (int k = 0; k <= vecSize; k++) {
        for (int i = 0; i <= vecSize; i++) {
            for (int j = 0; j <= vecSize; j++) {
                if (roadMatrix[i][j] > (roadMatrix[i][k] + roadMatrix[k][j]) && (roadMatrix[k][j] != INT_MAX && roadMatrix[i][k] != INT_MAX)){
                    roadMatrix[i][j] = roadMatrix[i][k] + roadMatrix[k][j];
                    predecessorMatrix[i][j] = predecessorMatrix[i][k];
                }
            }
        }
    }


    std::vector<int> firstPath;
    std::vector<int> secondPath;
    std::vector<int> thirdPath;

    int cost1, cost2;

    cost1 = roadMatrix[s][x] + roadMatrix[x][y] + roadMatrix[y][d];
    cost2 = roadMatrix[s][y] + roadMatrix[y][x] + roadMatrix[x][d];

    if(cost1 < cost2){
        buildPath(s, x, predecessorMatrix, firstPath);
        buildPath(x, y, predecessorMatrix, secondPath);
        buildPath(y, d, predecessorMatrix, thirdPath);
        cost = cost1;
    }
    else{
        buildPath(s, y, predecessorMatrix, firstPath);
        buildPath(y, x, predecessorMatrix, secondPath);
        buildPath(x, d, predecessorMatrix, thirdPath);
        cost = cost2;
    }

    for(unsigned int i = 0; i < firstPath.size(); i++)
        path.push_back(firstPath[i]);
    
    for(unsigned int i = 1; i < secondPath.size(); i++)
        path.push_back(secondPath[i]);
    
    for(unsigned int i = 1; i < thirdPath.size(); i++)
        path.push_back(thirdPath[i]);


  // Your output should be like this. You can change this as long as you keep
  // the output format. PrintRange function helps you print elements of
  // containers with iteratos (e.g., std::vector).
  std::cout << cost << " ";
  PrintRange(path.begin(), path.end());
  std::cout << std::endl;
}