#include "path.hpp"

bool hasLoop(std::vector<Location> path, Location nextLocation)
{
    for (auto l : path)
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
    auto last = list.at(0);
    list.erase(list.begin());
    for (auto location : list)
    {
        auto dir = last.getDirection(location);
        path.push_back(dir);
        last = location;
    }
    return path;
}

void generateNext(Grid *grid, Location to, std::vector<Location> path, std::map<int, std::vector<Location>> &q, direction dir)
{
    const Location last = path.back();
    auto nextLocation = last.nextLocation(dir);
    if (grid->possibleMove(last, dir) || nextLocation == to)
    {
        cout << "valid direction " << dir << endl;
        std::vector newPath = std::vector(path);
        if (!hasLoop(newPath, nextLocation))
        {
            newPath.push_back(nextLocation);
            int cost = newPath.size() + nextLocation.distance(to);
            q.insert({cost, newPath});
            cout << "direction " << dir << endl;
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
        auto path = q.begin()->second;
        q.erase(q.begin()->first);
        auto last = path[path.size() - 1];
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
