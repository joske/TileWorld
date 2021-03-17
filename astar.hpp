#include <vector>
#include <map>
#include <iostream>
#include "location.hpp"
#include "grid.hpp"
#include "searchstrategy.hpp"

class Node {
    public:
        Node(Location l, Node *p, int f) : location(l), parent(p), fscore(f) {}
        Location location;
        Node *parent;
        int fscore;
};

std::vector<direction> astar(Grid *grid, Location from, Location to);