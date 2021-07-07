#ifndef __LOCATION_H__
#define __LOCATION_H__

#include <iostream>
#include "searchstrategy.hpp"
#include "main.hpp"

using namespace std;

class Location {
    public:
        Location(int xx, int yy) : x(xx), y(yy)
        {
        }

        bool operator==(const Location& loc1) const {
            bool equal = loc1.x == x && loc1.y == y;
            return equal;
        }
        bool operator!=(const Location& loc1) const {
            return loc1.x != x || loc1.y != y;
        }
        int distance(const Location& other) const;
        Location nextLocation(const direction m) const;
        direction getDirection(Location next) const;

	int getX() const { return x; }
	int getY() const { return y; }

    private:
        int x, y;

    friend std::ostream& operator<<(std::ostream &strm, const Location &l) {
            return strm << "Location(" << l.x << ", " << l.y << ")";
    }
};

#endif
