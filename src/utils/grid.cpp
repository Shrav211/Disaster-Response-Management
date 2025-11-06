#include "grid.h"


Graph make_grid_graph(const GridSpec& gs) {
Graph g;
auto idx = [&](int r, int c){ return r*gs.cols + c; };
for (int r=0; r<gs.rows; ++r) {
for (int c=0; c<gs.cols; ++c) {
int u = idx(r,c);
if (r+1 < gs.rows) { int v = idx(r+1,c); g.add_edge(u,v,gs.w); g.add_edge(v,u,gs.w);}
if (c+1 < gs.cols) { int v = idx(r,c+1); g.add_edge(u,v,gs.w); g.add_edge(v,u,gs.w);}
}
}
return g;
}