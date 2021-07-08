#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <array>
#include <memory>
#include "agent.hpp"
#include "gridobjects.hpp"

#define COLS 20
#define ROWS 20

using namespace std;

class Grid
{
public:
    Grid(int pnumAgents, int pnumTiles, int pnumHoles, int pnumObst)
    {
        numAgents = pnumAgents;
        numTiles = pnumTiles;
        numHoles = pnumHoles;

        for (int i = 0; i < COLS; i++)
        {
            for (int j = 0; j < ROWS; j++)
            {
                objects[i][j] = NULL;
            }
        }
        for (int i = 0; i < numAgents; i++)
        {
            createAgent(i);
        }
        for (int i = 0; i < numTiles; i++)
        {
            createTile();
        }
        for (int i = 0; i < numHoles; i++)
        {
            createHole();
        }
        for (int i = 0; i < pnumObst; i++)
        {
            createObstacle();
        }
    }
    const vector<shared_ptr<Agent>> getAgents() const;
    void start();
    void update();
    shared_ptr<GridObject> getObject(int x, int y) const;
    shared_ptr<Hole> getClosestHole(const Location &start) const;
    shared_ptr<Tile> getClosestTile(const Location &start) const;
    bool possibleMove(const Location &from, const direction m) const;
    bool isFree(const Location &loc) const;
    bool allowedLocation(const Location &loc) const;
    void move(Agent *o, const Location &from, const Location &to);
    bool pickTile(shared_ptr<Tile> tile);
    int dumpTile(shared_ptr<Tile> tile, shared_ptr<Hole> hole);

private:
    int numAgents;
    int numHoles;
    int numTiles;
    vector<shared_ptr<Agent>> agents;
    vector<shared_ptr<Hole>> holes;
    vector<shared_ptr<Tile>> tiles;

    shared_ptr<GridObject> objects[COLS][ROWS];

    void createAgent(int id);
    void createTile();
    void createHole();
    void createObstacle();
    Location randomFreeLocation() const;
    void printGrid() const;
    shared_ptr<GridObject> findAgent(Agent *a);
    friend std::ostream &operator<<(std::ostream &strm, const Grid &a)
    {
        return strm << "Grid(agents=" << a.agents.size() << ", tiles="
                    << a.tiles.size() << ", holes=" << a.holes.size() << ")" << endl;
    }
};

#endif
