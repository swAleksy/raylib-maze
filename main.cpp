#include "raylib.h"

#include "Grid.h"
#include "MazeGenerator.h"
#include <ctime>
#include <cstdlib>

#define SCREEN_WIDTH    1000
#define SCREEN_HEIGHT   1000
#define CELL_SIZE       30

#define MENU_WIDTH      200

#define GRID_AREA_WIDTH  (SCREEN_WIDTH - MENU_WIDTH)
#define GRID_AREA_HEIGHT SCREEN_HEIGHT

#define MARGIN_X 25
#define MARGIN_Y 50

#define GRID_WIDTH  ((GRID_AREA_WIDTH - MARGIN_X * 2) / CELL_SIZE)
#define GRID_HEIGHT ((GRID_AREA_HEIGHT - MARGIN_Y * 2) / CELL_SIZE)

#define OFFSET_X (MENU_WIDTH + (GRID_AREA_WIDTH - GRID_WIDTH * CELL_SIZE) / 2)
#define OFFSET_Y ((GRID_AREA_HEIGHT - GRID_HEIGHT * CELL_SIZE) / 2)

void DrawGrid(const Grid& grid);

int main(void) {
    srand(time(NULL));
    Grid grid(GRID_WIDTH, GRID_HEIGHT);
    MazeGenerator generator(grid, DFS);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze");
    SetTargetFPS(100);

    while (!WindowShouldClose()) {
        if (!generator.finished()) generator.step();

        if (IsKeyPressed(KEY_SPACE)) generator.complete();

        if (IsKeyPressed(KEY_R)) {
            MazeMode cm = (generator.getCurrentMode() == DFS) ? PRIMS : DFS;
            generator.reset(cm);
        }

        BeginDrawing();
            ClearBackground({15, 15, 25, 255});

            DrawRectangle(0, 0, MENU_WIDTH, SCREEN_HEIGHT, {30, 30, 40, 255}); // panel bg
            DrawText("DFS / PRIMS", 10, 50, 20, WHITE);
            DrawText("[R] Reset", 10, 100, 16, GRAY);


            DrawGrid(grid);

            int cur = generator.getCurrentCell();
            if (cur != -1 && generator.getCurrentMode() == DFS) {
                auto [cx, cy] = grid.coords(cur);
                DrawRectangle(OFFSET_X + cx*CELL_SIZE, OFFSET_Y + cy*CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
            }

            else if (cur != -1 && generator.getCurrentMode() == PRIMS) {
                std::vector<int> frontier = generator.getFrontier();
                for (int idx : frontier){
                    auto [x, y] = grid.coords(idx);
                    DrawRectangle(OFFSET_X + x*CELL_SIZE, OFFSET_Y + y*CELL_SIZE, CELL_SIZE, CELL_SIZE, {255, 100, 150, 255});
                }
            }


        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void DrawGrid(const Grid& grid) {
    for (size_t i = 0; i < grid.data.size(); i++){
        auto [x,y] = grid.coords(i);
        int px = OFFSET_X + x * CELL_SIZE;
        int py = OFFSET_Y + y * CELL_SIZE;

        if (grid.data[i].visited) {
            DrawRectangle(px, py, CELL_SIZE, CELL_SIZE, {30, 30, 50, 255});
        }
        if (grid.data[i].walls.top) DrawLine(px, py, px + CELL_SIZE, py, {200, 200, 220, 255});
        if (grid.data[i].walls.bott) DrawLine(px, py + CELL_SIZE, px + CELL_SIZE, py + CELL_SIZE, {200, 200, 220, 255});
        if (grid.data[i].walls.left) DrawLine(px, py, px, py + CELL_SIZE, {200, 200, 220, 255});
        if (grid.data[i].walls.right) DrawLine(px + CELL_SIZE, py, px + CELL_SIZE, py + CELL_SIZE, {200, 200, 220, 255});
    }
}
