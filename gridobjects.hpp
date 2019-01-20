/*
 * gridobjects.hpp
 *
 *  Created on: Jan 15, 2019
 *      Author: jos
 */

#ifndef GRIDOBJECTS_HPP_
#define GRIDOBJECTS_HPP_

enum object_type {AGENT = 0, HOLE = 1, TILE = 2, OBSTACLE=3};

class GridObject {
	public:
		GridObject(int xx, int yy, object_type t) : x(xx), y(yy), type(t) {}
		GridObject(const GridObject &other) {
			x = other.x;
			y = other.y;
			type = other.type;
		}
		~GridObject() {}
		object_type getType() {	return type; }
		int getX() { return x;}
		int getY() { return y;}
		Location getLocation() {
			return Location(x, y);
		}
        void setLocation(const int x, const int y) {
        	this->x = x;
        	this->y = y;
        }
	protected:
		int x;
		int y;
		object_type type;
	private:
		friend std::ostream& operator<<(std::ostream &strm, const GridObject &a) {
			return strm << "Object(type=" << a.type << ", " << "x=" << a.x << ", y=" << a.y	<< ")" << endl;
		}
};

class Tile : public GridObject {
	public:
		Tile(int xx, int yy, int s) : GridObject(xx, yy, TILE) {score = s;}
		int getScore() { return score; }
	private:
		int score;
};

class Hole : public GridObject {
	public:
		Hole(int xx, int yy) : GridObject(xx, yy, HOLE) {}
};

class Obstacle : public GridObject {
	public:
		Obstacle(int xx, int yy) : GridObject(xx, yy, OBSTACLE) {}
};

#endif /* GRIDOBJECTS_HPP_ */
