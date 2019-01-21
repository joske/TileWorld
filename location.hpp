#ifndef __LOCATION_H__
#define __LOCATION_H__

#include <iostream>
#include "searchstrategy.hpp"

using namespace std;

class Location {
    public:
        Location(int xx, int yy) :
                x(xx), y(yy) {
        }
        ~Location() {
        }
        bool operator==(const Location& loc1) {
            return loc1.x == x && loc1.y == y;
        }
        int x, y;
        int distance(Location other);
        Location nextLocation(direction m);
};

std::ostream& operator<<(std::ostream &strm, const Location &l);

#endif
