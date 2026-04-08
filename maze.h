#pragma once

#include <vector>
using namespace std;

class Maze {
public:
    int rows, cols;
    vector<vector<int>> grid;

    Maze(int n);

    void generate();
    void generateRandom(int r, int c);

    bool isValid(int x, int y);
};