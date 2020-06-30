#include "grid.hpp"
#include "agent.hpp"
#include <iostream>
#include <unistd.h>
#include <limits.h>
#ifndef NOGUI
#include <gtkmm.h>
#endif
#include "main.hpp"

void Grid::start(void) {
    cout << "start" << endl;
#ifdef NOGUI
    while (true) {
        update();
        sleep(1);
    }
#endif
}

Location Grid::randomFreeLocation() const {
    TRACE_IN
    int c = RND(COLS)
    int r = RND(ROWS)
    while (objects[c][r] != NULL) {
        c = RND(COLS)
        r = RND(ROWS)
    }
    return Location(c, r);
}

bool Grid::possibleMove(const Location& from, const direction m) const {
    TRACE_IN
    Location newloc = from.nextLocation(m);
    int col = newloc.getX();
    int row = newloc.getY();

    if (!allowedLocation(newloc)) {
        return false;
    }
    GridObject* o = objects[col][row];
    if (o != NULL) {
        cout << "location " << newloc << " is taken by " << *o << endl;
        return false;
    }
    return true;
}

bool Grid::allowedLocation(const Location& loc) const {
    TRACE_IN
    int col = loc.getX();
    int row = loc.getY();

    if (row >= ROWS || row < 0) {
        cout << " row " << row << " out of bounds" << endl;
        return false;
    }
    if (col < 0 || col >= COLS) {
        cout << " col " << col << " out of bounds" << endl;
        return false;
    }
    return true;
}

void Grid::createAgent(int i) {
    TRACE_IN
    Location loc = randomFreeLocation();
    Agent* agent = new Agent(this, i, loc);
    cout << "created " << *agent << endl;
    agents.push_back(agent);
    objects[loc.getX()][loc.getY()] = agent;
}

void Grid::createHole() {
    TRACE_IN
    Location loc = randomFreeLocation();
    Hole* hole = new Hole(loc);
    cout << "created " << *hole << endl;
    holes.push_back(hole);
    objects[loc.getX()][loc.getY()] = hole;
}

void Grid::createTile() {
    TRACE_IN
    Location loc = randomFreeLocation();
    int score = RND(6)
    Tile* tile = new Tile(loc, score);
    cout << "created " << *tile << endl;
    tiles.push_back(tile);
    objects[loc.getX()][loc.getY()] = tile;
}

void Grid::createObstacle() {
    TRACE_IN
    Location loc = randomFreeLocation();
    Obstacle* obst = new Obstacle(loc);
    cout << "created " << *obst << endl;
    objects[loc.getX()][loc.getY()] = obst;
}

Hole* Grid::getClosestHole(const Location& start) const {
    TRACE_IN
    int minDist = INT_MAX;
    Hole* best = NULL;
    for (Hole* hole : holes) {
        int dist = hole->getLocation().distance(start);
        if (dist < minDist) {
            minDist = dist;
            best = hole;
        }
    }
    return best;
}

Tile* Grid::getClosestTile(const Location& start) const {
    TRACE_IN
    int minDist = INT_MAX;
    Tile* best = NULL;
    for (Tile* tile : tiles) {
        int dist = tile->getLocation().distance(start);
        if (dist < minDist) {
            minDist = dist;
            best = tile;
        }
    }
    return best;
}

void Grid::move(const Location& from, const Location& to) {
    GridObject* o = objects[from.getX()][from.getY()];
    objects[from.getX()][from.getY()] = NULL;
    objects[to.getX()][to.getY()] = o;
}

bool Grid::pickTile(Tile* tile) {
    TRACE_IN
    vector<Tile*>::iterator it = std::find(tiles.begin(), tiles.end(), tile);
    if (it != tiles.end()) {
        tiles.erase(it);
        createTile(); // create a new tile
    }
    return it != tiles.end();
}

int Grid::dumpTile(Tile* tile, Hole *hole) {
    TRACE_IN
    vector<Hole*>::iterator it = std::find(holes.begin(), holes.end(), hole);
    if (it != holes.end()) {
        holes.erase(it);
        createHole(); // and a new hole
        return tile->getScore();
    }
    return -1;
}

void Grid::update() {
    TRACE_IN
    for (Agent* agent : agents) {
        agent->update();
    }
    printGrid();
}

GridObject* Grid::getObject(int col, int row) const {
    if (row >= ROWS || row < 0) {
        cerr << " row " << row << " out of bounds" << endl;
        return NULL;
    }
    if (col < 0 || col >= COLS) {
        cerr << " col " << col << " out of bounds" << endl;
        return NULL;
    }
    return objects[col][row];
}

void Grid::printGrid() const {
#ifdef DEBUG
    TRACE_IN
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            GridObject* o = getObject(x, y);
            if (o != NULL) {
                switch (o->getType()) {
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
            } else {
                cout << ".";
            }
        }
        cout << endl;
    }
#endif
}

const vector<Agent*>& Grid::getAgents() const {
    TRACE_IN
    return agents;
}
