#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <string.h>
#include <string>
#include <vector>
#include "config.h"

using namespace std;
bool grid[HEIGHT * WIDTH];

int cellWidth = SCREEN_WIDTH / WIDTH;
int cellHeight = SCREEN_HEIGHT / HEIGHT;

int visited[HEIGHT * WIDTH];
stack<int> stck;

void drawMaze() {
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        int row = i / WIDTH;
        int col = i % WIDTH;
        int screenX = col * cellWidth;
        int screenY = row * cellWidth;
        if (grid[i])
            DrawRectangle(screenX, screenY, cellWidth, cellHeight, BLACK);
        else
            DrawRectangle(screenX, screenY, cellWidth, cellHeight, WHITE);
    }
}

void removeWall(int a, int b, bool *grid) {
    if (abs(b - a) == 2) {
        if (b > a)
            grid[b - 1] = 0;
        else
            grid[a - 1] = 0;
    }
    if (abs(b - a) == WIDTH * 2) {
        if (b > a)
            grid[b - WIDTH] = 0;
        else
            grid[a - WIDTH] = 0;
    }
}

vector<int> getNeighbours(int n) {
    int r = n / WIDTH;
    int c = n % WIDTH;
    vector<int> neighbours;
    if (r - 2 >= 0)
        if (!visited[n - WIDTH * 2])
            neighbours.push_back(n - WIDTH * 2);
    if (r + 2 < HEIGHT)
        if (!visited[n + WIDTH * 2])
            neighbours.push_back(n + WIDTH * 2);
    if (c + 2 < WIDTH)
        if (!visited[n + 2])
            neighbours.push_back(n + 2);
    if (c - 2 >= 0)
        if (!visited[n - 2])
            neighbours.push_back(n - 2);
    return neighbours;
}

void mazeGen(int initial) {
    srand(time(0));
    // Initial setup
    visited[initial] = true; // mark initial as visited
    stck.push(initial);

    while (!stck.empty()) {
        int current = stck.top();
        stck.pop();
        vector<int> currNeighbours = getNeighbours(current);

        if (currNeighbours.size()) {
            stck.push(current);
            int next = currNeighbours[rand() % currNeighbours.size()];
            removeWall(current, next, grid);
            visited[next] = true;
            stck.push(next);
        }
    }
}

int main() {
    for (int i = 0; i < HEIGHT * WIDTH; i++)
        if (i % 2 != 0 || (i / WIDTH) % 2 != 0)
            grid[i] = 1;
    // fill board with walls

    memset(visited, false,
            HEIGHT * WIDTH * sizeof(int)); // set all elements to unvisited

    mazeGen(0);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator");
    SetTargetFPS(60);


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        drawMaze();
        EndDrawing();
    }
    return EXIT_SUCCESS;
}
