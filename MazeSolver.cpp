#include "MazeSolver.h"
#include <cstdlib>
#include <limits.h>

MazeSolver::MazeSolver(const Grid& g, SolveMode m) 
    : defaultGrid(g), grid(g), mode(m) {
        grid.resetCells();
    }

void MazeSolver::step() {
    if (finished()) return;
    if (mode == DIJKSTRA) stepDijkstra();
    else if (mode == AS) stepAS();
    else stepWallfollower();
}

void MazeSolver::complete() {
    while (!finished()) step();
}

void restart(SolveMode newMode){ //todo

}

bool MazeSolver::finished() const { 
    return done;
}

int MazeSolver::getCurrentCell() const { // todo
    if (mode == DIJKSTRA) 
        return 1;
    else if (mode == AS) 
        return pq.top().second;
    else if (mode == WALLFOLLOWER) 
        return WfCurrentCell;
    return -1;
}

SolveMode MazeSolver::getCurrentMode() const{
    return mode;
}

void MazeSolver::init() {
    int startIdx = 0;
    int finisIdx = grid.data.size() - 1;

    grid.data[startIdx].start = true;
    grid.data[startIdx].visited = true;
    
    grid.data[finisIdx].finish = true;

    if (mode == WALLFOLLOWER) {
        WfCurrentCell = startIdx;
        WfFacing = 1;
    }
    else if (mode == DIJKSTRA) {
        distanceMap.assign(grid.data.size(), INT_MAX);
        distanceMap[startIdx] = 0;
        pq.push({0, startIdx});
        grid.data[startIdx].visited = true;
    }
    else if (mode == AS) {

    }
 
}

std::vector<int> MazeSolver::getNeighbors(int x, int y, bool onlyVisited) {
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

void MazeSolver::stepAS() { // todo

}

void MazeSolver::stepDijkstra() { 
    if (pq.empty()) { 
        done = true; 
        return; 
    }

    auto [dist, currIdx] = pq.top();
    pq.pop();

    if (grid.data[currIdx].finish) {
        done = true;
        return;
    }

    auto [x, y] = grid.coords(currIdx);
    std::vector<int> neighbors = GetNeighborsWithWalls(x, y);


    for (int nIdx : neighbors) {
        int newDist = distanceMap[currIdx] + 1;

        if (newDist < distanceMap[nIdx]) {
            distanceMap[nIdx] = newDist;
            
            grid.data[nIdx].parent = &grid.data[currIdx];
            
            grid.data[nIdx].visited = true; 
            grid.data[nIdx].inFrontier = true; // queue visualisation
            pq.push({newDist, nIdx});
        }
    }
}

void MazeSolver::stepWallfollower() {
    if (done) return;

    auto [x, y] = grid.coords(WfCurrentCell);
    
    std::vector<int> directionsToCheck;

    if (followLeft) { // (facing dir + check dir) % 4
        directionsToCheck = {
            (WfFacing + 3) % 4, // left
            WfFacing,           // str
            (WfFacing + 1) % 4, // right
            (WfFacing + 2) % 4  // back
        };
    } 
    else {
        directionsToCheck = {
            (WfFacing + 1) % 4, // 1. right
            WfFacing,           // 2. str
            (WfFacing + 3) % 4, // 3. legt
            (WfFacing + 2) % 4  // 4. back
        };
    }
    for (int nextDir : directionsToCheck) {
        bool wallExists = true;
        
        if (nextDir == 0) wallExists = grid.data[WfCurrentCell].walls.top;
        else if (nextDir == 1) wallExists = grid.data[WfCurrentCell].walls.right;
        else if (nextDir == 2) wallExists = grid.data[WfCurrentCell].walls.bott;
        else if (nextDir == 3) wallExists = grid.data[WfCurrentCell].walls.left;

        if (!wallExists) {
            int nx = x + dx[nextDir];
            int ny = y + dy[nextDir];
            
            if (grid.inBounds(nx, ny)) {
                WfCurrentCell = grid.index(nx, ny);
                WfFacing = nextDir; 
                grid.data[WfCurrentCell].visited = true;
                
                if (grid.data[WfCurrentCell].finish) {
                    done = true;
                }
                return; 
            }
        }
    }
}

std::vector<int> MazeSolver::GetNeighborsWithWalls(int x, int y, bool onlyVisited) {
    int baseIdx = grid.index(x, y);
    std::vector<int> result;
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (!grid.inBounds(nx, ny))
            continue;

        int nIdx = grid.index(nx, ny);

        if (onlyVisited && !grid.data[nIdx].visited) 
            continue;

        if (!hasWall(baseIdx, i) && !hasWall(nIdx, (i + 2) % 4)) {
            result.push_back(nIdx);
        }
    }
    return result;
}

bool MazeSolver::hasWall(const int idx, int dir) {
    switch (dir) {
        case 0: return grid.data[idx].walls.top;
        case 1: return grid.data[idx].walls.right;
        case 2: return grid.data[idx].walls.bott;
        case 3: return grid.data[idx].walls.left;
    }
    return true;
}

