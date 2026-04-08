#ifndef SOLVER_ASTAR_H
#define SOLVER_ASTAR_H

#include "maze.h"
#include <vector>
#include <utility>

using namespace std;

vector<pair<int,int>> solveAStar(Maze &maze);

#endif