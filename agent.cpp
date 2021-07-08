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
        if (getLocation() == tile->getLocation())
        {
            // we are there, pick the tile
            grid->pickTile(tile);
            LDEBUG(*this << " pick tile")
            hasTile = true;
            path.clear();
            hole = grid->getClosestHole(getLocation());
            LDEBUG(*this << " found hole " << *hole)
            state = MOVE_TO_HOLE;
            return;
        }
        if (tile != grid->getObject(tile->getLocation().getX(), tile->getLocation().getY()))
        {
            // find a new tile
            tile = grid->getClosestTile(getLocation());
            path.clear();
            LDEBUG(*this << " found tile " << *tile)
        }
        if (path.empty())
        {
            path = astar(grid, getLocation(), tile->getLocation());
        }
        if (!path.empty())
        {
            const Location newLoc = path.front();
            path.erase(path.begin());
            if (newLoc == tile->getLocation() || grid->allowedLocation(newLoc))
            {
                grid->move(this, loc, newLoc);
                setLocation(newLoc);
            }
        }
        else
        {
            if (tile)
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
    if (hole && tile)
    {
        LDEBUG(*this << " move to hole " << *hole)
        if (getLocation() == hole->getLocation())
        {
            // we are there, dump the tile
            grid->dumpTile(tile, hole);
            LDEBUG(*this << " dump tile")
            hasTile = false;
            this->score += tile->getScore();
            tile.reset();
            hole.reset();
            tile = grid->getClosestTile(loc);
            LDEBUG(*this << " found tile " << *tile)
            path.clear();
            state = MOVE_TO_TILE;
            return;
        }
        if (hole == grid->getObject(hole->getLocation().getX(), hole->getLocation().getY()))
        {
            // hole is gone
            hole = grid->getClosestHole(loc);
            LDEBUG(*this << " found hole " << *hole)
        }
        if (path.empty())
        {
            path = astar(grid, getLocation(), hole->getLocation());
        }
        if (!path.empty())
        {
            const Location newLoc = path.front();
            path.erase(path.begin());
            if (newLoc == hole->getLocation() || grid->allowedLocation(newLoc))
            {
                LDEBUG(*this << " next location " << newLoc << " to hole " << *hole)
                grid->move(this, loc, newLoc);
                setLocation(newLoc);
            }
        }
        else
        {
            LDEBUG(*this << " no path found to " << *hole)
        }
    }
}
