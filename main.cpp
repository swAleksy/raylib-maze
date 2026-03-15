#include "raylib.h"

#include "MazeSolver.h"
#include "Types.h"
#include "Grid.h"
#include "MazeGenerator.h"
#include "Render.h"

#include <ctime>
#include <cstdlib>

#define SCREEN_WIDTH    1000
#define SCREEN_HEIGHT   1000
#define CELL_SIZE       10

#define MENU_WIDTH      200

#define GRID_AREA_WIDTH  (SCREEN_WIDTH - MENU_WIDTH)
#define GRID_AREA_HEIGHT SCREEN_HEIGHT

#define MARGIN_X 25
#define MARGIN_Y 50

#define GRID_WIDTH  ((GRID_AREA_WIDTH - MARGIN_X * 2) / CELL_SIZE)
#define GRID_HEIGHT ((GRID_AREA_HEIGHT - MARGIN_Y * 2) / CELL_SIZE)

// #define OFFSET_X (MENU_WIDTH + (GRID_AREA_WIDTH - GRID_WIDTH * CELL_SIZE) / 2)
// #define OFFSET_Y ((GRID_AREA_HEIGHT - GRID_HEIGHT * CELL_SIZE) / 2)

void DrawGrid(const Grid& grid);

int main(void) {
    srand(time(NULL));
    Grid grid(GRID_WIDTH, GRID_HEIGHT);

    MazeGenerator generator(grid, DFS);
    MazeSolver solver(grid, WALLFOLLOWER);

    Render renderer(CELL_SIZE, MENU_WIDTH, SCREEN_WIDTH, SCREEN_HEIGHT, grid);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze");
    SetTargetFPS(100);
    bool flag = true;
    while (!WindowShouldClose()) {
        if (!generator.finished()) {
            generator.step();
        }
        else {
            if (flag) {
                solver.updateGrid();
                flag = false;
            }
            for (int i = 0; i < 10; i++)
                solver.step();
        }

        if (IsKeyPressed(KEY_SPACE)) generator.complete();

        if (IsKeyPressed(KEY_R)) {
            MazeMode cm = (generator.getCurrentMode() == DFS) ? PRIMS : DFS;
            generator.reset(cm);

            flag = true;
        }

        if (IsKeyPressed(KEY_T)) {
            SolveMode nextMode;
            switch (solver.getCurrentMode()) {
                case WALLFOLLOWER: nextMode = DIJKSTRA;     break;
                case DIJKSTRA:     nextMode = AS;           break;
                case AS:           nextMode = WALLFOLLOWER; break;
                default:           nextMode = WALLFOLLOWER; break;
            }
            solver.restart(nextMode);
        }

        BeginDrawing();
        if (!generator.finished()) {
            renderer.RenderGeneratorFrame(grid, generator);
        }
        else {
            renderer.RenderSolverFrame(solver.getGrid(), solver);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
