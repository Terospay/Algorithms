#include <numeric>
#include <vector>
#include <set>

const int INF = std::numeric_limits<int>::max();

int dijkstra(int from, std::vector<std::vector<std::pair<int, int>>>& g, std::vector<int>& dist, std::vector<int>& prev) {
    int n = g.size();

    dist.assign(n, INF);
    prev.resize(n);
    dist[from] = 0;

    std::set<std::pair<int, int>> q;
    q.insert(std::make_pair(dist[from], from));
    while (!q.empty()) {
        int v = q.begin()->second;
        q.erase(q.begin());

        for (int j = 0; j < g[v].size(); ++j) {
            int to = g[v][j].first;
            int len = g[v][j].second;
            if (dist[v] + len < dist[to]) {
                q.erase(std::make_pair(dist[to], to));
                dist[to] = dist[v] + len;
                prev[to] = v;
                q.insert(std::make_pair(dist[to], to));
            }
        }
    }
}
