#include "astar.h"
#include "registry.h"
#include <queue>
#include <unordered_map>
#include <limits>
#include <cmath>


// NOTE: We register a default A*; main can re-create with grid width if needed.
static Registrar _reg_astar("AStar", []{ return std::make_unique<AStarPF>(1); });


double AStarPF::heuristic(int a, int b) const {
if (cols_ <= 0) return 0.0;
int ar = a / cols_, ac = a % cols_;
int br = b / cols_, bc = b % cols_;
return std::abs(ar - br) + std::abs(ac - bc);
}


PathResult AStarPF::find_path(const Graph& g, int src, int dst) {
// TODO (Teammate #2): implement A* using f=g+h, relax neighbors, parent map.
// Return {path, cost}; if no path: return {{}, INF}.
return {{}, std::numeric_limits<double>::infinity()};
}