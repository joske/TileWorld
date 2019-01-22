#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <array>
#include "agent.hpp"
#include "gridobjects.hpp"

#define COLS 40
#define ROWS 40

using namespace std;

class Grid {
    public:
        Grid(int pnumAgents, int pnumTiles, int pnumHoles) {
            numAgents = pnumAgents;
            numTiles = pnumTiles;
            numHoles = pnumHoles;

            for (int i = 0; i < COLS; i++) {
                for (int j = 0; j < ROWS; j++) {
                    objects[i][j] = NULL;
                }
            }
            for (int i = 0; i < numAgents; i++) {
                createAgent(i);
            }
            for (int i = 0; i < numTiles; i++) {
                createTile();
            }
            for (int i = 0; i < numHoles; i++) {
                createHole();
            }
            for (int i = 0; i < numHoles; i++) {
                createObstacle();
            }
        }
        vector<Agent*> getAgents();
        void start();
        void update();
        GridObject* getObject(int x, int y);
        Hole* getClosestHole(Location start);
        Tile* getClosestTile(Location start);
        bool possibleMove(Location from, direction m);
        bool allowedLocation(Location loc);
        void move(Location from, Location to);
        bool pickTile(Tile* tile);
        int dumpTile(Tile* tile, Hole *hole);
        private:
        int numAgents;
        int numHoles;
        int numTiles;
        vector<Agent*> agents;
        vector<Hole*> holes;
        vector<Tile*> tiles;

        GridObject* objects[COLS][ROWS];

        void createAgent(int id);
        void createTile();
        void createHole();
        void createObstacle();
        Location randomFreeLocation();
        void printGrid();

        friend std::ostream& operator<<(std::ostream &strm, const Grid &a) {
            return strm << "Grid(agents=" << a.agents.size() << ", tiles="
                    << a.tiles.size() << ", holes=" << a.holes.size() << endl;
        }

};

#endif
