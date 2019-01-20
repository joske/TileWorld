#ifndef __AGENT_HPP__
#define __AGENT_HPP__

#include <iostream>
#include "location.hpp"
#include "gridobjects.hpp"
#include "searchstrategy.hpp"

using namespace std;

enum State {IDLE=0, FIND_TILE=1, FIND_HOLE=2, MOVE_TO_TILE=3, MOVE_TO_HOLE=4};
class Grid;

class Agent : public GridObject {
    public:
        Agent(Grid* g, int pid, int x, int y) : GridObject(x, y, AGENT) {score = 0; id = pid; state = IDLE; grid = g;}

        int getId();
        void update();

    private:
        Grid* grid;
        int id;
        int score;
        State state;
        Tile* tile;
        Hole* hole;

        void idle();
        void findTile();
        void findHole();
        void moveToTile();
        void moveToHole();
        direction getNextLocalMove(Location from, Location to);
};
#endif
