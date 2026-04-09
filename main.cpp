#include "maze.h"
#include "solver_astar.h"
#include "visualizer.h"

int main() {

    Maze maze(20);        // ✅ FIX: pass size in constructor

    maze.generate();      // ✅ FIX: correct function name

    vector<pair<int,int>> path = solveAStar(maze);

    runVisualizer(maze, path);

    return 0;
}