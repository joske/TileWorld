#include "astar.hpp"
#include <queue>
#include <vector>
#include <set>
#include <functional>
#include "main.hpp"

template <class T>
void checkNeighbor(Grid *grid, T &openList, std::vector<Node> &openSet, std::vector<Node> &closedList, direction d, Node &current, Location &from, Location &to)
{
    Location nextLoc = current.location.nextLocation(d);
    if (nextLoc == to || grid->possibleMove(current.location, d))
    {
        int h = nextLoc.distance(to);
        LDEBUG("next location to examine " << nextLoc);
        int g = from.distance(nextLoc);
        std::vector<Location> path;
        path.insert(path.begin(), current.path.begin(), current.path.end());
        path.insert(path.end(), nextLoc);
        Node child = Node(nextLoc, path, g + h);        
        if (std::find(closedList.begin(), closedList.end(), child) == closedList.end())
        {
            for (Node n : openSet)
            {
                if (n.location == child.location && n.fscore < child.fscore)
                {
                    LDEBUG("better node exists with score " << n.fscore);
                    return;
                }
            }
            LDEBUG("add child with fscore " << child.fscore);
            // child is not in openList
            openList.push(child);
            openSet.insert(openSet.end(), child);
        }
    }
}

std::vector<Location> astar(Grid *grid, Location from, Location to)
{
    LDEBUG("finding path from " << from << " to " << to);
    auto cmp = [](Node left, Node right)
    { return left.fscore > right.fscore; };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> openList(cmp);
    std::vector<Node> openSet;
    std::vector<Node> closedList;
    Node fromNode = Node(from, std::vector<Location>(), 0);
    openList.push(fromNode);
    openSet.insert(openSet.end(), fromNode);
    while (!openList.empty())
    {
        Node current = openList.top();
        openList.pop();
        openSet.erase(std::find(openSet.begin(), openSet.end(), current));
        LDEBUG("current=" << current.location);
        if (current.location == to)
        {
            // arrived
            LDEBUG("found path");
            return current.path;
        }
        closedList.insert(closedList.end(), current);
        checkNeighbor(grid, openList, openSet, closedList, direction::UP, current, from, to);
        checkNeighbor(grid, openList, openSet, closedList, direction::LEFT, current, from, to);
        checkNeighbor(grid, openList, openSet, closedList, direction::DOWN, current, from, to);
        checkNeighbor(grid, openList, openSet, closedList, direction::RIGHT, current, from, to);
    }
    return std::vector<Location>();
}