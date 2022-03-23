#include "config.h"
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <math.h>
#include <stack>
#include <string.h>
#include <vector>

class Cell;
using namespace std;

vector<Cell> grid;

int cellWidth = SCREEN_WIDTH / WIDTH;
int cellHeight = SCREEN_HEIGHT / HEIGHT;

class Cell {
    public:
        int row;
        int col;
        int screenX;
        int screenY;
        bool visited = false;
        bool wall = false;

        Cell(int row, int col) {
            this->row = row;
            this->col = col;
            screenX = col * cellWidth;
            screenY = row * cellWidth;
            if (col % 2 != 0 || row % 2 != 0)
                this->wall = true;
        }

        void drawCell(bool drawing) {
            /* if (this->row == 0 && this->col == 0) */
            /*     DrawRectangle(screenX, screenY, cellWidth, cellHeight, ORANGE); */
            if (drawing) {
                if (this->visited)
                    DrawRectangle(screenX, screenY, cellWidth, cellHeight, BLUE);
            } else {
                if (!this->wall)
                    DrawRectangle(screenX, screenY, cellWidth, cellHeight, WHITE);
            }
        }

        int removeWall(int b) {
            int a = this->row * WIDTH + this->col;
            if (abs(b - a) == 2) {
                if (b > a) {
                    grid[b - 1].wall = 0;
                    return b - 1;
                } else {
                    grid[a - 1].wall = 0;
                    return a - 1;
                }
            }
            if (abs(b - a) == WIDTH * 2) {
                if (b > a) {
                    grid[b - WIDTH].wall = 0;
                    return b - WIDTH;
                } else {
                    grid[a - WIDTH].wall = 0;
                    return a - WIDTH;
                }
            }
            cout << "Could not remove wall between " << a << " and " << b << endl;
            return 1;
        }

        vector<Cell> getNeighbours() {
            int n = row * WIDTH + col;
            vector<Cell> neighbours;
            if (row - 2 >= 0)
                if (!grid[n - WIDTH * 2].visited)
                    neighbours.push_back(grid[n - WIDTH * 2]);
            if (row + 2 < HEIGHT)
                if (!grid[n + WIDTH * 2].visited)
                    neighbours.push_back(grid[n + WIDTH * 2]);
            if (col + 2 < WIDTH)
                if (!grid[n + 2].visited)
                    neighbours.push_back(grid[n + 2]);
            if (col - 2 >= 0)
                if (!grid[n - 2].visited)
                    neighbours.push_back(grid[n - 2]);
            return neighbours;
        }
};

void drawMaze(bool drawing, vector<Cell> grid) {
    for (int i = 0; i < HEIGHT * WIDTH; i++)
        grid[i].drawCell(drawing);
}

void aStar(int start, int end, int (*h)(int, int)) {
    vector<int> openSet = {start};
    make_heap(openSet.begin(), openSet.end());
}

void mazeGen(int initial) {
    stack<Cell> stck;
    stck.push(grid[initial]);
    grid[initial].visited = true;

    while (!stck.empty()) {
        Cell current = stck.top();
        stck.pop();
        vector<Cell> currNeighbours = current.getNeighbours();

        if (currNeighbours.size() > 0) {
            stck.push(current);
            Cell next = currNeighbours[rand() % currNeighbours.size()];
            int removedWall = current.removeWall(next.row * WIDTH + next.col);
            grid[removedWall].visited = true;
            grid[next.row * WIDTH + next.col].visited = true;
            stck.push(next);
        }
    }
}
int main() {
    srand(time(0));
    // fill board with walls
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        grid.push_back(Cell(i / WIDTH, i % WIDTH));
    }

    mazeGen(0);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generator");
    SetTargetFPS(600);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);
        drawMaze(false, grid);
        EndDrawing();
    }
    return EXIT_SUCCESS;
}
