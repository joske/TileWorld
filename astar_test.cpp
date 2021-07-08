#include "astar.hpp"
#include "location.hpp"
#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
    Grid grid(0, 0, 0, 0);
    Location from = Location(0, 0);
    Location to = Location(1, 1);
    vector<Location> path = astar(&grid, from, to);
    cout << "path: " << path.size() << endl;
    if (path.size() == 2) {
        cout << "test passed" << endl;
    }
}