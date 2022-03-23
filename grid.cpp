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

void drawMaze(bool drawing) {
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        int row = i / WIDTH;
        int col = i % WIDTH;
        int screenX = col * cellWidth;
        int screenY = row * cellWidth;
        if (i == 0)
            DrawRectangle(screenX, screenY, cellWidth, cellHeight, ORANGE);
        else if (drawing) {
            if (visited[i])
                DrawRectangle(screenX, screenY, cellWidth, cellHeight, BLUE);
        }
        else {
            if (!grid[i])
                DrawRectangle(screenX, screenY, cellWidth, cellHeight, WHITE);
        }
    }
}

int removeWall(int a, int b, bool *grid) {
    if (abs(b - a) == 2) {
        if (b > a) {
            grid[b - 1] = 0;
            return b - 1;
        }
        else {
            grid[a - 1] = 0;
            return a - 1;
        }
    }
    if (abs(b - a) == WIDTH * 2) {
        if (b > a) {
            grid[b - WIDTH] = 0;
            return b - WIDTH;
        }
        else {
            grid[a - WIDTH] = 0;
            return a - WIDTH;
        }
    }
    return 1;
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


int main() {
    for (int i = 0; i < HEIGHT * WIDTH; i++)
        if (i % 2 != 0 || (i / WIDTH) % 2 != 0)
            grid[i] = 1;
    // fill board with walls

    memset(visited, false,
            HEIGHT * WIDTH * sizeof(int)); // set all elements to unvisited

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator");
    SetTargetFPS(60);

    srand(time(0));
    visited[0] = true; // mark beginning as visited
    stck.push(0);

    bool running = true;
    while (!WindowShouldClose()) {
        if (!stck.empty()) {
            int current = stck.top();
            stck.pop();
            vector<int> currNeighbours = getNeighbours(current);

            if (currNeighbours.size()) {
                stck.push(current);
                int next = currNeighbours[rand() % currNeighbours.size()];
                int wall = removeWall(current, next, grid);
                visited[wall] = true;
                visited[next] = true;
                stck.push(next);
            }
        } else {
            running = false;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        drawMaze(running);
        EndDrawing();
    }
    return EXIT_SUCCESS;
}
