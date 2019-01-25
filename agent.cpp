#include "agent.hpp"
#include "grid.hpp"
#include <limits.h>
#include "main.hpp"

int Agent::getId() const {
    return id;
}

int Agent::getScore() const {
    return score;
}

void Agent::update() {
    switch (state) {
        case IDLE:
            idle();
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
    state = MOVE_TO_TILE;
}

void Agent::moveToTile() {
    TRACE_IN
    // maybe another tile is now closer?
    Tile* otherTile = grid->getClosestTile(loc);
    if (otherTile != NULL) {
        cout << *this << " move to tile " << *otherTile << endl;
        direction m = getNextLocalMove(this->getLocation(), otherTile->getLocation());
        Location newLoc = loc.nextLocation(m);
        grid->move(loc, newLoc);
	setLocation(newLoc);
        if (newLoc == otherTile->getLocation()) {
            // we are there, pick the tile
            bool tileStilThere = grid->pickTile(otherTile);
            if (tileStilThere) {
		tile = otherTile;
                state = MOVE_TO_HOLE;
            }
        }
    }
}

void Agent::moveToHole() {
    TRACE_IN
    Hole* hole = grid->getClosestHole(loc);
    if (hole != NULL && tile != NULL) {
        direction m = getNextLocalMove(this->getLocation(), hole->getLocation());
        cout << *this << " next move " << m << " to hole " << *hole << endl;
        Location newLoc = loc.nextLocation(m);
        grid->move(loc, newLoc);
	setLocation(newLoc);
        if (newLoc == hole->getLocation()) {
            // we are there, dump the tile
            int sc = grid->dumpTile(tile, hole);
            if (sc != -1) {
                delete tile;
                delete hole;
                tile = NULL;
                this->score += sc;
                state = MOVE_TO_TILE;
            }
        }
    }
}

direction Agent::getNextLocalMove(const Location& from, const Location& to) {
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

