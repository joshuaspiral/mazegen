#include "config.h"
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

typedef struct {
    bool north;
    bool east;
    bool south;
    bool west;
} Walls;

int cellWidth = SCREEN_WIDTH / WIDTH;
int cellHeight = SCREEN_HEIGHT / HEIGHT;

Walls wallArray[HEIGHT * WIDTH];
int visited[HEIGHT * WIDTH];
stack<int> stck;

void drawLines(int n, Walls walls) {
    int r = n / WIDTH;
    int c = n % WIDTH;
    int screenX = c * cellWidth;
    int screenY = r * cellHeight;
    if (walls.east)
        DrawLine(screenX + cellWidth, screenY, screenX + cellWidth,
                screenY + cellHeight, BLACK);
    if (walls.south)
        DrawLine(screenX, screenY + cellHeight, screenX + cellWidth,
                screenY + cellHeight, BLACK);
}

void drawCells(bool drawing) {
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        int r = i / WIDTH;
        int c = i % WIDTH;
        double screenX = c * cellWidth;
        double screenY = r * cellHeight;
        if (visited[i] && drawing)
            DrawRectangle(screenX, screenY + 1, cellWidth, cellHeight,
                    BLUE);
        drawLines(i, wallArray[i]);
    }
}
void removeWall(int a, int b, Walls *aWalls, Walls *bWalls) {
    if (b - a == 1) {
        aWalls->east = false;
        bWalls->west = false;
    } else if (b - a == -1) {
        aWalls->west = false;
        bWalls->east = false;
    } else if (b - a == WIDTH) {
        aWalls->south = false;
        bWalls->north = false;
    } else if (b - a == -WIDTH) {
        aWalls->north = false;
        bWalls->south = false;
    }
}

vector<int> getNeighbours(int n) {
    int r = n / WIDTH;
    int c = n % WIDTH;
    vector<int> neighbours;
    if (r - 1 >= 0)
        if (!visited[n - WIDTH])
            neighbours.push_back(n - WIDTH);
    if (r + 1 < HEIGHT)
        if (!visited[n + WIDTH])
            neighbours.push_back(n + WIDTH);
    if (c + 1 < WIDTH)
        if (!visited[n + 1])
            neighbours.push_back(n + 1);
    if (c - 1 >= 0)
        if (!visited[n - 1])
            neighbours.push_back(n - 1);
    return neighbours;
}

int main() {
    memset(visited, false,
            HEIGHT * WIDTH * sizeof(int)); // set all elements to unvisited

    for (int i = 0; i < HEIGHT * WIDTH; i++)
        wallArray[i] = Walls{1, 1, 1, 1};

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator");
    SetTargetFPS(1000);
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

                std::random_shuffle(currNeighbours.begin(), currNeighbours.end());
                int next = currNeighbours[0];
                removeWall(current, next, &wallArray[current], &wallArray[next]);
                visited[next] = true;
                stck.push(next);
            }
        } else
            running = false;

        BeginDrawing();
        ClearBackground(WHITE);
        drawCells(running);
        EndDrawing();
    }
    return EXIT_SUCCESS;
}
