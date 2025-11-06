#pragma once
#include "base.h"


class DijkstraPF : public PathFinder {
public:
PathResult find_path(const Graph& g, int src, int dst) override;
};