#include "dijkstra.h"
#include "registry.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <cmath>


static Registrar _reg_dij("Dijkstra", []{ return std::make_unique<DijkstraPF>(); });


PathResult DijkstraPF::find_path(const Graph& g, int src, int dst) {
using QN = std::pair<double,int>; // (dist, node)
const double INF = std::numeric_limits<double>::infinity();


std::unordered_map<int,double> dist; dist.reserve(1024);
std::unordered_map<int,int> parent; parent.reserve(1024);


for (int u : g.nodes()) dist[u] = INF;
dist[src] = 0.0;


std::priority_queue<QN, std::vector<QN>, std::greater<QN>> pq;
pq.push({0.0, src});


while (!pq.empty()) {
auto [d,u] = pq.top(); pq.pop();
if (d > dist[u]) continue; // stale
if (u == dst) break;
for (const auto &e : g.neighbors(u)) {
double nd = d + e.w;
if (nd < dist[e.v]) {
dist[e.v] = nd;
parent[e.v] = u;
pq.push({nd, e.v});
}
}
}


if (!std::isfinite(dist[dst])) return {{}, INF};


std::vector<int> path; path.reserve(64);
for (int cur = dst; ; ) {
path.push_back(cur);
if (cur == src) break;
cur = parent[cur];
}
std::reverse(path.begin(), path.end());
return {path, dist[dst]};
}