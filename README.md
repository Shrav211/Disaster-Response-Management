

**Language:** C++17 | **Build:** Makefile | **Tested on:** engnx04a.utdallas.edu (g++ 8.5.0)

This project is fully compatible with the default g++ compiler available on engnx04a.utdallas.edu.

## Compile & Run (on engnx server)
```bash
make clean && make
./build/disaster_sim tests/grid_5x5.txt tests/resources_basic.txt tests/incidents_basic.txt
```


## Input Formats
### 1) Grid File (grid_5x5.txt)
```
# rows cols weight
5 5 1
```
This generates an undirected 5x5 grid with uniform edge weight=1.


### 2) Resources File (resources_basic.txt)
```
# id node kind
A1 0 ambulance
F1 24 firetruck
```


### 3) Incidents File (incidents_basic.txt)
```
# id node severity
E1 12 3
```


## Algorithms (Plug-and-Play)
We expose a registry. Select the algorithm using the `--algo` flag (`Dijkstra`, `AStar`, `PrimMST`). Example:
```bash
./build/disaster_sim tests/grid_5x5.txt tests/resources_basic.txt tests/incidents_basic.txt --algo Dijkstra
```
Your teammates only touch `src/algorithms/astar.*` and `src/algorithms/prim.*`.

Note: `AStar` is implemented using a Manhattan heuristic for grid graphs (main recreates AStar with the grid width so the heuristic is correct). Use `--algo AStar` to run A*.


## Output
- Prints selected algorithm, chosen resource, path, and path cost.
- ASCII grid annotated with resource/incident positions.


## Test Cases
Add more files to `tests/` and run with `make run` or pass paths manually.


## Dependencies/Assumptions
- No external libs required. Console UI only (ASCII), compliant with headless server.