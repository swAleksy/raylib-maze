#ifndef TYPES_H
#define TYPES_H

enum MazeMode { DFS, PRIMS };

enum SolveMode {WALLFOLLOWER, DIJKSTRA, AS };

enum Direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

enum class AppState { GENERATING, PREPARING_SOLVER, SOLVING, DONE }; //todo

struct Wall {
    bool left = true, top = true, right = true, bott = true;
};

struct Cell {
    Wall walls;

    bool visited = false;
    bool path = false;
    bool inFrontier = false;

    int parentIdx = -1;

    bool start = false;
    bool finish = false;
};

#endif
