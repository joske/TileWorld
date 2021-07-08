#include <vector>
#include <map>
#include <iostream>
#include <vector>
#include "location.hpp"
#include "grid.hpp"
#include "searchstrategy.hpp"

class Node {
    public:
        Node(Location l, vector<Location> p, int f) : location(l), path(p), fscore(f) {}
        Location location;
        vector<Location> path;
        int fscore;

        bool operator==(const Node& other) const
        {
            return location == other.location;
        }
        bool operator!=(const Node& other) const
        {
            return location != other.location;
        }
        bool operator<(const Node& other) const
        {
            return fscore < other.fscore;
        }

    friend std::ostream& operator<<(std::ostream &strm, const Node &n) {
        return strm << "Node(" << n.location << ", fscore=" << n.fscore << ")";
    }
};


std::vector<Location> astar(Grid *grid, Location from, Location to);