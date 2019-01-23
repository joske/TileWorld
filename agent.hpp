#ifndef __AGENT_HPP__
#define __AGENT_HPP__

#include <iostream>
#include "location.hpp"
#include "gridobjects.hpp"
#include "searchstrategy.hpp"

using namespace std;

enum State {
    IDLE = 0, FIND_TILE = 1, FIND_HOLE = 2, MOVE_TO_TILE = 3, MOVE_TO_HOLE = 4
};
class Grid;

class Agent: public GridObject {
    public:
        Agent(Grid* g, int pid, int x, int y) :
                GridObject(x, y, AGENT) {
            score = 0;
            id = pid;
            state = IDLE;
            hole = NULL;
            tile = NULL;
            grid = g;
            gotTile = false;
        }

        int getId();
        void update();
        Tile* getTile() {
            return tile;
        }
        bool hasTile() {
            return gotTile;
        }
        int getScore();

    private:
        Grid* grid;
        int id;
        int score;
        State state;
        Tile* tile;
        bool gotTile;
        Hole* hole;

        void idle();
        void findTile();
        void findHole();
        void moveToTile();
        void moveToHole();
        direction getNextLocalMove(Location from, Location to);

        friend std::ostream& operator<<(std::ostream &strm, const Agent &a) {
            return strm << "Agent(id=" << a.id << ", x=" << a.x << ", y=" << a.y << ", hasTile=" << a.gotTile << ", score=" << a.score << ", state=" << a.state << ")" << endl;
        }

};
#endif
