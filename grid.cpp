#include "grid.hpp"
#include "agent.hpp"
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <gtkmm.h>
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

Location Grid::randomFreeLocation() {
    TRACE_IN
    int c = RND(COLS)
    int r = RND(ROWS)
    while (objects[c][r] != NULL) {
        c = RND(COLS)
        r = RND(ROWS)
    }
    return Location(c, r);
}

bool Grid::possibleMove(Location from, direction m) {
    TRACE_IN
    Location newloc = from.nextLocation(m);
    int col = newloc.x;
    int row = newloc.y;

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

bool Grid::allowedLocation(Location loc) {
    TRACE_IN
    int col = loc.x;
    int row = loc.y;

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
    Agent* agent = new Agent(this, i, loc.x, loc.y);
    cout << "created " << *agent << endl;
    agents.push_back(agent);
    objects[loc.x][loc.y] = agent;
}

void Grid::createHole() {
    TRACE_IN
    Location loc = randomFreeLocation();
    Hole* hole = new Hole(loc.x, loc.y);
    cout << "created " << *hole << endl;
    holes.push_back(hole);
    objects[loc.x][loc.y] = hole;
}

void Grid::createTile() {
    TRACE_IN
    Location loc = randomFreeLocation();
    int score = RND(6)
    Tile* tile = new Tile(loc.x, loc.y, score);
    cout << "created " << *tile << endl;
    tiles.push_back(tile);
    objects[loc.x][loc.y] = tile;
}

void Grid::createObstacle() {
    TRACE_IN
    Location loc = randomFreeLocation();
    Obstacle* obst = new Obstacle(loc.x, loc.y);
    cout << "created " << *obst << endl;
    objects[loc.x][loc.y] = obst;
}

Hole* Grid::getClosestHole(Location start) {
    TRACE_IN
    int minDist = INT_MAX;
    Hole* best = NULL;
    vector<Hole*>::iterator hole = holes.begin();
    while (hole != holes.end()) {
        Location holeLocation = Location((*hole)->getX(), (*hole)->getY());
        int dist = holeLocation.distance(start);
        if (dist < minDist) {
            minDist = dist;
            best = *hole;
        }
        hole++;
    }
    return best;
}

Tile* Grid::getClosestTile(Location start) {
    TRACE_IN
    int minDist = INT_MAX;
    Tile* best = NULL;
    vector<Tile*>::iterator tile = tiles.begin();
    while (tile != tiles.end()) {
        Location holeLocation = Location((*tile)->getX(), (*tile)->getY());
        int dist = holeLocation.distance(start);
        if (dist < minDist) {
            minDist = dist;
            best = *tile;
        }
        tile++;
    }
    return best;
}

void Grid::move(Location from, Location to) {
    GridObject* o = objects[from.x][from.y];
    o->setLocation(to.x, to.y);
    objects[to.x][to.y] = o;
    objects[from.x][from.y] = NULL;
}

bool Grid::pickTile(Tile* tile) {
    TRACE_IN
    vector<Tile*>::iterator it = tiles.begin();
    while (it != tiles.end()) {
        if ((*it)->getLocation() == tile->getLocation()) {
            tiles.erase(it);
            createTile(); // create a new tile
            return true;
        }
        it++;
    }
    return false;
}

int Grid::dumpTile(Tile* tile, Hole *hole) {
    TRACE_IN
    vector<Hole*>::iterator it = holes.begin();
    while (it != holes.end()) {
        if ((*it)->getLocation() == hole->getLocation()) {
            holes.erase(it);
            createHole(); // and a new hole
            return tile->getScore();
        }
        it++;
    }
    return -1;
}

void Grid::update() {
    TRACE_IN
    vector<Agent*>::iterator agent = agents.begin();
    while (agent != agents.end()) {
        Location oldLoc = (*agent)->getLocation();
        (*agent)->update();
        Location newLoc = (*agent)->getLocation();
        objects[newLoc.x][newLoc.y] = *agent;
        agent++;
    }
    printGrid();
}

GridObject* Grid::getObject(int c, int r) {
    return objects[c][r];
}

void Grid::printGrid() {
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
}
vector<Agent*> Grid::getAgents() {
    TRACE_IN
    return agents;
}
