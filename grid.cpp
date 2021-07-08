#include "grid.hpp"
#include "agent.hpp"
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <algorithm>
#include <memory>
#ifdef GTKGUI
#include <gtkmm.h>
#endif
#include "main.hpp"

void Grid::start(void)
{
    LDEBUG("start");
#ifdef NOGUI
    while (true)
    {
        update();
        sleep(1);
    }
#endif
}

Location Grid::randomFreeLocation() const
{
    TRACE_IN
    int c = RND(COLS);
    int r = RND(ROWS);
    while (objects[c][r] != NULL)
    {
        c = RND(COLS);
        r = RND(ROWS);
    }
    return Location(c, r);
}

bool Grid::isFree(const Location &location) const
{
    shared_ptr<GridObject> o = objects[location.getX()][location.getY()];
    if (o)
    {
        return false;
    }
    return true;
}

bool Grid::possibleMove(const Location &from, const direction m) const
{
    // TRACE_IN
    Location newloc = from.nextLocation(m);
    int col = newloc.getX();
    int row = newloc.getY();

    if (!allowedLocation(newloc))
    {
        return false;
    }
    return isFree(newloc);
}

bool Grid::allowedLocation(const Location &loc) const
{
    // TRACE_IN
    int col = loc.getX();
    int row = loc.getY();

    if (row >= ROWS || row < 0)
    {
        return false;
    }
    if (col < 0 || col >= COLS)
    {
        return false;
    }
    return isFree(loc);
}

void Grid::createAgent(int i)
{
    TRACE_IN
    Location loc = randomFreeLocation();
    shared_ptr<Agent> agent = make_shared<Agent>(this, i, loc);
    LDEBUG("created " << *agent);
    agents.push_back(agent);
    objects[loc.getX()][loc.getY()] = static_pointer_cast<GridObject>(agent);
}

void Grid::createHole()
{
    TRACE_IN
    Location loc = randomFreeLocation();
    shared_ptr<Hole> hole = make_shared<Hole>(loc);
    LDEBUG("created " << *hole);
    holes.push_back(hole);
    objects[loc.getX()][loc.getY()] = static_pointer_cast<GridObject>(hole);
}

void Grid::createTile()
{
    TRACE_IN
    Location loc = randomFreeLocation();
    int score = RND(5) + 1;
    shared_ptr<Tile> tile = make_shared<Tile>(loc, score);
    LDEBUG("created " << *tile);
    tiles.push_back(tile);
    objects[loc.getX()][loc.getY()] = static_pointer_cast<GridObject>(tile);
}

void Grid::createObstacle()
{
    TRACE_IN
    Location loc = randomFreeLocation();
    shared_ptr<Obstacle> obst = make_shared<Obstacle>(loc);
    LDEBUG("created " << *obst);
    objects[loc.getX()][loc.getY()] = obst;
}

shared_ptr<Hole> Grid::getClosestHole(const Location &start) const
{
    TRACE_IN
    int minDist = INT_MAX;
    shared_ptr<Hole> best = NULL;
    for (shared_ptr<Hole> hole : holes)
    {
        int dist = hole->getLocation().distance(start);
        if (dist < minDist)
        {
            minDist = dist;
            best = hole;
        }
    }
    return best;
}

shared_ptr<Tile> Grid::getClosestTile(const Location &start) const
{
    TRACE_IN
    int minDist = INT_MAX;
    shared_ptr<Tile> best = NULL;
    for (shared_ptr<Tile> tile : tiles)
    {
        int dist = tile->getLocation().distance(start);
        if (dist < minDist)
        {
            minDist = dist;
            best = tile;
        }
    }
    return best;
}

shared_ptr<GridObject> Grid::findAgent(Agent *a)
{
    for (shared_ptr<Agent> agent : agents)
    {
        if (agent.get() == a)
        {
            return static_pointer_cast<GridObject>(agent);
        }
    }
    return NULL;
}

void Grid::move(Agent *o, const Location &from, const Location &to)
{
    shared_ptr<GridObject> agent = findAgent(o);
    objects[from.getX()][from.getY()].reset();
    objects[to.getX()][to.getY()].swap(agent);
}

bool Grid::pickTile(shared_ptr<Tile> tile)
{
    TRACE_IN
    vector<shared_ptr<Tile>>::iterator it = std::find(tiles.begin(), tiles.end(), tile);
    if (it != tiles.end())
    {
        tiles.erase(it);
        createTile(); // create a new tile
    }
    return it != tiles.end();
}

int Grid::dumpTile(shared_ptr<Tile> tile, shared_ptr<Hole> hole)
{
    TRACE_IN
    vector<shared_ptr<Hole>>::iterator it = std::find(holes.begin(), holes.end(), hole);
    if (it != holes.end())
    {
        holes.erase(it);
        createHole(); // and a new hole
        return tile->getScore();
    }
    return -1;
}

void Grid::update()
{
    TRACE_IN
    for (shared_ptr<Agent> agent : agents)
    {
        agent->update();
    }
    printGrid();
}

shared_ptr<GridObject> Grid::getObject(int col, int row) const
{
    if (row >= ROWS || row < 0)
    {
        cerr << " row " << row << " out of bounds" << endl;
        return NULL;
    }
    if (col < 0 || col >= COLS)
    {
        cerr << " col " << col << " out of bounds" << endl;
        return NULL;
    }
    return objects[col][row];
}

void Grid::printGrid() const
{
#ifdef DEBUG
    TRACE_IN
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLS; x++)
        {
            shared_ptr<GridObject> o = getObject(x, y);
            if (o != NULL)
            {
                switch (o->getType())
                {
                case AGENT:
                    cout << "A";
                    break;
                case HOLE:
                    cout << "O";
                    break;
                case TILE:
                    cout << "T";
                    break;
                case OBSTACLE:
                    cout << "#";
                    break;
                default:
                    cout << o->getType();
                }
            }
            else
            {
                cout << ".";
            }
        }
        cout << endl;
    }
#endif
}

const vector<shared_ptr<Agent>> Grid::getAgents() const
{
    TRACE_IN
    return agents;
}
