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
    tile = grid->getClosestTile(loc);
    LDEBUG(*this << " found tile " << *tile)
    state = MOVE_TO_TILE;
}

void Agent::moveToTile()
{
    TRACE_IN
    // maybe another tile is now closer?
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
            path.erase(path.begin());
            const Location newLoc = loc.nextLocation(m);
            grid->move(loc, newLoc);
            setLocation(newLoc);
            if (newLoc == tile->getLocation())
            {
                // we are there, pick the tile
                bool tileStilThere = grid->pickTile(tile);
                if (tileStilThere)
                {
                    LDEBUG(*this << " pick tile")
                    hasTile = true;
                    path.clear();
                    hole = grid->getClosestHole(newLoc);
                    LDEBUG(*this << " found hole " << *hole)
                    state = MOVE_TO_HOLE;
                }
                else
                {
                    // find a new tile
                    tile = grid->getClosestTile(newLoc);
                    LDEBUG(*this << " found tile " << *tile)
                }
            }
        }
        else
        {
            LDEBUG(*this << " no path found to " << *hole)
        }
    }
}

void Agent::moveToHole()
{
    TRACE_IN
    if (hole != NULL && tile != NULL)
    {
        if (path.empty())
        {
            path = shortestPath(grid, getLocation(), hole->getLocation());
        }
        if (!path.empty())
        {
            direction m = path.front();
            path.erase(path.begin());
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
                    LDEBUG(*this << " dump tile")
                    delete tile;
                    delete hole;
                    tile = NULL;
                    hole = NULL;
                    hasTile = false;
                    this->score += sc;
                    tile = grid->getClosestTile(loc);
                    LDEBUG(*this << " found tile " << *tile)
                    path.clear();
                    state = MOVE_TO_TILE;
                }
                else
                {
                    hole = grid->getClosestHole(loc);
                    LDEBUG(*this << " found hole " << *hole)
                }
            }
        }
        else
        {
            LDEBUG(*this << " no path found to " << *hole)
        }
    }
}
