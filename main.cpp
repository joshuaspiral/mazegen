#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <string.h>
#include <string>
#include <vector>
#define HEIGHT 50
#define WIDTH 50
#define SCREEN_HEIGHT 1000
#define SCREEN_WIDTH 1000

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

void drawCell(int n, Walls walls) {
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

void removeWall(int a, int b, Walls *aWalls, Walls *bWalls) {
    if (b - a == 1) {
        aWalls->east = false;
        bWalls->west = false;
    }
    else if (b - a == -1) {
        aWalls->west = false;
        bWalls->east = false;
    }
    else if (b - a == WIDTH) {
        aWalls->south = false;
        bWalls->north = false;
    }
    else if (b - a == -WIDTH) {
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

            std::random_shuffle(currNeighbours.begin(), currNeighbours.end()); 
            int next = currNeighbours[0];
            removeWall(current, next, &wallArray[current], &wallArray[next]);
            visited[next] = true;
            stck.push(next);
        }
    }
}

int main() {
    memset(visited, false,
            HEIGHT * WIDTH * sizeof(int)); // set all elements to unvisited

    for (int i = 0; i < HEIGHT * WIDTH; i++)
        wallArray[i] = Walls {1, 1, 1, 1};

    mazeGen(0);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        for (int i = 0; i < HEIGHT * WIDTH; i++) {
            drawCell(i, wallArray[i]);
        }
        EndDrawing();
    }
    return EXIT_SUCCESS;
}
