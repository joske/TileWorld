#include "location.hpp"
#include <iostream>
#include "searchstrategy.hpp"

int Location::distance(Location other) {
    return abs(x - other.x) + abs(y - other.y);
}

 Location Location::nextLocation(direction m) {
	int nr = x;
	int nc = y;

	switch (m) {
	case UP:
		nr--;
		break;
	case DOWN:
		nr++;
		break;
	case LEFT:
		nc--;
		break;
	case RIGHT:
		nc++;
		break;
	}
	return Location(nr, nc);
}

std::ostream& operator<<(std::ostream &strm, const Location &l) {
    return strm << "Location(" << l.x << ", " << l.y << ")";
}
