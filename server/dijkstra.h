//---------------------------------------------------
// Name: Filippo Ciandy, Rowan Tilroe
// ID:1641346,1629172
// CMPUT 275, Winter 2020
// Assingment 2 Part 1: Directions
//---------------------------------------------------
#ifndef _DIJKSTRA_H_
#define _DIJKSTRA_H_

#include "wdigraph.h"
#include "heap.h"
#include <utility>
#include <unordered_map>

using namespace std;

// for brevity
// typedef introduces a synonym (alias)
// for the type specified
typedef pair<int, int> PII; // (predecessor, vertex)
typedef pair<PII, long long> PPIIL; // ((predecessor, vertex), cost)
// tree has key of current vertex, value of predecessor

void dijkstra(const WDigraph& graph, int startVertex,unordered_map<int, int>& tree);


#endif
