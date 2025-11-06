#pragma once
#include "base.h"


class PrimMSTPF : public PathFinder {
public:
// Teammate #3: build MST to represent coordination network; optionally derive a path.
PathResult find_path(const Graph& g, int src, int dst) override;
};