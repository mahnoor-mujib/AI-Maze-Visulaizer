#include "solver_astar.h"
#include <queue>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <climits>

using namespace std;

struct Node {
    int x, y;
    int g, f;
};

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

vector<pair<int,int>> solveAStar(Maze &maze) {

    srand(time(0));

    int n = maze.rows, m = maze.cols;

    vector<vector<int>> g(n, vector<int>(m, INT_MAX));
    vector<vector<pair<int,int>>> parent(n, vector<pair<int,int>>(m, {-1,-1}));

    auto cmp = [](Node a, Node b) {
        return a.f > b.f;
    };

    priority_queue<Node, vector<Node>, decltype(cmp)> pq(cmp);

    int sx = 1, sy = 1;
    int gx = n - 2, gy = m - 2;

    g[sx][sy] = 0;
    pq.push({sx, sy, 0, heuristic(sx, sy, gx, gy)});

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while (!pq.empty()) {

        Node cur = pq.top();
        pq.pop();

        if (cur.x == gx && cur.y == gy)
            break;

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (!maze.isValid(nx, ny)) continue;

            int ng = cur.g + 1;

            int nf = ng + heuristic(nx, ny, gx, gy);

            // 🔥 RANDOM TIE BREAKING
            if (ng < g[nx][ny] ||
               (ng == g[nx][ny] && (rand() % 2))) {

                g[nx][ny] = ng;
                parent[nx][ny] = {cur.x, cur.y};
                pq.push({nx, ny, ng, nf});
            }
        }
    }

    vector<pair<int,int>> path;

    int x = gx, y = gy;

    while (x != -1 && y != -1) {
        path.push_back({x, y});
        auto p = parent[x][y];
        x = p.first;
        y = p.second;
    }

    reverse(path.begin(), path.end());

    return path;
}