#include "astar.hpp"
#include <queue>
#include <vector>
#include <set>

std::vector<direction> makePath(Node *end, Node *start) {
    std::vector<direction> directions;
    Node *current = end;
    Node *parent = current->parent;
    while (current->location != start->location) {
        direction d = parent->location.getDirection(current->location);
        directions.insert(directions.begin(), d);
    }
    return directions;
}

template<class T>
void checkNeighbor(Grid *grid, T& openList, std::set<Node> closedList, direction d, Node current, Location from, Location to) {
    Location nextLoc = current.location.nextLocation(d);
    if (nextLoc == to || grid->possibleMove(current.location, d)) {
        int h = nextLoc.distance(to);
        int g = from.distance(nextLoc);
        Node child = Node(nextLoc, &current, g + h);
        if (!closedList.contains(child)) {
            for (Node n : openList) {
                if (n.location == child.location) {
                    return;
                }
            }
            // child is not in openList
            openList.push(child);
        }
    }
}

std::vector<direction> astar(Grid *grid, Location from, Location to) {
    auto cmp = [](Node left, Node right) { return left.fscore < right.fscore;};
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openList(cmp);
    std::set<Node> closedList;
    Node fromNode = Node(from, NULL, 0);
    openList.push(fromNode);
    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();
        if (current.location == to) {
            // arrived
            return makePath(&current, &fromNode);
        }
        closedList.insert(current);
        checkNeighbor(grid, openList, closedList, direction::UP, current, from, to);
        checkNeighbor(grid, openList, closedList, direction::LEFT, current, from, to);
        checkNeighbor(grid, openList, closedList, direction::DOWN, current, from, to);
        checkNeighbor(grid, openList, closedList, direction::RIGHT, current, from, to);
    }
    return {};
}