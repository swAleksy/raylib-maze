#include "MazeGenerator.h"
#include <cstdlib>

MazeGenerator::MazeGenerator(Grid& grid, MazeMode mode) : grid(grid), mode(mode) {
    init();
}

void MazeGenerator::step() {
    if (finished()) return;
    if (mode == DFS) stepDFS();
    else stepPrims();
}

void MazeGenerator::complete() {
    while (!finished()) step();
}

bool MazeGenerator::finished() const { 
    return done;
}

void MazeGenerator::reset(MazeMode newMode) {
    mode = newMode;
    done = false;
    stack = {};
    frontier.clear();
    for (auto& cell : grid.data) {
        cell = Cell{};  
    }
    init();
}

int MazeGenerator::getCurrentCell() const { 
    if (mode == DFS && !stack.empty()) 
        return stack.top();
    if (mode == PRIMS && !frontier.empty()) 
        return frontier[0];
    
    return -1;
}

MazeMode MazeGenerator::getCurrentMode() const{
    return mode;
}

const std::vector<int>& MazeGenerator::getFrontier() const { 
    return frontier; 
}

void MazeGenerator::init() {
    
    int randomCell = rand() % grid.data.size();
    auto [x, y] = grid.coords(randomCell);

    int startIdx = grid.index(x, y);
    grid.data[startIdx].visited = true;

    if (mode == DFS) {
        stack.push(startIdx);
    } else {
        addNeighborsToFrontier(x, y);
    }
}

void MazeGenerator::stepDFS() {
    if (stack.empty()) { 
        done = true; 
        return; 
    }

    int current = stack.top();
    auto [x, y] = grid.coords(current);
    std::vector<int> neighbors;

    std::vector<int> nIdxVec = getNeighbors(x,y);
    for (int nIdx : nIdxVec ){
        if (!grid.data[nIdx].visited) 
            neighbors.push_back(nIdx);
    }

    if (!neighbors.empty()) {
        int next = neighbors[rand() % neighbors.size()];
        removeWalls(current, next);
        grid.data[next].visited = true;
        stack.push(next);
    } else {
        stack.pop();
    }
}

void MazeGenerator::stepPrims(){
    if (frontier.empty()) {
        done = true;
        return;
    }

    int randIdx = rand() % frontier.size();
    int current = frontier[randIdx];
    auto [x, y] = grid.coords(current);

    std::vector<int> neighborsInMaze = getNeighbors(x, y, true);

    if (!neighborsInMaze.empty()) {
        removeWalls(current, neighborsInMaze[rand() % neighborsInMaze.size()]);
    }

    grid.data[current].visited = true;

    frontier[randIdx] = frontier.back();
    frontier.pop_back();

    addNeighborsToFrontier(x, y);
}

void MazeGenerator::addNeighborsToFrontier(int x, int y) {
    std::vector nIdxVec = getNeighbors(x, y);
    for (int nIdx : nIdxVec){
        if (!grid.data[nIdx].visited && !grid.data[nIdx].inFrontier) {
            grid.data[nIdx].inFrontier = true;
            frontier.push_back(nIdx);
        }
    }
}

std::vector<int> MazeGenerator::getNeighbors(int x, int y, bool onlyVisited) {
    std::vector<int> result;
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (grid.inBounds(nx, ny)) {
            int nIdx = grid.index(nx, ny);
            if (!onlyVisited || grid.data[nIdx].visited)
                result.push_back(nIdx);
        }
    }
    return result;
}

void MazeGenerator::removeWalls(int n, int m) {
    auto [x1, y1] = grid.coords(n);
    auto [x2, y2] = grid.coords(m);

    int diffX = x1 - x2;
    int diffY = y1 - y2;

    if (diffX == -1) {
        grid.data[n].walls.right = false;
        grid.data[m].walls.left = false;
    }
    else if (diffX == 1) {
        grid.data[n].walls.left = false;
        grid.data[m].walls.right = false;
    }
    else if (diffY == -1) {
        grid.data[n].walls.bott = false;
        grid.data[m].walls.top = false;
    }
    else if (diffY == 1) {
        grid.data[n].walls.top = false;
        grid.data[m].walls.bott = false;
    }
}