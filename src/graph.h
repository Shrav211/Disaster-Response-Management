#pragma once
#include <vector>
#include <unordered_map>
#include <limits>
#include <utility>


struct Edge { int v; double w; };


class Graph {
public:
// Directed weighted graph
void add_node(int u);
void add_edge(int u, int v, double w);
const std::vector<Edge>& neighbors(int u) const;
std::vector<int> nodes() const;
private:
std::unordered_map<int, std::vector<Edge>> adj_;
};