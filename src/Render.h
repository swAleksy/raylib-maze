#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "Grid.h"
#include "MazeSolver.h"
#include "MazeGenerator.h"

class Render {
public:
    Render(int cellSize, int menuWidth, int screenWidth, int screenHeight, const Grid& grid);
    void RenderGeneratorFrame(const Grid& grid, MazeGenerator& generator);
    void RenderSolverFrame(const Grid& grid, MazeSolver& solver);

private:
    int cellSize;
    int menuWidth;
    int offsetX;
    int offsetY;
    int screenHeight;

    Color colorBackground = {15, 15, 25, 255};
    Color colorBackground2 = {15, 15, 255, 255};
    Color colorWall = {200, 200, 220, 255};
    Color colorVisited = {30, 30, 50, 255};
    Color colorVisitedSolving = {70, 30, 50, 255};
    Color colorPanel = {30, 30, 40, 255};

    void DrawUI(MazeMode mode);
    void DrawUI(SolveMode mode);
    void DrawGrid(const Grid& grid, bool solving);
    void DrawOverlay(const Grid& grid, MazeGenerator& generator);
    void DrawOverlay(const Grid& grid, MazeSolver& solver);
};

#endif
