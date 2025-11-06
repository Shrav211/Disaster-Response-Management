#pragma once
#include "../graph.h"


struct GridSpec { int rows; int cols; double w; };


Graph make_grid_graph(const GridSpec& gs);