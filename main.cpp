#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <cmath>
#include <tuple>
#include <algorithm>
#include <functional>
#include <random>
#include <chrono>
#include <thread>
#include "rang.hpp"

using namespace std;

class Maze {
public:
    explicit Maze(int size) : size(size), maze(size, vector<char>(size, '#')) {
        generateMaze();
    }

    void displayMaze() const 
	{
		system("clear");
		auto drawBorder = [this]() {
			cout 
				<< rang::fg::cyan 
				<< '+' << string(size * 2 - 1, '-') 
				<< '+' << rang::style::reset << "\n";
		};

		auto drawCell = [](char cell) {
			static const std::map<char, rang::fg> colorMap = {
				{'#', rang::fg::red},
				{' ', rang::fg::green},
				{'*', rang::fg::yellow},
				{'+', rang::fg::blue}
			};

			auto it = colorMap.find(cell);
			if (it != colorMap.end()) {
				cout << it->second << cell << rang::style::reset << ' ';
			} else {
				cout << cell << ' ';
			}
		};

		for (int y = 0; y < size; ++y) {
			cout << rang::fg::cyan << '|' << rang::style::reset;

			for (int x = 0; x < size; ++x) {
				drawCell(maze[y][x]);
			}

			cout << rang::fg::cyan << '|' << rang::style::reset << "\n";
		}

		drawBorder();
		cout.flush();
	}

    void clearVisited() {
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                if (maze[y][x] == '*') {
                    maze[y][x] = '+'; // Mark previously visited cells differently
                }
            }
        }
    }

    vector<vector<char>> &getMaze() {
        return maze;
    }

    int getSize() const {
        return size;
    }

private:
    int size;
    vector<vector<char>> maze;

    void generateMaze() {
        stack<pair<int, int>> cellStack;
        auto randomDirection = []() {
            vector<pair<int, int>> directions = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}};
            shuffle(directions.begin(), directions.end(), mt19937(random_device{}()));
            return directions;
        };

        maze[0][0] = ' ';
        cellStack.push({0, 0});

        while (!cellStack.empty()) {
            auto [x, y] = cellStack.top();
            bool moved = false;

            for (auto [dx, dy] : randomDirection()) {
                int nx = x + dx;
                int ny = y + dy;
                if (isInside(nx, ny) && maze[ny][nx] == '#') {
                    maze[ny][nx] = ' ';
                    maze[y + dy / 2][x + dx / 2] = ' ';
                    cellStack.push({nx, ny});
                    moved = true;
                    break;
                }
            }

            if (!moved) {
                cellStack.pop();
            }
        }

        ensurePathToGoal();
    }

    void ensurePathToGoal() {
        // Ensure there is a randomized, valid path to the goal
        int x = size - 1, y = size - 1;
        while (x > 0 || y > 0) {
            maze[y][x] = ' ';
            if (x > 0 && (y == 0 || rand() % 2 == 0)) {
                x--;
            } else {
                y--;
            }
        }
        maze[0][0] = ' '; // Ensure the start point is clear
    }

    bool isInside(int x, int y) const {
        return x >= 0 && x < size && y >= 0 && y < size;
    }
};

class MazeSolver {
public:
    MazeSolver(Maze &maze) : maze(maze), size(maze.getSize()), solved(false) {}

    void solve() {
        auto heuristic = [&](int x, int y) {
            return abs(size - 1 - x) + abs(size - 1 - y);
        };

        function<int(int, int, int, int)> idaStar = [&](int x, int y, int g, int threshold) -> int {
            int f = g + heuristic(x, y);
            if (f > threshold) return f;

            if (x == size - 1 && y == size - 1) {
                solved = true;
                maze.getMaze()[y][x] = '*';
                maze.displayMaze();
                this_thread::sleep_for(chrono::milliseconds(200));
                return -1; // Found solution
            }

            if (maze.getMaze()[y][x] != ' ') return INT_MAX;

            maze.getMaze()[y][x] = '*';
            maze.displayMaze();
            this_thread::sleep_for(chrono::milliseconds(200));

            int minThreshold = INT_MAX;
            for (auto [dx, dy] : directions) {
                int nx = x + dx;
                int ny = y + dy;
                if (isInside(nx, ny)) {
                    int nextThreshold = idaStar(nx, ny, g + 1, threshold);
                    if (solved) return -1; // Stop searching if solved
                    minThreshold = min(minThreshold, nextThreshold);
                }
            }

            maze.getMaze()[y][x] = '+'; // Mark as checked but not part of the final path
            return minThreshold;
        };

        int threshold = heuristic(0, 0);
        while (!solved) {
            maze.clearVisited();
            int nextThreshold = idaStar(0, 0, 0, threshold);
            if (solved) break;
            if (nextThreshold == INT_MAX) {
                cerr << "No solution found.\n";
                return;
            }
            threshold = nextThreshold; // Increment to the next minimum threshold
        }

        maze.displayMaze(); // Display final solved maze
    }

private:
    Maze &maze;
    int size;
    bool solved;
    vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    bool isInside(int x, int y) const {
        return x >= 0 && x < size && y >= 0 && y < size;
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <maze_size>\n";
        return 1;
    }

    int mazeSize = stoi(argv[1]);
    if (mazeSize < 5 || mazeSize % 2 == 0) {
        cerr << "Maze size must be an odd number >= 5.\n";
        return 1;
    }

    Maze maze(mazeSize);
    maze.displayMaze();

    MazeSolver solver(maze);
    solver.solve();

    return 0;
}

