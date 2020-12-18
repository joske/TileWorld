#ifndef __AGENT_HPP__
#define __AGENT_HPP__

#include <iostream>
#include "location.hpp"
#include "gridobjects.hpp"
#include "searchstrategy.hpp"
#include <vector>

using namespace std;

enum State
{
    IDLE = 0,
    MOVE_TO_TILE = 1,
    MOVE_TO_HOLE = 2
};
class Grid;

class Agent : public GridObject
{
public:
    bool hasTile;
    Agent(Grid *g, int pid, Location &loc) : GridObject(loc, AGENT)
    {
        score = 0;
        id = pid;
        state = IDLE;
        tile = NULL;
        hole = NULL;
        grid = g;
        hasTile = false;
    }

    int getId() const;
    void update();
    Tile *getTile() const
    {
        return tile;
    }
    int getScore() const;

    virtual std::ostream &print(std::ostream &strm) const
    {
        return strm << "Agent(id=" << id << ", x=" << getX() << ", y=" << getY() << ", hasTile=" << hasTile << ", score=" << score << ", state=" << state << ")" << endl;
    }

private:
    Grid *grid;
    int id;
    int score;
    State state;
    Tile *tile;
    Hole *hole;
    std::vector<direction> path;

    void idle();
    void moveToTile();
    void moveToHole();
    direction getNextLocalMove(const Location &from, const Location &to);
};
#endif
