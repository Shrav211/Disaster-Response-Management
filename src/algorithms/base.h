#pragma once
#include <vector>
#include <string>
#include <utility>
#include "../graph.h"


// Path = sequence of node IDs; cost = total weight
using PathResult = std::pair<std::vector<int>, double>;


struct PathFinder {
virtual ~PathFinder() = default;
virtual PathResult find_path(const Graph& g, int src, int dst) = 0;
};