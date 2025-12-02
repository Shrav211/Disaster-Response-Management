#include "prim.h"
#include "registry.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>


static Registrar _reg_prim("PrimMST", []{ return std::make_unique<PrimMSTPF>(); });


PathResult PrimMSTPF::find_path(const Graph& g, int src, int dst) {
// TODO (Teammate #3): implement Prim's MST build; optionally compute a tree path src->dst.
return {{}, std::numeric_limits<double>::infinity()};
}


