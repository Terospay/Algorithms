#include <iostream>
#include <vector>
#include <limits>
#include <stack>
#include <queue>
#include <algorithm>

class graph{
 public:
  graph(int n, int s, int t ){
    capacity = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    flow_in_graph = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    p = std::vector<int>(n, 0);
    dist = std::vector<int>(n, std::numeric_limits<int>::max());
    number_of_vertices = n;
    source = s;
    sink = t;
  }

  void add_edge(int from, int to, int cap){
    capacity[from][to] = cap;
  }

  int findmaxflow(){
    flow_in_graph = std::vector<std::vector<int>>(number_of_vertices, std::vector<int>(number_of_vertices, 0));
    int maxflow = 0;
    while (bfs()){
      p = std::vector<int>(number_of_vertices, 0);
      int flow = dfs(source, std::numeric_limits<int>::max());
      while (flow != 0){
        maxflow += flow;
        flow = dfs(source, std::numeric_limits<int>::max());
      }
    }
    return maxflow;
  }



 private:

  int dfs(int vertex, int minc){
    if (vertex == sink || minc == 0){
      return minc;
    }
    for (int v = p[vertex]; v < number_of_vertices; ++v){
      if (dist[v] == dist[vertex] + 1){
        int delta = dfs(v, std::min(minc, capacity[vertex][v] - flow_in_graph[vertex][v]));
        if (delta != 0){
          flow_in_graph[vertex][v] += delta;
          flow_in_graph[v][vertex] -= delta;
          return delta;
        }
      }
      p[vertex]++;
    }
    return 0;
  }

  bool bfs(){
    dist = std::vector<int>(number_of_vertices, std::numeric_limits<int>::max());
    dist[source] = 0;
    std::queue<int> queue;
    queue.push(source);
    while (!queue.empty()){
      int u = queue.front();
      queue.pop();
      for (int v = 0; v < number_of_vertices; ++v){
        if (flow_in_graph[u][v] < capacity[u][v] && dist[v] == std::numeric_limits<int>::max()){
          dist[v] = dist[u] + 1;
          queue.push(v);
        }
      }
    }
    return dist[sink] != std::numeric_limits<int>::max();
  }


  std::vector<std::vector<int>> capacity;
  std::vector<std::vector<int>> flow_in_graph;
  std::vector<int> p;
  std::vector<int> dist;
  int number_of_vertices = 0;
  int source  = 0;
  int sink = 0;
};


int main(){
  int number_of_vertices = 0;
  int number_of_edges = 0;
  int from = 0;
  int to = 0;
  int cap = 0;

  std::cin >> number_of_vertices >> number_of_edges;
  graph g(number_of_vertices, 0, number_of_vertices - 1);
  for (int i = 0; i < number_of_edges; ++i){
    std::cin>>from>>to>>cap;
    g.add_edge(from - 1, to - 1, cap);
  }
  std::cout<<g.findmaxflow();
  return 0;
}