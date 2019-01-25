#include "agent.hpp"
#include "grid.hpp"
#include <limits.h>
#include "main.hpp"

int Agent::getId() {
    return id;
}

int Agent::getScore() {
    return score;
}

void Agent::update() {
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
}

void Agent::idle() {
    state = FIND_TILE;
}

void Agent::findTile() {
    TRACE_IN
    tile = grid->getClosestTile(loc);
    cout << *this << " found tile " << *tile << endl;
    state = MOVE_TO_TILE;
}

void Agent::findHole() {
    TRACE_IN
    ;
    hole = grid->getClosestHole(loc);
    cout << *this << " found hole " << *hole << endl;
    state = MOVE_TO_HOLE;
}

void Agent::moveToTile() {
    TRACE_IN
    // maybe another tile is now closer?
    Tile* other = grid->getClosestTile(loc);
    if (other != NULL) {
        if (other->getLocation().distance(loc)
                < tile->getLocation().distance(loc)) {
            // indeed closer, move to that one instead
            tile = other;
        }
    }
    if (tile != NULL && tile == grid->getObject(tile->getX(), tile->getY())) {
        direction m = getNextLocalMove(this->getLocation(), tile->getLocation());
        Location newLoc = loc.nextLocation(m);
        grid->move(loc, newLoc);
	setLocation(newLoc);
        if (newLoc == tile->getLocation()) {
            // we are there, pick the tile
            bool tileStilThere = grid->pickTile(tile);
            if (tileStilThere) {
                gotTile = true;
                state = FIND_HOLE;
            } else {
                tile = NULL;
                state = FIND_TILE;
            }
        }
    } else {
        state = FIND_TILE;
    }
}

void Agent::moveToHole() {
    TRACE_IN
    Hole* other = grid->getClosestHole(loc);
    if (other != NULL) {
        if (other->getLocation().distance(loc)
                < hole->getLocation().distance(loc)) {
            // indeed closer, move to that one instead
            hole = other;
        }
    }
    if (tile != NULL && hole == grid->getObject(hole->getX(), hole->getY())) {
        direction m = getNextLocalMove(this->getLocation(), hole->getLocation());
        cout << *this << " next move " << m << endl;
        Location newLoc = loc.nextLocation(m);
        grid->move(loc, newLoc);
	setLocation(newLoc);
        if (newLoc == hole->getLocation()) {
            // we are there, dump the tile
            int sc = grid->dumpTile(tile, hole);
            if (sc != -1) {
                gotTile = false;
                delete tile;
                delete hole;
                tile = NULL;
                hole = NULL;
                this->score += sc;
                state = FIND_TILE;
            } else {
                // hole disappeared, find a new one
                state = FIND_HOLE;
            }
        }
    } else {
        // hole disappeared, find a new one
        state = FIND_HOLE;
    }
}

direction Agent::getNextLocalMove(Location from, Location to) {
    TRACE_IN
    int r = (rand() % 100) + 1;
    if (r > 80) {
        cout << *this << " random move" << endl;
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

