#include "MazeSolver.h"
#include <climits>
#include <cmath>

MazeSolver::MazeSolver(const Grid& g, SolveMode m)
    : defaultGrid(g), grid(g), mode(m) {
        init();
    }

void MazeSolver::step() {
    if (isFinished()) return;
    if (mode == DIJKSTRA) stepDijkstra();
    else if (mode == AS) stepAS();
    else stepWallfollower();
}

void MazeSolver::complete() {
    while (!isFinished()) step();
}

void MazeSolver::restart(SolveMode newMode){
    mode = newMode;
    updateGrid();
    done = false;
}

bool MazeSolver::isFinished() const {
    return done;
}

int MazeSolver::getCurrentCell() const {
    if (pq.empty())
        return -1;
    if (mode == DIJKSTRA)
        return pq.top().second;
    else if (mode == AS)
        return pq.top().second;
    else if (mode == WALLFOLLOWER)
        return WfCurrentCell;
    return -1;
}

SolveMode MazeSolver::getCurrentMode() const{
    return mode;
}

void MazeSolver::updateGrid(){
    grid = defaultGrid;
    init();
}

const Grid& MazeSolver::getGrid() const {
    return grid;
}

void MazeSolver::init() {
    grid.resetCells();

    int startIdx = 0;
    int finisIdx = grid.data.size() - 1;

    grid.data[startIdx].start = true;
    grid.data[startIdx].visited = true;

    grid.data[finisIdx].finish = true;

    if (mode == WALLFOLLOWER) {
        WfCurrentCell = startIdx;
        WfFacing = RIGHT;
    }
    else if (mode == DIJKSTRA) {
        distanceMap.assign(grid.data.size(), INT_MAX);
        distanceMap[startIdx] = 0;

        pq = {};
        pq.push({0, startIdx});

        //grid.data[startIdx].visited = true;
    }
    else if (mode == AS) {
        distanceMap.assign(grid.data.size(), INT_MAX);
        distanceMap[startIdx] = 0;

        pq = {};
        int score = getManhattanDist(startIdx);
        pq.push({score, startIdx});

        //grid.data[startIdx].visited = true;
    }
}


void MazeSolver::stepAS() {
    if (pq.empty()) {
        done = true;
        return;
    }
    auto [score, currIdx] = pq.top(); // score == cost from start to current idx (distance map) + manhattan distance to finish cell
    pq.pop();
    grid.data[currIdx].visited = true;

    if (grid.data[currIdx].finish) {
        done = true;
        setFinPath(currIdx);
        return;
    }

    auto [x, y] = grid.coords(currIdx);
    std::vector<int> neighbors = GetNeighborsWithWalls(x, y);

    for (int nIdx : neighbors) {
        int baseScore = distanceMap[currIdx] + 1; // dist from start

        if (baseScore < distanceMap[nIdx]) {
            distanceMap[nIdx] = baseScore;

            int score = baseScore + getManhattanDist(nIdx);

            grid.data[nIdx].parentIdx = currIdx;

            grid.data[nIdx].inFrontier = true;
            pq.push({score, nIdx});
        }
    }
}

void MazeSolver::stepDijkstra() {
    if (pq.empty()) {
        done = true;
        return;
    }

    auto [dist, currIdx] = pq.top();
    pq.pop();
    grid.data[currIdx].visited = true
    ;
    if (grid.data[currIdx].finish) {
        done = true;
        setFinPath(currIdx);
        return;
    }

    auto [x, y] = grid.coords(currIdx);
    std::vector<int> neighbors = GetNeighborsWithWalls(x, y);


    for (int nIdx : neighbors) {
        int newDist = distanceMap[currIdx] + 1;

        if (newDist < distanceMap[nIdx]) {
            distanceMap[nIdx] = newDist;

            grid.data[nIdx].parentIdx = currIdx;

            grid.data[nIdx].inFrontier = true; // queue visualisation
            pq.push({newDist, nIdx});
        }
    }
}

void MazeSolver::stepWallfollower() {
    if (done) return;

    auto [x, y] = grid.coords(WfCurrentCell);

    std::vector<int> directionsToCheck;
    int facingInt = static_cast<int>(WfFacing);

    if (followLeft) { // (facing dir + check dir) % 4
        directionsToCheck = {
            (facingInt + 3) % 4, // left
            facingInt,           // str
            (facingInt + 1) % 4, // right
            (facingInt + 2) % 4  // back
        };
    }
    else {
        directionsToCheck = {
            (facingInt + 1) % 4, // 1. right
            facingInt,           // 2. str
            (facingInt + 3) % 4, // 3. legt
            (facingInt + 2) % 4  // 4. back
        };
    }
    for (int nextDir : directionsToCheck) {
        bool wallExists = true;

        if (nextDir == UP) wallExists = grid.data[WfCurrentCell].walls.top;
        else if (nextDir == RIGHT) wallExists = grid.data[WfCurrentCell].walls.right;
        else if (nextDir == DOWN) wallExists = grid.data[WfCurrentCell].walls.bott;
        else if (nextDir == LEFT) wallExists = grid.data[WfCurrentCell].walls.left;

        if (!wallExists) {
            int nx = x + dx[nextDir];
            int ny = y + dy[nextDir];

            if (grid.inBounds(nx, ny)) {
                WfCurrentCell = grid.index(nx, ny);
                WfFacing = static_cast<Direction>(nextDir);
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

int MazeSolver::getManhattanDist(int idx) {
    auto [x1, y1] = grid.coords(idx);
    auto [x2, y2] = grid.coords(grid.data.size() - 1);
    return std::abs(x1 - x2) + std::abs(y1 - y2); // manhattan distance
}

void MazeSolver::setFinPath(int endIdx){
    int curr = endIdx;
    int safetyCounter = 0;
    int maxCells = grid.data.size();
    while (curr != -1) {
        grid.data[curr].path = true;
        curr = grid.data[curr].parentIdx;
    }
}
