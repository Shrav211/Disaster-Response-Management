#pragma once
#include <string>


struct Resource {
std::string id;
int node;
std::string kind; // ambulance, firetruck, etc.
bool available = true;
};


struct Incident {
std::string id;
int node;
int severity; // 1..5
};