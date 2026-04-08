#include "maze.h"
#include <queue>
#include <omp.h>
#include <vector>
#include <iostream>

using namespace std;

struct Cell {
    int x, y;
};

bool solveOMP(Maze &maze) {

    int n = maze.rows;

    vector<vector<bool>> visited(n, vector<bool>(n, false));

    queue<Cell> q;
    q.push({0,0});
    visited[0][0] = true;

    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};

    while(!q.empty()){

        Cell cur = q.front();
        q.pop();

        if(cur.x == n-1 && cur.y == n-1){
            cout << "Solved with OpenMP BFS\n";
            return true;
        }

        #pragma omp parallel for
        for(int i=0;i<4;i++){
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if(maze.isValid(nx,ny) && !visited[nx][ny]){
                #pragma omp critical
                {
                    if(!visited[nx][ny]){
                        visited[nx][ny] = true;
                        q.push({nx,ny});
                    }
                }
            }
        }
    }

    return false;
}