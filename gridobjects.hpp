/*
 * gridobjects.hpp
 *
 *  Created on: Jan 15, 2019
 *      Author: jos
 */

#ifndef GRIDOBJECTS_HPP_
#define GRIDOBJECTS_HPP_

enum object_type {
    AGENT = 0, HOLE = 1, TILE = 2, OBSTACLE = 3
};

class GridObject {
    public:
        GridObject(const Location& loc, object_type t) :
                loc(loc), type(t) {
        }
        object_type getType() const {
            return type;
        }
        int getX() const {
            return loc.getX();
        }
        int getY() const {
            return loc.getY();
        }
        const Location& getLocation() const {
            return loc;
        }
        void setLocation(const Location& newLoc) {
	    loc = newLoc;
        }

    protected:
	Location loc;
        object_type type;

    friend std::ostream& operator<<(std::ostream &strm, const GridObject &a) {
        return strm << "Object(type=" << a.type << ", " << "x=" << a.getX() << ", y="
                    << a.getY() << ")" << endl;
    }
};

class Tile: public GridObject {
    public:
        Tile(Location& loc, int s) :
                GridObject(loc, TILE) {
            score = s;
        }
        int getScore() const {
            return score;
        }
    private:
        int score;

    friend std::ostream& operator<<(std::ostream &strm, const Tile &a) {
        return strm << "Tile(x=" << a.getX() << ", y=" << a.getY() << ", score=" << a.score << ")" << endl;
    }
};

class Hole: public GridObject {
    public:
        Hole(Location& loc) :
                GridObject(loc, HOLE) {
        }
};

class Obstacle: public GridObject {
    public:
        Obstacle(Location& loc) :
                GridObject(loc, OBSTACLE) {
        }
};

#endif /* GRIDOBJECTS_HPP_ */
