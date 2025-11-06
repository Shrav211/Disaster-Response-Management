#pragma once
#include "base.h"


class AStarPF : public PathFinder {
public:
// Teammate implements A* with f=g+h; provide cols for Manhattan heuristic on grid
explicit AStarPF(int cols = 1) : cols_(cols) {}
PathResult find_path(const Graph& g, int src, int dst) override;
private:
int cols_;
double heuristic(int a, int b) const; // Manhattan on grid
};