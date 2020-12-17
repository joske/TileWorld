#include "path.hpp"

bool hasLoop(std::vector<Location> path, Location nextLocation)
{
    for (Location l : path)
    {
        if (l == nextLocation)
        {
            return true;
        }
    }
    return false;
}

std::vector<direction> makePath(std::vector<Location> list)
{
    std::vector<direction> path;
    Location last = list.at(0);
    list.erase(list.begin());
    cout << "Path from: "<< last;
    for (Location location : list)
    {
        direction dir = last.getDirection(location);
        cout << " next " << location;
        path.push_back(dir);
        last = location;
    }
    cout << endl;
    return path;
}

void generateNext(Grid *grid, Location to, std::vector<Location> &path, std::map<int, std::vector<Location>> &q, direction dir)
{
    const Location last = path.back();
    Location nextLocation = last.nextLocation(dir);
    if (grid->possibleMove(last, dir) || nextLocation == to)
    {
        std::vector newPath = std::vector(path);
        if (!hasLoop(newPath, nextLocation))
        {
            newPath.push_back(nextLocation);
            int cost = newPath.size() + nextLocation.distance(to);
            q.insert({cost, newPath});
        }
    }
}

std::vector<direction> shortestPath(Grid *grid, Location from, Location to)
{
    std::vector<Location> initial;
    std::map<int, std::vector<Location>> q;
    initial.push_back(from);
    q.insert({0, initial});
    while (!q.empty())
    {
        vector<Location> path = q.begin()->second;
        q.erase(q.begin()->first);
        Location last = path[path.size() - 1];
        if (last == to)
        {
            return makePath(path);
        }
        generateNext(grid, to, path, q, UP);
        generateNext(grid, to, path, q, DOWN);
        generateNext(grid, to, path, q, LEFT);
        generateNext(grid, to, path, q, RIGHT);
    }
    return std::vector<direction>();
}
