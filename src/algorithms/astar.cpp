#include "astar.h"
#include "registry.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>


// NOTE: We register a default A*; main can re-create with grid width if needed.
static Registrar _reg_astar("AStar", []{ return std::make_unique<AStarPF>(1); });


double AStarPF::heuristic(int a, int b) const {
if (cols_ <= 0) return 0.0;
int ar = a / cols_, ac = a % cols_;
int br = b / cols_, bc = b % cols_;
return std::abs(ar - br) + std::abs(ac - bc);
}


PathResult AStarPF::find_path(const Graph& g, int src, int dst) {
	const double INF = std::numeric_limits<double>::infinity();
	// trivial case
	if (src == dst) return {{src}, 0.0};

	// g-score: best known cost from src to node
	std::unordered_map<int,double> gscore; gscore.reserve(1024);
	std::unordered_map<int,int> parent; parent.reserve(1024);

	for (int u : g.nodes()) gscore[u] = INF;
	gscore[src] = 0.0;

	using PQE = std::pair<double,int>; // (f, node)
	std::priority_queue<PQE, std::vector<PQE>, std::greater<PQE>> pq;
	pq.push({heuristic(src,dst), src});

	while (!pq.empty()) {
		auto [f,u] = pq.top(); pq.pop();
		double cur_g = gscore[u];
		// stale entry check: if this f is worse than best-known f, skip
		if (f > cur_g + heuristic(u,dst)) continue;
		if (u == dst) break;

		for (const auto &e : g.neighbors(u)) {
			double tentative_g = cur_g + e.w;
			if (tentative_g < gscore[e.v]) {
				gscore[e.v] = tentative_g;
				parent[e.v] = u;
				double nf = tentative_g + heuristic(e.v,dst);
				pq.push({nf, e.v});
			}
		}
	}

	if (!std::isfinite(gscore[dst])) return {{}, INF};

	std::vector<int> path; path.reserve(64);
	for (int cur = dst; ; ) {
		path.push_back(cur);
		if (cur == src) break;
		cur = parent[cur];
	}
	std::reverse(path.begin(), path.end());
	return {path, gscore[dst]};
}