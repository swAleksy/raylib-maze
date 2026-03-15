#include "Render.h"
#include "Types.h"
#include <raylib.h>

Render::Render(int cellSize, int menuWidth, int screenWidth, int screenHeight, const Grid& g)
    : cellSize(cellSize),
      menuWidth(menuWidth),
      offsetX(menuWidth + (screenWidth - menuWidth - g.cols * cellSize) / 2),
      offsetY((screenHeight - g.rows * cellSize) / 2),
      screenHeight(screenHeight)
{}


void Render::RenderGeneratorFrame(const Grid& grid, MazeGenerator& generator) {
    ClearBackground(colorBackground);

    DrawUI(generator.getCurrentMode());
    DrawGrid(grid, false);
    DrawOverlay(grid, generator);
}

void Render::RenderSolverFrame(const Grid& grid, MazeSolver& solver) {
    ClearBackground(colorBackground);

    DrawUI(solver.getCurrentMode());
    DrawGrid(grid, true);
    DrawOverlay(grid, solver);
}

void Render::DrawUI(MazeMode mode) {
    DrawRectangle(0, 0, menuWidth, screenHeight, colorPanel);

    DrawText("MAZE GENERATOR", 15, 30, 20, WHITE);
    DrawLine(15, 60, menuWidth - 15, 60, GRAY);

    DrawText("Current Mode:", 15, 80, 16, LIGHTGRAY);
    DrawText(mode == DFS ? "DFS" : "Prim's Alg.", 15, 100, 22, SKYBLUE);

    DrawText("Controls:", 15, 150, 16, LIGHTGRAY);
    DrawText("[R] Change Mode", 15, 180, 16, WHITE);
    DrawText("[SPACE] Skip Gen", 15, 210, 16, WHITE);
}

void Render::DrawUI(SolveMode mode) {
    DrawRectangle(0, 0, menuWidth, screenHeight, colorPanel);

    DrawText("MAZE SOLVER", 15, 30, 20, WHITE);
    DrawLine(15, 60, menuWidth - 15, 60, GRAY);

    const char* modeText = "";
    if (mode == WALLFOLLOWER) modeText = "Wall Follower";
    else if (mode == DIJKSTRA) modeText = "Dijkstra";
    else if (mode == AS) modeText = "A* (A-Star)";

    DrawText("Current Mode:", 15, 80, 16, LIGHTGRAY);
    DrawText(modeText, 15, 100, 22, GREEN);

    DrawText("Controls:", 15, 150, 16, LIGHTGRAY);
    DrawText("[T] Change Solver", 15, 180, 16, WHITE);
    DrawText("[R] Reset Maze", 15, 210, 16, WHITE);
}

void Render::DrawGrid(const Grid& grid, bool solving) {
    for (size_t i = 0; i < grid.data.size(); i++) {
        auto [x, y] = grid.coords(i);
        int px = offsetX + x * cellSize;
        int py = offsetY + y * cellSize;

        if (solving) {
            if (grid.data[i].path) {
                DrawRectangle(px+cellSize*0.1, py+cellSize*0.1, cellSize-cellSize*0.2, cellSize-cellSize*0.2, DARKGREEN);
            }
            else if (grid.data[i].visited) {
                DrawRectangle(px, py, cellSize, cellSize, colorVisitedSolving);
            }
            else if (grid.data[i].inFrontier) {
                DrawRectangle(px, py, cellSize, cellSize, {90, 40, 60, 255});
            }
        }
        else{
            DrawRectangle(px, py, cellSize, cellSize, colorVisited);
        }

        if (grid.data[i].walls.top) DrawLine(px, py, px + cellSize, py, colorWall);
        if (grid.data[i].walls.bott) DrawLine(px, py + cellSize, px + cellSize, py + cellSize, colorWall);
        if (grid.data[i].walls.left) DrawLine(px, py, px, py + cellSize, colorWall);
        if (grid.data[i].walls.right) DrawLine(px + cellSize, py, px + cellSize, py + cellSize, colorWall);
    }
}

void Render::DrawOverlay(const Grid& grid, MazeGenerator& generator) {
    int cur = generator.getCurrentCell();
    if (cur == -1) return;

    if (generator.getCurrentMode() == DFS) {
        auto [cx, cy] = grid.coords(cur);
        DrawRectangle(offsetX + cx * cellSize, offsetY + cy * cellSize, cellSize, cellSize, SKYBLUE);
    }
    else if (generator.getCurrentMode() == PRIMS) {
        std::vector<int> frontier = generator.getFrontier();
        for (int idx : frontier) {
            auto [x, y] = grid.coords(idx);
            DrawRectangle(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize, MAROON);
        }
    }
}

void Render::DrawOverlay(const Grid& grid, MazeSolver& solver){
    int cur = solver.getCurrentCell();
    if (cur == -1) return;

    if (solver.getCurrentMode() == WALLFOLLOWER) {
        auto [cx, cy] = grid.coords(cur);
        DrawRectangle(offsetX + cx * cellSize, offsetY + cy * cellSize, cellSize, cellSize, RED);
    }
    else if (solver.getCurrentMode() == DIJKSTRA) {
        auto [x, y] = grid.coords(cur);
        DrawRectangle(offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize, MAROON);
    }
}
