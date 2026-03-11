// #include "raylib.h"

// #include <cstdlib> 
//#include <ctime>  
// #include <vector>
// #include <stack>
// #include <utility>

#include "raylib.h"
#include "Grid.h"
#include "MazeGenerator.h"
#include <ctime>
#include <cstdlib>

#define SCREEN_WIDTH    1000
#define SCREEN_HEIGHT   1000
#define CELL_SIZE       30

#define MENU_WIDTH      200

#define GRID_AREA_WIDTH  (SCREEN_WIDTH - MENU_WIDTH)
#define GRID_AREA_HEIGHT SCREEN_HEIGHT

#define MARGIN_X 25
#define MARGIN_Y 50

#define GRID_WIDTH  ((GRID_AREA_WIDTH - MARGIN_X * 2) / CELL_SIZE)
#define GRID_HEIGHT ((GRID_AREA_HEIGHT - MARGIN_Y * 2) / CELL_SIZE)

#define OFFSET_X (MENU_WIDTH + (GRID_AREA_WIDTH - GRID_WIDTH * CELL_SIZE) / 2)
#define OFFSET_Y ((GRID_AREA_HEIGHT - GRID_HEIGHT * CELL_SIZE) / 2)

// enum MazeMode { DFS, PRIMS };

// struct Wall {
//     bool left = true;
//     bool top = true;
//     bool right = true;
//     bool bott = true;
// };

// struct Cell {
//     struct Wall walls;
//     bool visited = false;
//     bool path = false;
//     bool inFrontier = false;
// };

// struct MazeState {
//     std::stack<int> dfsStack;
//     std::vector<int> frontier;
//     bool finished = false;
//     MazeMode algo = DFS;
// };

// class Grid {
// public:
//     int cols; // width
//     int rows; // height
//     std::vector<Cell> data;

//     Grid(int c, int r) : cols(c), rows(r) {
//         data.resize(cols * rows);
//     }

//     int index(int x, int y) const {
//         return y * cols + x;
//     }

//     std::pair<int,int> coords(int i) const {
//         return { i % cols, i / cols };
//     }

//     bool inBounds(int x, int y) const {
//         return x >= 0 && y >= 0 && x < cols && y < rows;
//     }
// };

void DrawGrid(const Grid& grid);

// class MazeGenerator {
// public:

//     MazeGenerator(Grid& grid, MazeMode mode) : grid(grid), mode(mode) {
//         init();
//     }
    
//     void step() {
//         if (finished()) return;
//         if (mode == DFS) stepDFS();
//         else stepPrims();
//     }

//     void complete() {
//         while (!finished()) step();
//     }

//     bool finished() const { 
//         return done;
//     }

//     void reset(MazeMode newMode) {
//         mode = newMode;
//         done = false;
//         stack = {};
//         frontier.clear();
//         for (auto& cell : grid.data) {
//             cell = Cell{};  
//         }
//         init();
//     }

//     int getCurrentCell() const { 
//         if (mode == DFS && !stack.empty()) 
//             return stack.top();
//         if (mode == PRIMS && !frontier.empty()) 
//             return frontier[0];
        
//         return -1;
//     }

//     MazeMode getCurrentMode() const{
//         return mode;
//     }

//     const std::vector<int>& getFrontier() const { 
//         return frontier; 
//     }


//     private:
//     Grid& grid;
//     MazeMode mode;
//     std::stack<int> stack;
//     std::vector<int> frontier;
//     bool done = false;
//     static constexpr int dx[] = {0, 1, 0, -1};
//     static constexpr int dy[] = {-1, 0, 1, 0};

//     void init() {
        
//         int randomCell = rand() % grid.data.size();
//         auto [x, y] = grid.coords(randomCell);

//         int startIdx = grid.index(x, y);
//         grid.data[startIdx].visited = true;

//         if (mode == DFS) {
//             stack.push(startIdx);
//         } else {
//             addNeighborsToFrontier(x, y);
//         }
//     }

//     void stepDFS() {
//         if (stack.empty()) { done = true; return; }

//         int current = stack.top();
//         auto [x, y] = grid.coords(current);
//         std::vector<int> neighbors;

//         std::vector<int> nIdxVec = getNeighbors(x,y);
//         for (int nIdx : nIdxVec ){
//             if (!grid.data[nIdx].visited) 
//                 neighbors.push_back(nIdx);
//         }

//         if (!neighbors.empty()) {
//             int next = neighbors[rand() % neighbors.size()];
//             removeWalls(current, next);
//             grid.data[next].visited = true;
//             stack.push(next);
//         } else {
//             stack.pop();
//         }
//     }

//     void stepPrims(){
//         if (frontier.empty()) {
//             done = true;
//             return;
//         }

//         int randIdx = rand() % frontier.size();
//         int current = frontier[randIdx];
//         auto [x, y] = grid.coords(current);

//         std::vector<int> neighborsInMaze = getNeighbors(x, y, true);

//         if (!neighborsInMaze.empty()) {
//             removeWalls(current, neighborsInMaze[rand() % neighborsInMaze.size()]);
//         }

//         grid.data[current].visited = true;

//         frontier[randIdx] = frontier.back();
//         frontier.pop_back();

//         addNeighborsToFrontier(x, y);
//     }

//     void addNeighborsToFrontier(int x, int y) {
//         std::vector nIdxVec = getNeighbors(x, y);
//         for (int nIdx : nIdxVec){
//             if (!grid.data[nIdx].visited && !grid.data[nIdx].inFrontier) {
//                 grid.data[nIdx].inFrontier = true;
//                 frontier.push_back(nIdx);
//             }
//         }
//     }

//     std::vector<int> getNeighbors(int x, int y, bool onlyVisited = false) {
//         std::vector<int> result;
//         for (int i = 0; i < 4; i++) {
//             int nx = x + dx[i];
//             int ny = y + dy[i];
//             if (grid.inBounds(nx, ny)) {
//                 int nIdx = grid.index(nx, ny);
//                 if (!onlyVisited || grid.data[nIdx].visited)
//                     result.push_back(nIdx);
//             }
//         }
//         return result;
//     }

//     void removeWalls(int n, int m) {
//         auto [x1, y1] = grid.coords(n);
//         auto [x2, y2] = grid.coords(m);

//         int diffX = x1 - x2;
//         int diffY = y1 - y2;

//         if (diffX == -1) {
//             grid.data[n].walls.right = false;
//             grid.data[m].walls.left = false;
//         }
//         else if (diffX == 1) {
//             grid.data[n].walls.left = false;
//             grid.data[m].walls.right = false;
//         }
//         else if (diffY == -1) {
//             grid.data[n].walls.bott = false;
//             grid.data[m].walls.top = false;
//         }
//         else if (diffY == 1) {
//             grid.data[n].walls.top = false;
//             grid.data[m].walls.bott = false;
//         }
//     }
// };

int main(void) {
    srand(time(NULL));
    Grid grid(GRID_WIDTH, GRID_HEIGHT);
    MazeGenerator generator(grid, DFS);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze");
    SetTargetFPS(100);

    while (!WindowShouldClose()) {
        if (!generator.finished()) generator.step();

        if (IsKeyPressed(KEY_SPACE)) generator.complete();

        if (IsKeyPressed(KEY_R)) {
            MazeMode cm = (generator.getCurrentMode() == DFS) ? PRIMS : DFS;
            generator.reset(cm);
        }

        BeginDrawing();
            ClearBackground({15, 15, 25, 255});

            DrawRectangle(0, 0, MENU_WIDTH, SCREEN_HEIGHT, {30, 30, 40, 255}); // panel bg
            DrawText("DFS / PRIMS", 10, 50, 20, WHITE);
            DrawText("[R] Reset", 10, 100, 16, GRAY);


            DrawGrid(grid);

            int cur = generator.getCurrentCell();
            if (cur != -1 && generator.getCurrentMode() == DFS) {
                auto [cx, cy] = grid.coords(cur);
                DrawRectangle(OFFSET_X + cx*CELL_SIZE, OFFSET_Y + cy*CELL_SIZE, CELL_SIZE, CELL_SIZE, BLUE);
            }

            else if (cur != -1 && generator.getCurrentMode() == PRIMS) {
                std::vector<int> frontier = generator.getFrontier();
                for (int idx : frontier){
                    auto [x, y] = grid.coords(idx);
                    DrawRectangle(OFFSET_X + x*CELL_SIZE, OFFSET_Y + y*CELL_SIZE, CELL_SIZE, CELL_SIZE, {255, 100, 150, 255});
                }
            }


        EndDrawing();
    }

    CloseWindow();
    return 0;
}




void DrawGrid(const Grid& grid) {
    for (size_t i = 0; i < grid.data.size(); i++){
        auto [x,y] = grid.coords(i);
        int px = OFFSET_X + x * CELL_SIZE;
        int py = OFFSET_Y + y * CELL_SIZE;

        if (grid.data[i].visited) {
            DrawRectangle(px, py, CELL_SIZE, CELL_SIZE, {30, 30, 50, 255});
        }
        if (grid.data[i].walls.top) DrawLine(px, py, px + CELL_SIZE, py, {200, 200, 220, 255});
        if (grid.data[i].walls.bott) DrawLine(px, py + CELL_SIZE, px + CELL_SIZE, py + CELL_SIZE, {200, 200, 220, 255});
        if (grid.data[i].walls.left) DrawLine(px, py, px, py + CELL_SIZE, {200, 200, 220, 255});
        if (grid.data[i].walls.right) DrawLine(px + CELL_SIZE, py, px + CELL_SIZE, py + CELL_SIZE, {200, 200, 220, 255});
    }
}
