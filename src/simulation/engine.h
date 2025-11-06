#pragma once
#include <vector>
#include <string>
#include "../graph.h"
#include "../algorithms/base.h"
#include "models.h"
#include <memory>


class SimulationEngine {
public:
SimulationEngine(const Graph& g, std::vector<Resource> resources, std::unique_ptr<PathFinder> pf);


// Returns chosen resource id, path, and cost
std::tuple<std::string, std::vector<int>, double> assign_nearest(const Incident& inc);


private:
const Graph& g_;
std::vector<Resource> R_;
std::unique_ptr<PathFinder> pf_;
};