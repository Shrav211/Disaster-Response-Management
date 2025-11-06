#include <bits/stdc++.h>
#include "graph.h"
#include "algorithms/registry.h"
#include "algorithms/dijkstra.h"
#include "algorithms/astar.h"
#include "algorithms/prim.h"
#include "simulation/engine.h"
#include "simulation/models.h"
#include "utils/grid.h"

using namespace std;

static void die(const string& msg) {
    cerr << "[ERROR] " << msg << "\n";
    exit(1);
}

static void load_grid_spec(const string& path, GridSpec& gs) {
    ifstream in(path);
    if (!in) die("Cannot open grid file: " + path);
    string line;
    bool ok = false;
    while (getline(in, line)) {
        if (line.empty() || line[0]=='#') continue;
        stringstream ss(line);
        if (ss >> gs.rows >> gs.cols >> gs.w) { ok = true; break; }
    }
    if (!ok) die("Bad grid file format: " + path);
}

static vector<Resource> load_resources(const string& path) {
    ifstream in(path);
    if (!in) die("Cannot open resources file: " + path);
    vector<Resource> R; string line;
    while (getline(in, line)) {
        if (line.empty() || line[0]=='#') continue;
        string id, kind; int node; stringstream ss(line);
        if (ss >> id >> node >> kind) R.push_back(Resource{id,node,kind,true});
    }
    if (R.empty()) cerr << "[WARN] No resources loaded from " << path << "\n";
    return R;
}

static vector<Incident> load_incidents(const string& path) {
    ifstream in(path);
    if (!in) die("Cannot open incidents file: " + path);
    vector<Incident> E; string line;
    while (getline(in, line)) {
        if (line.empty() || line[0]=='#') continue;
        string id; int node, sev; stringstream ss(line);
        if (ss >> id >> node >> sev) E.push_back(Incident{id,node,sev});
    }
    if (E.empty()) cerr << "[WARN] No incidents loaded from " << path << "\n";
    return E;
}

static void print_grid_ascii(int rows, int cols, int incident_node,
                             const vector<Resource>& R,
                             const vector<int>& path)
{
    unordered_set<int> pathset(path.begin(), path.end());
    for (int r=0; r<rows; ++r) {
        for (int c=0; c<cols; ++c) {
            int idx = r*cols + c;
            string cell = to_string(idx);
            if (idx == incident_node) cell = "E";
            for (auto &res : R) if (res.node == idx) cell = "R"; // initial resource positions
            if (pathset.count(idx)) cell = "*";
            cout << setw(3) << cell;
        }
        cout << '\n';
    }
}

// Severity-priority wrapper with FIFO tie-breaker
struct IncWrap { Incident inc; int order; };
struct BySeverity {
    bool operator()(const IncWrap& a, const IncWrap& b) const {
        if (a.inc.severity != b.inc.severity) return a.inc.severity < b.inc.severity; // higher first
        return a.order > b.order; // earlier first
    }
};

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 4) {
        cerr << "Usage: " << argv[0]
             << " <grid_file> <resources_file> <incidents_file> [--algo NAME]\n";
        cerr << "Available algorithms: ";
        for (auto &n : AlgorithmRegistry::instance().names()) cerr << n << " ";
        cerr << "\n";
        return 1;
    }
    string grid_path = argv[1], res_path = argv[2], inc_path = argv[3];
    string algo = "Dijkstra";
    for (int i=4; i<argc; ++i)
        if (string(argv[i])=="--algo" && i+1<argc) algo = argv[++i];

    // Load inputs
    GridSpec gs{5,5,1};
    load_grid_spec(grid_path, gs);
    auto G = make_grid_graph(gs);
    auto resources = load_resources(res_path);
    auto incidents = load_incidents(inc_path);
    if (incidents.empty()) die("No incidents to process.");

    // Create algorithm instance
    auto pf = AlgorithmRegistry::instance().create(algo);
    if (!pf) {
        cerr << "[ERROR] Unknown algorithm: " << algo << "\nAvailable: ";
        for (auto &n : AlgorithmRegistry::instance().names()) cerr << n << " ";
        cerr << "\n";
        return 2;
    }
    // Special-case A* to pass grid width for Manhattan heuristic
    if (algo == "AStar") {
        pf = std::make_unique<AStarPF>(gs.cols);
    }

    SimulationEngine engine(G, resources, std::move(pf));

    // Build a severity-priority queue (max-heap), FIFO on ties
    priority_queue<IncWrap, vector<IncWrap>, BySeverity> pq;
    for (int i = 0; i < (int)incidents.size(); ++i) pq.push({incidents[i], i});

    cout << "Algorithm: " << algo << "\n";
    cout << "Processing " << incidents.size() << " incidents by severity...\n\n";

    int processed = 0;
    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();
        const auto &inc = cur.inc;

        auto t0 = chrono::high_resolution_clock::now();
        auto [rid, path, cost] = engine.assign_nearest(inc);
        auto t1 = chrono::high_resolution_clock::now();
        auto us = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();

        cout << "#" << (++processed) << ") Incident " << inc.id
             << " @ node " << inc.node
             << " | severity " << inc.severity
             << " | resource: " << (rid.empty()? string("None"): rid)
             << " | cost: " << (std::isfinite(cost)? cost : -1)
             << " | runtime: " << us << " us\n";

        // Optional: detailed grid for the first processed incident
        cout << "\nGrid view (initial resource positions, * = path, E = incident)\n";
        print_grid_ascii(gs.rows, gs.cols, inc.node, resources, path);
        cout << "\n----------------------------------------\n\n";

    }

    return 0;
}
