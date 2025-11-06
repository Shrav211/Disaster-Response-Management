#include "graph.h"


void Graph::add_node(int u) {
if (!adj_.count(u)) adj_[u] = {};
}


void Graph::add_edge(int u, int v, double w) {
add_node(u); add_node(v);
adj_[u].push_back(Edge{v, w});
}


const std::vector<Edge>& Graph::neighbors(int u) const {
static const std::vector<Edge> kEmpty;
auto it = adj_.find(u);
if (it == adj_.end()) return kEmpty;
return it->second;
}


std::vector<int> Graph::nodes() const {
std::vector<int> out; out.reserve(adj_.size());
for (auto &kv : adj_) out.push_back(kv.first);
return out;
}