#include "agent.hpp"
#include "grid.hpp"
#include <limits.h>
#include "main.hpp"
#include "path.hpp"

int Agent::getId() const
{
    return id;
}

int Agent::getScore() const
{
    return score;
}

void Agent::update()
{
    switch (state)
    {
    case IDLE:
        idle();
        break;
    case MOVE_TO_TILE:
        moveToTile();
        break;
    case MOVE_TO_HOLE:
        moveToHole();
        break;
    }
}

void Agent::idle()
{
    state = MOVE_TO_TILE;
}

void Agent::moveToTile()
{
    TRACE_IN
    // maybe another tile is now closer?
    tile = grid->getClosestTile(loc);
    if (tile != NULL)
    {
        LDEBUG(*this << " move to tile " << *tile)
        if (path.empty())
        {
            path = shortestPath(grid, getLocation(), tile->getLocation());
        }
        if (!path.empty())
        {
            direction m = path.front();
            Location newLoc = loc.nextLocation(m);
            grid->move(loc, newLoc);
            setLocation(newLoc);
            if (newLoc == tile->getLocation())
            {
                // we are there, pick the tile
                bool tileStilThere = grid->pickTile(tile);
                if (tileStilThere)
                {
                    hasTile = true;
                    state = MOVE_TO_HOLE;
                }
            }
        }
    }
}

void Agent::moveToHole()
{
    TRACE_IN
    hole = grid->getClosestHole(loc);
    if (hole != NULL && tile != NULL)
    {
        if (path.empty())
        {
            path = shortestPath(grid, getLocation(), hole->getLocation());
        }
        direction m = path.front();
        LDEBUG(*this << " next move " << m << " to hole " << *hole)
        Location newLoc = loc.nextLocation(m);
        grid->move(loc, newLoc);
        setLocation(newLoc);
        if (newLoc == hole->getLocation())
        {
            // we are there, dump the tile
            int sc = grid->dumpTile(tile, hole);
            if (sc != -1)
            {
                delete tile;
                delete hole;
                tile = NULL;
                hole = NULL;
                hasTile = false;
                this->score += sc;
                state = MOVE_TO_TILE;
            }
        }
    }
}

direction Agent::getNextLocalMove(const Location &from, const Location &to)
{
    TRACE_IN
    LDEBUG(*this << " move from " << from << " to " << to)
    int r = (rand() % 100) + 1;
    if (r > 80)
    {
        LDEBUG(*this << " random move")
        r = (rand() % 4);
        direction d = static_cast<direction>(r);
        int count = 0;
        while (!grid->possibleMove(from, d))
        {
            r = (rand() % 4);
            d = static_cast<direction>(r);
            if (count++ == 1000)
                break;
        }
        if (count >= 1000)
        {
            LDEBUG(*this << " is stuck ")
            return STUCK;
        }
        return d;
    }
    int min_dist = INT_MAX;
    direction best_move = STUCK;
    for (int dir = 0; dir < 4; dir++)
    {
        direction d = static_cast<direction>(dir);
        Location l = from.nextLocation(d);
        if (l == to)
        {
            // we have arrived
            best_move = d;
            break;
        }
        else if (grid->possibleMove(from, d))
        {
            int dist = l.distance(to);
            if (dist < min_dist)
            {
                min_dist = dist;
                best_move = d;
            }
        }
    }
    return best_move;
}
