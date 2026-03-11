#include "Grid.h"

Grid::Grid(int c, int r) : cols(c), rows(r) {
    data.resize(cols * rows);
}

int Grid::index(int x, int y) const { 
    return y * cols + x; 
}

std::pair<int, int> Grid::coords(int i) const { 
    return { i % cols, i / cols }; 
} 

bool Grid::inBounds(int x, int y) const {
    return x >= 0 && y >= 0 && x < cols && y < rows;
}