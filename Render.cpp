#include "Render.h"
#include "Types.h"
#include <raylib.h>

Render::Render(int cellSize, int menuWidth, int screenWidth, int screenHeight, const Grid& g)
    : cellSize(cellSize),
      menuWidth(menuWidth),
      screenHeight(screenHeight),
      offsetX(menuWidth + (screenWidth - menuWidth - g.cols * cellSize) / 2),
      offsetY((screenHeight - g.rows * cellSize) / 2)
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
    DrawText(mode == DFS ? "Mode: DFS" : "Mode: PRIMS", 10, 50, 20, WHITE);
    DrawText("[R] Reset", 10, 100, 16, GRAY);
    DrawText("[SPACE] Complete", 10, 130, 16, GRAY);
}

void Render::DrawUI(SolveMode mode) {
    DrawRectangle(0, 0, menuWidth, screenHeight, colorPanel);
    DrawText("Solving maze:", 10, 20, 10, RED);
    if (mode == WALLFOLLOWER) DrawText("Mode: WALLFOLLOWER", 10, 50, 20, WHITE);
    else if (mode == DIJKSTRA) DrawText("Mode: DIJKSTRA", 10, 50, 20, WHITE);
    else if (mode == AS) DrawText("Mode: A*", 10, 50, 20, WHITE);

    // DrawText("[R] Reset", 10, 100, 16, GRAY);
    // DrawText("[SPACE] Complete", 10, 130, 16, GRAY);
}

void Render::DrawGrid(const Grid& grid, bool solving) {
    for (size_t i = 0; i < grid.data.size(); i++) {
        auto [x, y] = grid.coords(i);
        int px = offsetX + x * cellSize;
        int py = offsetY + y * cellSize;

        if (grid.data[i].visited && !solving) {
            DrawRectangle(px, py, cellSize, cellSize, colorVisited);
        }
        else if (grid.data[i].visited && solving){
            DrawRectangle(px, py, cellSize, cellSize, colorVisitedSolving);
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
