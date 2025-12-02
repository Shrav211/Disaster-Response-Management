#include "prim.h"
#include "registry.h"

#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <cmath>

static Registrar _reg_prim("PrimMST", []{ return std::make_unique<PrimMSTPF>(); });

PathResult PrimMSTPF::find_path(const Graph& g, int src, int dst) {
    using QN = std::pair<double,int>;
    const double INF = std::numeric_limits<double>::infinity();

    // 1. Collect graph nodes
    std::vector<int> nodes = g.nodes();
    if (nodes.empty())
        return {{}, INF};

    // map node ID -> index
    std::unordered_map<int,int> id_to_idx;
    for (int i = 0; i < (int)nodes.size(); i++)
        id_to_idx[nodes[i]] = i;

    // source/destination must exist
    if (!id_to_idx.count(src) || !id_to_idx.count(dst))
        return {{}, INF};

    int n = nodes.size();
    int sIdx = id_to_idx[src];
    int dIdx = id_to_idx[dst];

    // 2. Prim's MST structures
    std::vector<double> key(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<bool> inMST(n, false);

    std::priority_queue<QN, std::vector<QN>, std::greater<QN>> pq;

    key[sIdx] = 0.0;
    pq.push({0.0, sIdx});

    // 3. Prim’s MST algorithm
    while (!pq.empty()) {
        auto [curKey, uIdx] = pq.top();
        pq.pop();
        if (inMST[uIdx]) continue;
        inMST[uIdx] = true;

        int uId = nodes[uIdx];

        // Relax edges
        for (const auto &e : g.neighbors(uId)) {
            if (!id_to_idx.count(e.v)) continue;
            int vIdx = id_to_idx[e.v];
            double w = e.w;

            if (!inMST[vIdx] && w < key[vIdx]) {
                key[vIdx] = w;
                parent[vIdx] = uIdx;
                pq.push({w, vIdx});
            }
        }
    }

    // If dst not in MST
    if (!inMST[dIdx])
        return {{}, INF};

    // 4. Build MST adjacency list
    std::vector<std::vector<std::pair<int,double>>> tree(n);
    for (int vIdx = 0; vIdx < n; vIdx++) {
        int pIdx = parent[vIdx];
        if (pIdx != -1) {
            double w = key[vIdx];
            tree[pIdx].push_back({vIdx, w});
            tree[vIdx].push_back({pIdx, w});
        }
    }

    // 5. DFS to find path in MST
    std::vector<bool> visited(n, false);
    std::vector<int> parentPath(n, -1);
    std::vector<int> stack = { sIdx };
    visited[sIdx] = true;

    while (!stack.empty()) {
        int uIdx = stack.back();
        stack.pop_back();

        if (uIdx == dIdx) break;

        for (auto &edge : tree[uIdx]) {
            int vIdx = edge.first;
            if (!visited[vIdx]) {
                visited[vIdx] = true;
                parentPath[vIdx] = uIdx;
                stack.push_back(vIdx);
            }
        }
    }

    if (!visited[dIdx])
        return {{}, INF};

    // 6. Reconstruct path
    std::vector<int> idxPath;
    for (int cur = dIdx; cur != -1; cur = parentPath[cur]) {
        idxPath.push_back(cur);
        if (cur == sIdx) break;
    }
    std::reverse(idxPath.begin(), idxPath.end());

    // 7. Compute cost on MST
    double cost = 0.0;
    for (size_t i = 0; i + 1 < idxPath.size(); i++) {
        int u = idxPath[i];
        int v = idxPath[i + 1];

        double w = INF;
        for (auto &e : tree[u]) {
            if (e.first == v) {
                w = e.second;
                break;
            }
        }
        if (!std::isfinite(w)) {
            cost = INF;
            break;
        }
        cost += w;
    }

    // Convert index path → node IDs
    std::vector<int> finalPath;
    for (int index : idxPath)
        finalPath.push_back(nodes[index]);

    return { finalPath, cost };
}















//#include "prim.h"
//#include "registry.h"
//#include <vector>
//#include <queue>
//#include <unordered_map>
//#include <limits>
//static Registrar _reg_prim("PrimMST", []{ return std::make_unique<PrimMSTPF>(); });
//PathResult PrimMSTPF::find_path(const Graph& g, int src, int dst) {
// TODO (Teammate #3): implement Prim's MST build; optionally compute a tree path src->dst.
//return {{}, std::numeric_limits<double>::infinity()};
//}


