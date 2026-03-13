#ifndef GRID_H
#define GRID_H

#include <vector>
#include <utility>
#include "Types.h"

class Grid {
public:
    int cols;
    int rows;
    std::vector<Cell> data;

    Grid(int c, int r);
    Grid(const Grid& grid);
    int index(int x, int y) const;
    std::pair<int, int> coords(int i) const;
    bool inBounds(int x, int y) const;
    void Grid::resetCells();
};

#endif