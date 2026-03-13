#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <stack>
#include <vector>
#include <queue>
#include "Grid.h"

class MazeSolver {
public:
    MazeSolver(const Grid& g, SolveMode m);
    void step();
    void complete();
    void restart(SolveMode newMode);
    bool finished() const;
    int getCurrentCell() const;
    SolveMode getCurrentMode() const;

private:
    const Grid& defaultGrid;
    Grid grid;
    SolveMode mode;
    
    int WfCurrentCell;
    int WfFacing; // direction of alg 0: up, 1: right, 2: down, 3: left; (dx[x], dy[x])
    bool followLeft = false;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq; //dist, idx
    std::vector<int> distanceMap;

    bool done = false;
    static constexpr int dx[] = {0, 1, 0, -1};
    static constexpr int dy[] = {-1, 0, 1, 0};

    void init();
    std::vector<int> getNeighbors(int x, int y, bool onlyVisited = false);
    void stepAS();
    void stepDijkstra();
    void stepWallfollower();
    std::vector<int> MazeSolver::GetNeighborsWithWalls(int x, int y, bool onlyVisited = false);
    bool MazeSolver::hasWall(const int idx, int dir);
};

#endif