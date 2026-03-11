#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include <stack>
#include <vector>
#include "Grid.h"

class MazeGenerator {
public:
    MazeGenerator(Grid& grid, MazeMode mode);
    void step();
    void complete();
    void reset(MazeMode newMode);
    bool finished() const;
    int getCurrentCell() const;
    MazeMode getCurrentMode() const;
    const std::vector<int>& getFrontier() const;

private:
    Grid& grid;
    MazeMode mode;
    std::stack<int> stack;
    std::vector<int> frontier;
    bool done = false;
    static constexpr int dx[] = {0, 1, 0, -1};
    static constexpr int dy[] = {-1, 0, 1, 0};

    void init();
    void stepDFS();
    void stepPrims();
    void addNeighborsToFrontier(int x, int y);
    std::vector<int> getNeighbors(int x, int y, bool onlyVisited = false);
    void removeWalls(int n, int m);
};

#endif