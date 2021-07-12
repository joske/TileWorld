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
    moveToObject(tile);
}

void Agent::moveToHole()
{
    TRACE_IN
    if (hole && tile)
    {
        LDEBUG(*this << " move to hole " << *hole)
        moveToObject(hole);
    }
}

void Agent::moveToObject(shared_ptr<GridObject> o)
{
    if (getLocation() == o->getLocation())
    {
        if (state == MOVE_TO_TILE)
        {
            pickTile();
        }
        else
        {
            dumpTile();
        }
        return;
    }
    if (o != grid->getObject(o->getLocation().getX(), o->getLocation().getY()))
    {
        if (state == MOVE_TO_TILE)
        {
            // tile is gone
            tile = grid->getClosestTile(loc);
            LDEBUG(*this << " found " << *tile)
        }
        else
        {
            // hole is gone
            hole = grid->getClosestHole(loc);
            LDEBUG(*this << " found " << *hole)
        }
        return;
    }
    if (path.empty())
    {
        path = astar(grid, getLocation(), o->getLocation());
    }
    if (!path.empty())
    {
        const Location newLoc = path.front();
        path.erase(path.begin());
        if (newLoc == o->getLocation() || grid->allowedLocation(newLoc))
        {
            LDEBUG(*this << " next location " << newLoc << " to " << *o)
            grid->move(this, loc, newLoc);
            setLocation(newLoc);
        }
    }
    else
    {
        LDEBUG(*this << " no path found to " << *o)
    }
}

void Agent::dumpTile()
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
}

void Agent::pickTile()
{
    grid->pickTile(tile);
    LDEBUG(*this << " pick tile")
    hasTile = true;
    path.clear();
    hole = grid->getClosestHole(getLocation());
    LDEBUG(*this << " found hole " << *hole)
    state = MOVE_TO_HOLE;
}