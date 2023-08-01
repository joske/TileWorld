#ifndef __AGENT_HPP__
#define __AGENT_HPP__

#include <iostream>
#include <memory>
#include <vector>

#include "./gridobjects.hpp"
#include "./location.hpp"
#include "./searchstrategy.hpp"

using namespace std;

enum State { IDLE = 0, MOVE_TO_TILE = 1, MOVE_TO_HOLE = 2 };
class Grid;

class Agent : public GridObject {
public:
  bool hasTile;
  Agent(Grid *g, int pid, Location &loc) : GridObject(loc, AGENT) {
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
  shared_ptr<Tile> getTile() const { return tile; }
  int getScore() const;

  virtual std::ostream &print(std::ostream &strm) const {
    return strm << "Agent(id=" << id << ", x=" << getX() << ", y=" << getY()
                << ", hasTile=" << hasTile << ", score=" << score
                << ", state=" << state << ")" << endl;
  }

private:
  Grid *grid;
  int id;
  int score;
  State state;
  shared_ptr<Tile> tile;
  shared_ptr<Hole> hole;
  std::vector<Location> path;

  void idle();
  void pickTile();
  void dumpTile();
  void moveToObject(shared_ptr<GridObject> o);
  void moveToTile();
  void moveToHole();
  direction getNextLocalMove(const Location &from, const Location &to);
};
#endif
