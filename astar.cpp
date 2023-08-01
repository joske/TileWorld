#include "astar.hpp"
#include "main.hpp"
#include "priorityq.hpp"
#include <functional>
#include <set>
#include <vector>

template <class T>
void checkNeighbor(Grid *grid, T &openList, std::vector<Node> &closedList,
                   direction d, Node &current, Location &from, Location &to) {
  Location nextLoc = current.location.nextLocation(d);
  if (nextLoc == to || grid->possibleMove(current.location, d)) {
    int h = nextLoc.distance(to);
    int g = from.distance(nextLoc);
    std::vector<Location> path;
    path.insert(path.begin(), current.path.begin(), current.path.end());
    path.insert(path.end(), nextLoc);
    Node child = Node(nextLoc, path, g + h);
    if (std::find(closedList.begin(), closedList.end(), child) ==
        closedList.end()) {
      if (!openList.contains(child)) {
        // child is not in openList
        openList.push(child);
      }
    }
  }
}

std::vector<Location> astar(Grid *grid, Location from, Location to) {
  LDEBUG("finding path from " << from << " to " << to);
  auto cmp = [](const Node &left, const Node &right) {
    return left.fscore > right.fscore;
  };
  auto hash = [](const Node &n) {
    return std::hash<int>()(n.location.getX()) ^
           std::hash<int>()(n.location.getY());
  };
  MyPriorityQueue<Node, std::vector<Node>, decltype(cmp), decltype(hash)>
      openList(cmp, hash);
  std::vector<Node> closedList;
  Node fromNode = Node(from, std::vector<Location>(), 0);
  openList.push(fromNode);
  while (!openList.empty()) {
    Node current = openList.pop();
    if (current.location == to) {
      // arrived
      return current.path;
    }
    closedList.insert(closedList.end(), current);
    checkNeighbor(grid, openList, closedList, direction::UP, current, from, to);
    checkNeighbor(grid, openList, closedList, direction::LEFT, current, from,
                  to);
    checkNeighbor(grid, openList, closedList, direction::DOWN, current, from,
                  to);
    checkNeighbor(grid, openList, closedList, direction::RIGHT, current, from,
                  to);
  }
  return std::vector<Location>();
}
