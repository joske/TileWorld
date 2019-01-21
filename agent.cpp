#include "agent.hpp"
#include "grid.hpp"
#include <limits.h>
#include "main.hpp"

int Agent::getId() {
    return id;
}

void Agent::update() {
    TRACE_IN
    switch (state) {
        case IDLE:
            idle();
            break;
        case FIND_HOLE:
            findHole();
            break;
        case FIND_TILE:
            findTile();
            break;
        case MOVE_TO_TILE:
            moveToTile();
            break;
        case MOVE_TO_HOLE:
            moveToHole();
            break;
    }
    TRACE_OUT
    ;
}

void Agent::idle() {
    TRACE_IN
    state = FIND_TILE;
}

void Agent::findTile() {
    TRACE_IN
    tile = grid->getClosestTile(Location(x, y));
    cout << *this << " found tile " << *tile << endl;
    state = MOVE_TO_TILE;
}

void Agent::findHole() {
    TRACE_IN
    ;
    hole = grid->getClosestHole(Location(x, y));
    cout << *this << " found hole " << *hole << endl;
    state = MOVE_TO_HOLE;
}

void Agent::moveToTile() {
    TRACE_IN
    if (tile != NULL) {
        direction m = getNextLocalMove(this->getLocation(), tile->getLocation());
        cout << this << " next move " << m << endl;
        Location oldLoc = Location(x, y);
        Location newLoc = oldLoc.nextLocation(m);
        grid->move(oldLoc, newLoc);
        if (newLoc == tile->getLocation()) {
            // we are there, pick the tile
            grid->pickTile(tile);
            gotTile = true;
            state = FIND_HOLE;
        }
    }
}

void Agent::moveToHole() {
    TRACE_IN
    if (hole != NULL) {
        direction m = getNextLocalMove(this->getLocation(), hole->getLocation());
        cout << *this << " next move " << m << endl;
        Location oldLoc = Location(x, y);
        Location newLoc = oldLoc.nextLocation(m);
        grid->move(oldLoc, newLoc);
        if (newLoc == hole->getLocation()) {
            // we are there, dump the tile
            int sc = grid->dumpTile(tile, hole);
            tile = NULL;
            gotTile = false;
            delete tile;
            hole = NULL;
            delete hole;
            this->score += sc;
            state = FIND_TILE;
        }
    }
}

direction Agent::getNextLocalMove(Location from, Location to) {
    TRACE_IN
    int r = (rand() % 100) + 1;
    if (r > 80) {
        cout << "random move" << endl;
        r = (rand() % 4);
        direction d = static_cast<direction>(r);
        while (!grid->possibleMove(from, d)) {
            r = (rand() % 4);
            d = static_cast<direction>(r);
        }
        return d;
    }
    int min_dist = INT_MAX;
    direction best_move;
    for (int dir = 0; dir < 4; dir++) {
        direction d = static_cast<direction>(dir);
        Location l = from.nextLocation(d);
        if (l == to) {
            // we have arrived
            best_move = d;
            break;
        } else if (grid->possibleMove(from, d)) {
            int dist = l.distance(to);
            if (dist < min_dist) {
                min_dist = dist;
                best_move = d;
            }
        }
    }
    return best_move;
}

