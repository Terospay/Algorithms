#include <numeric>
#include <vector>

const int INF = std::numeric_limits<int>::max();

int dijkstra(int from, std::vector<std::vector<std::pair<int, int>>>& g, std::vector<int>& dist, std::vector<int>& prev) {
    int n = g.size();

    dist.assign(n, INF);
    prev.resize(n);
    dist[from] = 0;
	
    std::vector<bool> used(n, false);
    for (int i = 0; i < n; ++i) {
        int v = -1;
        for (int j = 0; j < n; ++j) {
            if (!used[j] && (v == -1 || dist[j] < dist[v])) {
                v = j;
            }
        }
        if (dist[v] == INF) {
            break;
        }
        used[v] = true;

        for (int j = 0; j < g[v].size(); ++j) {
            int to = g[v][j].first;
            int len = g[v][j].second;
            if (dist[v] + len < dist[to]) {
                dist[to] = dist[v] + len;
                prev[to] = v;
            }
        }
    }
}
