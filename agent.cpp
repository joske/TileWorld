#include "agent.hpp"
#include "grid.hpp"
#include <limits.h>
#include "main.hpp"
#include "astar.hpp"

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
            path = astar(grid, getLocation(), tile->getLocation());
        }
        if (!path.empty())
        {
            const Location newLoc = path.front();
            path.erase(path.begin());
            grid->move(this, loc, newLoc);
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
            if (hole != NULL)
            {
                LDEBUG(*this << " no path found to " << *tile)
            }
            else
            {
                LDEBUG(*this << " no path found to FOOBAR")
            }
        }
    }
}

void Agent::moveToHole()
{
    TRACE_IN
    if (hole != NULL && tile != NULL)
    {
        LDEBUG(*this << " move to hole " << *hole)
        if (path.empty())
        {
            path = astar(grid, getLocation(), hole->getLocation());
        }
        if (!path.empty())
        {
            const Location newLoc = path.front();
            path.erase(path.begin());
            LDEBUG(*this << " next location " << newLoc << " to hole " << *hole)
            grid->move(this, loc, newLoc);
            setLocation(newLoc);
            if (newLoc == hole->getLocation())
            {
                // we are there, dump the tile
                int sc = grid->dumpTile(tile, hole);
                if (sc != -1)
                {
                    LDEBUG(*this << " dump tile")
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
