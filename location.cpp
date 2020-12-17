#include "location.hpp"
#include <iostream>
#include "searchstrategy.hpp"

int Location::distance(const Location &other) const
{
    return abs(x - other.x) + abs(y - other.y);
}

Location Location::nextLocation(const direction m) const
{
    int nx = x;
    int ny = y;

    switch (m)
    {
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
    case STUCK:
        break;
    }
    return Location(nx, ny);
}

direction Location::getDirection(Location next) const
{
    if (x == next.x)
    {
        if (y == y + 1)
        {
            return LEFT;
        }
        else
        {
            return RIGHT;
        }
    }
    else
    {
        if (x == next.x + 1)
        {
            return UP;
        }
        else
        {
            return DOWN;
        }
    }
}
