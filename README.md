# Raylib Maze Generator & Solver

A C++ visualizer for maze generation (DFS, Prim's) and solving algorithms (A*, Dijkstra, Wall Follower).

## Prerequisites

You need to have **raylib** installed on your system:
- **Fedora:** `sudo dnf install raylib-devel`
- **Ubuntu/Debian:** `sudo apt install libraylib-dev`
- **Arch Linux:** `sudo pacman -S raylib`
- [raylib-quickstart](https://github.com/raylib-extras/raylib-quickstart)

## How to Build and Run

Use the following command in the root directory:
`$ make run`

## Controls
- [R]: Reset maze / Change generation algorithm
- [SPACE]: Skip generation animation
- [T]: Change solving algorithm