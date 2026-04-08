#include "maze.h"
#include <cstdlib>
#include <ctime>

Maze::Maze(int n) {
    rows = cols = n;
    grid.assign(rows, vector<int>(cols, 0));
}

void Maze::generate() {
    generateRandom(rows, cols);
}

void Maze::generateRandom(int r, int c) {

    rows = r;
    cols = c;

    grid.assign(rows, vector<int>(cols, 0));

    srand(time(0));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {

            if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1) {
                grid[i][j] = 1;
            } else {
                grid[i][j] = (rand() % 100 < 30) ? 1 : 0;
            }
        }
    }

    grid[1][1] = 0;
    grid[rows - 2][cols - 2] = 0;
}

bool Maze::isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < rows && y < cols && grid[x][y] == 0;
}