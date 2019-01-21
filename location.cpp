#include "location.hpp"
#include <iostream>
#include "searchstrategy.hpp"

int Location::distance(Location other) {
    return abs(x - other.x) + abs(y - other.y);
}

Location Location::nextLocation(direction m) {
    int nx = x;
    int ny = y;

    switch (m) {
        case UP:
            ny--;
            break;
        case DOWN:
            ny++;
            break;
        case LEFT:
            nx--;
            break;
        case RIGHT:
            nx++;
            break;
    }
    return Location(nx, ny);
}

std::ostream& operator<<(std::ostream &strm, const Location &l) {
    return strm << "Location(" << l.x << ", " << l.y << ")";
}
