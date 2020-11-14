#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;

// Union-Find Disjoint Sets Library written in OOP manner, using both path compression and union by rank heuristics
class UnionFind {                                              // OOP style
 private:
  vi p, rank, setSize;                       // remember: vi is vector<int>
  int numSets;

 public:
  UnionFind(int N) {
    setSize.assign(N, 1);
    numSets = N;
    rank.assign(N, 0);
    p.assign(N, 0);
    for (int i = 0; i < N; i++)
      p[i] = i;
  }

  int findSet(int i) {
    return (p[i] == i) ? i : (p[i] = findSet(p[i]));
  }

  bool isSameSet(int i, int j) {
    return findSet(i) == findSet(j);
  }

  void unionSet(int i, int j) {
    if (!isSameSet(i, j)) {
      numSets--;
      int x = findSet(i), y = findSet(j);
      // rank is used to keep the tree short
      if (rank[x] > rank[y]) {
        p[y] = x; setSize[x] += setSize[y];
      }
      else {
        p[x] = y;
        setSize[y] += setSize[x];
        if (rank[x] == rank[y])
          rank[y]++;
      }
    }
  }
  int numDisjointSets() {
    return numSets;
  }
  int sizeOfSet(int i) {
    return setSize[findSet(i)];
  }
};

int main() {
  /*
  // Graph in Figure 4.10 left, format: list of weighted edges
  // This example shows another form of reading graph input
  5 7
  0 1 4
  0 2 4
  0 3 6
  0 4 6
  1 2 2
  2 3 8
  3 4 9
  ans is 18*/


  int V, E; scanf("%d %d", &V, &E);
  // Kruskal's algorithm merged with Prim's algorithm
  vector<pair<int, ii>> EL;                          // (weight and two end vertices)
  for (int i = 0; i < E; i++) {
    int u, v, w; scanf("%d %d %d", &u, &v, &w);           // read (u, v, w)
    EL.push_back({w, ii(u, v)});
  }
  sort(EL.begin(), EL.end());             // sort by edge weight O(E log E)
  // note: pair object has built-in comparison function

  int mst_cost = 0, num_taken = 0;
  UnionFind UF(V);                     // all V are disjoint sets initially
  for (int i = 0; i < E && num_taken < V-1; i++) {   // for each edge, O(E)
    int w, u, v;
    pair<int, ii> front = EL[i];
    if (!UF.isSameSet(front.second.first, front.second.second)) {                                     // check
      num_taken++;             // one more edge is taken as part of the MST
      mst_cost += front.first;                          // add the weight of e to MST
      UF.unionSet(front.second.first, front.second.second);                                       // link them
    } }                       // note: the runtime cost of UFDS is very light
  // note: the number of disjoint sets must eventually be 1 for a valid MST
  printf("MST cost = %d (Kruskal's)\n", mst_cost);

  return 0;
}