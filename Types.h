#ifndef TYPES_H
#define TYPES_H

enum MazeMode { DFS, PRIMS };

enum SolveMode {WALLFOLLOWER, DIJKSTRA, AS };

struct Wall {
    bool left = true, top = true, right = true, bott = true;
};

struct Cell {
    Wall walls;
    
    bool visited = false;
    bool path = false;
    bool inFrontier = false;

    Cell* parent = nullptr;

    bool start = false;
    bool finish = false;
};

#endif