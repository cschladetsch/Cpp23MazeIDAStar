# Maze Solver

This project implements a maze generator and solver using **C++23**. The maze is randomly generated and always ensures a valid path from the start (top-left corner) to the goal (bottom-right corner). The solution is visualized in real-time using ANSI colours provided by the `rang` library.

## Example

[Example](resources/CppMaze.gif)
[Example2](resources/CppMaze2.gif)

## Features
- Randomized maze generation with guaranteed solvability.
- Iterative Deepening A* (IDA*) algorithm to find the solution.
- Real-time visualization with ANSI color-coded output:
  - **Red**: Walls (`#`)
  - **Green**: Unexplored paths (` `)
  - **Yellow**: Current solution path (`*`)
  - **Blue**: Checked but not part of the final path (`+`)
- Clear borders for better visualization.

## Dependencies
- **C++23 Compiler**: Ensure your environment supports C++23.
- [rang.hpp](https://github.com/agauniyal/rang): Header-only library for colorizing terminal output.
- rang will be installed locally in the project folder if not found.

## How to Build

```bash
$ mkdir -p build && cd build && cmake .. && make
```
Or, to do it all in one step and run the executable with an argument:

```bash
$ ./r N
```
Where N is an odd number greater than 5.

## Usage
Run the program with the desired maze size:

```bash
./MazeSolver <maze_size>
```

### Example
```bash
./MazeSolver 15
```
This generates a 15x15 maze, solves it using the IDA* algorithm, and visualizes the process step-by-step.

## How It Works
### Maze Generation
- The maze is created using a randomized depth-first search (DFS) algorithm.
- A direct connection between the start and goal is ensured by backtracking if necessary.

### Maze Solving
- The **IDA*** algorithm combines iterative deepening with a heuristic to explore the shortest path to the goal.
- The heuristic used is the Manhattan distance to the goal.
- Explored paths and failed attempts are visualized in real-time.

## Color Legend
- **Red (`#`)**: Walls
- **Green (` `)**: Unexplored paths
- **Yellow (`*`)**: Current path being explored
- **Blue (`+`)**: Checked paths that are not part of the final solution

## Limitations
- Only odd-sized mazes are supported (e.g., 5x5, 7x7).
- Visualization is terminal-based and requires support for ANSI colors.

## License
This project is open-source and available under the MIT License.

## Acknowledgments
- **rang.hpp** for ANSI color support.
- Iterative Deepening A* inspiration from AI pathfinding techniques.
