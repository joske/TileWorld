/*
 * gridobjects.hpp
 *
 *  Created on: Jan 15, 2019
 *      Author: jos
 */

#ifndef GRIDOBJECTS_HPP_
#define GRIDOBJECTS_HPP_

#include "location.hpp"
#include <iostream>

enum object_type { AGENT = 0, HOLE = 1, TILE = 2, OBSTACLE = 3 };

class GridObject {
public:
  GridObject(const Location &loc, object_type t) : loc(loc), type(t) {}
  virtual ~GridObject() {}

  object_type getType() const { return type; }
  int getX() const { return loc.getX(); }
  int getY() const { return loc.getY(); }
  const Location &getLocation() const { return loc; }
  void setLocation(const Location &newLoc) { loc = newLoc; }

  virtual std::ostream &print(std::ostream &strm) const {
    return strm << "Object(type=" << type << ", "
                << "x=" << getX() << ", y=" << getY() << ")" << endl;
  }

protected:
  Location loc;
  object_type type;

  friend std::ostream &operator<<(std::ostream &strm, const GridObject &a) {
    return a.print(strm);
  }
};

class Tile : public GridObject {
public:
  Tile(Location &loc, int s) : GridObject(loc, TILE) { score = s; }
  int getScore() const { return score; }

  virtual std::ostream &print(std::ostream &strm) const {
    return strm << "Tile(x=" << getX() << ", y=" << getY()
                << ", score=" << score << ")" << endl;
  }

private:
  int score;
};

class Hole : public GridObject {
public:
  Hole(Location &loc) : GridObject(loc, HOLE) {}
  virtual std::ostream &print(std::ostream &strm) const {
    return strm << "Hole(x=" << getX() << ", y=" << getY() << ")" << endl;
  }
};

class Obstacle : public GridObject {
public:
  Obstacle(Location &loc) : GridObject(loc, OBSTACLE) {}
  virtual std::ostream &print(std::ostream &strm) const {
    return strm << "Obstacle(x=" << getX() << ", y=" << getY() << ")" << endl;
  }
};

#endif /* GRIDOBJECTS_HPP_ */
