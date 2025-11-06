#include "engine.h"
#include <limits>


SimulationEngine::SimulationEngine(const Graph& g, std::vector<Resource> resources, std::unique_ptr<PathFinder> pf)
: g_(g), R_(std::move(resources)), pf_(std::move(pf)) {}


std::tuple<std::string, std::vector<int>, double> SimulationEngine::assign_nearest(const Incident& inc) {
std::string best_id;
std::vector<int> best_path;
double best_cost = std::numeric_limits<double>::infinity();


for (auto &r : R_) {
if (!r.available) continue;
auto res = pf_->find_path(g_, r.node, inc.node);
if (res.second < best_cost) {
best_cost = res.second;
best_path = std::move(res.first);
best_id = r.id;
}
}
// mark chosen resource busy & move it (instant for v0)
if (!best_id.empty()) {
for (auto &r : R_) if (r.id == best_id) { r.available = false; r.node = inc.node; }
}
return {best_id, best_path, best_cost};
}