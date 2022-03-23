#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <stack>
#include <string.h>
#include <string>
#include <vector>
#define HEIGHT 20
#define WIDTH 20
#define SCREEN_HEIGHT 1000
#define SCREEN_WIDTH 1000

using namespace std;

int cellWidth = SCREEN_WIDTH / WIDTH;
int cellHeight = SCREEN_HEIGHT / HEIGHT;

pair<bool, bool> wallArray[HEIGHT * WIDTH]; // East wall then south wall
int visited[HEIGHT * WIDTH];
stack<int> stck;

void drawCell(int n, pair<bool, bool> walls) {
    int r = n / WIDTH;
    int c = n % WIDTH;
    int screenX = c * cellWidth;
    int screenY = r * cellHeight;
    if (walls.first)
        DrawLine(screenX + cellWidth, screenY, screenX + cellWidth,
                screenY + cellHeight, BLACK);
    if (walls.second)
        DrawLine(screenX, screenY + cellHeight, screenX + cellWidth,
                screenY + cellHeight, BLACK);
}

void removeWall(int n1, int n2, pair<bool, bool> *walls) {
    if (n2 - n1 == 1)
        walls->first = false;
    else if (n2 - WIDTH == n1)
        walls->second = false;
    else {
        cout << "COULD NOT REMOVE WALL" << endl;
        cout << n1 << " " << n2 << endl;
    }
}

vector<int> getNeighbours(int n) {
    vector<int> neighbours;
    if (n - WIDTH >= 0)
        if (!visited[n - WIDTH])
            neighbours.push_back(n - WIDTH);
    if (n + WIDTH < WIDTH * HEIGHT)
        if (!visited[n + WIDTH])
            neighbours.push_back(n + WIDTH);
    if (n + 1 < WIDTH * HEIGHT)
        if (!visited[n + 1])
            neighbours.push_back(n + 1);
    if (n - 1 >= 0)
        if (!visited[n - 1])
            neighbours.push_back(n - 1);
    return neighbours;
}

void mazeGen(int initial) {
    // Initial setup
    visited[initial] = true; // mark initial as visited
    stck.push(initial);

    while (!stck.empty()) {
        int current = stck.top();
        stck.pop();
        vector<int> currNeighbours = getNeighbours(current);

        if (currNeighbours.size()) {
            stck.push(current);

            /* std::random_shuffle(currNeighbours.begin(), currNeighbours.end()); */ // shuffle neighbours
            for (int neighbour : currNeighbours) {
                if (!visited[neighbour]) {
                    removeWall(current, neighbour, &wallArray[current]);
                    visited[neighbour] = true;
                    stck.push(neighbour);
                }
            }
        }
    }
}

int main() {
    memset(visited, false,
            HEIGHT * WIDTH * sizeof(int)); // set all elements to unvisited

    for (int i = 0; i < HEIGHT * WIDTH; i++)
        wallArray[i] = pair<bool, bool>{1, 1};

    mazeGen(0);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator");
    SetTargetFPS(60);

    removeWall(0, 20, &wallArray[0]);
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
