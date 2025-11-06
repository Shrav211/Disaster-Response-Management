#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include "base.h"


class AlgorithmRegistry {
public:
using Factory = std::function<std::unique_ptr<PathFinder>()>;
static AlgorithmRegistry& instance() { static AlgorithmRegistry R; return R; }


void register_algo(const std::string& name, Factory f) { factories_[name] = std::move(f); }
std::unique_ptr<PathFinder> create(const std::string& name) const {
auto it = factories_.find(name);
if (it == factories_.end()) return nullptr;
return (it->second)();
}
std::vector<std::string> names() const {
std::vector<std::string> v; v.reserve(factories_.size());
for (auto &kv : factories_) v.push_back(kv.first);
return v;
}
private:
std::unordered_map<std::string, Factory> factories_;
};


struct Registrar {
Registrar(const std::string& name, AlgorithmRegistry::Factory f) {
AlgorithmRegistry::instance().register_algo(name, std::move(f));
}
};