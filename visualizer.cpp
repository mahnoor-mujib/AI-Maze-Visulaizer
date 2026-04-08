#include "visualizer.h"
#include "maze.h"
#include "solver_astar.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

using namespace std;

#define CELL 25

bool paused = false;
int speed = 100;

// ---------------- helper ----------------
bool isInside(int x, int y, SDL_Rect r) {
    return x > r.x && x < r.x + r.w && y > r.y && y < r.y + r.h;
}

// ---------------- text rendering ----------------
void drawText(SDL_Renderer* renderer, TTF_Font* font,
              const string &text, int x, int y) {

    SDL_Color color = {0, 0, 0};

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst = {x, y, surface->w, surface->h};

    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

// ---------------- MAIN VISUALIZER ----------------
void runVisualizer(Maze &maze, vector<pair<int,int>> &path) {

    int rows = maze.rows;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow(
        "AI Maze Solver",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        rows * CELL,
        rows * CELL + 100,
        0
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    TTF_Font* font = TTF_OpenFont("arial.ttf", 18);
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 18);
    }

    SDL_Rect startBtn = {10,  rows * CELL + 10, 80, 30};
    SDL_Rect pauseBtn = {100, rows * CELL + 10, 80, 30};
    SDL_Rect resetBtn = {190, rows * CELL + 10, 80, 30};

    bool running = true;
    SDL_Event e;

    int step = 0;
    paused = true;   // 🔥 IMPORTANT: start in STOP state

    // ---------------- SAFE RESET ----------------
    auto safeReset = [&]() {

        SDL_PumpEvents(); // clears stuck clicks

        maze.generate();
        path = solveAStar(maze);

        step = 0;
        paused = true;   // 🔥 stop after reset until START pressed

        if (path.empty()) {
            path.push_back({0, 0});
        }
    };

    while (running) {

        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_MOUSEBUTTONDOWN) {

                int x = e.button.x;
                int y = e.button.y;

                // START → always restart properly
                if (isInside(x, y, startBtn)) {
                    if (!path.empty()) {
                        step = 0;        // 🔥 FIX: restart animation
                        paused = false;
                    }
                }

                if (isInside(x, y, pauseBtn)) {
                    paused = true;
                }

                if (isInside(x, y, resetBtn)) {
                    safeReset();
                }
            }
        }

        // ---------------- SAFETY ----------------
        if (path.empty()) {
            safeReset();
        }

        if (step >= (int)path.size()) {
            step = (int)path.size() - 1;
            paused = true;
        }

        // ---------------- DRAW ----------------
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // maze
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < rows; j++) {

                SDL_Rect r = {j * CELL, i * CELL, CELL, CELL};

                if (maze.grid[i][j] == 1)
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                else
                    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 255);

                SDL_RenderFillRect(renderer, &r);
            }
        }

        // path
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        for (int i = 0; i <= step && i < (int)path.size(); i++) {

            SDL_Rect r = {
                path[i].second * CELL,
                path[i].first * CELL,
                CELL, CELL
            };

            SDL_RenderFillRect(renderer, &r);
        }

        // buttons
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRect(renderer, &startBtn);

        SDL_SetRenderDrawColor(renderer, 200, 200, 0, 255);
        SDL_RenderFillRect(renderer, &pauseBtn);

        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        SDL_RenderFillRect(renderer, &resetBtn);

        drawText(renderer, font, "START", 20,  rows * CELL + 50);
        drawText(renderer, font, "PAUSE", 110, rows * CELL + 50);
        drawText(renderer, font, "RESET", 200, rows * CELL + 50);

        SDL_RenderPresent(renderer);

        // animation
        if (!paused && step < (int)path.size() - 1) {
            step++;
            SDL_Delay(speed);
        }
    }

    // cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}