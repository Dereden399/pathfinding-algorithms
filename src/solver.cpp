#include "solver.hpp"

#include <iostream>
#include <queue>
void AStarSolver::solve(Problem& problem) {
    std::cout << "Solving problem with A* algorithm..." << std::endl;
    std::printf("Start: (%d, %d). Goal: (%d, %d)\n", problem.start[0],
                problem.start[1], problem.goal[0], problem.goal[1]);
    if (problem.start[0] == -1 || problem.start[1] == -1 ||
        problem.goal[0] == -1 || problem.goal[1] == -1) {
        std::cout << "Start or goal not set!" << std::endl;
        return;
    }
    auto compare = [](const std::tuple<int, int, int>& a,
                      const std::tuple<int, int, int>& b) {
        return std::get<0>(a) > std::get<0>(b);
    };
    std::priority_queue<std::tuple<int, int, int>,
                        std::vector<std::tuple<int, int, int>>,
                        decltype(compare)>
        queue(compare);

    std::vector<std::vector<int>> cost(problem.n,
                                       std::vector<int>(problem.m, -1));
    cost[problem.start[0]][problem.start[1]] = 0;

    std::vector<std::vector<std::pair<int, int>>> parent(
        problem.n,
        std::vector<std::pair<int, int>>(problem.m, std::pair(-1, -1)));
    auto calculateHeuristic = [&problem](int x, int y) {
        return std::abs(x - problem.goal[0]) + std::abs(y - problem.goal[1]);
    };
    auto calculateCellCost = [&problem](int x, int y) {
        short val = problem.grid[x][y];
        // nothing
        if (val == 0) return 1;
        // wall
        if (val == 1) return 1000000;
        // forest
        if (val == 2) return 5;
        // water
        if (val == 3) return 15;
        return 1;
    };
    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    queue.push({calculateHeuristic(problem.start[0], problem.start[1]),
                problem.start[0], problem.start[1]});
    while (!queue.empty()) {
        auto [cellCost, x, y] = queue.top();
        queue.pop();
        if (x == problem.goal[0] && y == problem.goal[1]) break;

        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx < 0 || nx >= problem.n || ny < 0 || ny >= problem.m)
                continue;
            int newCost = cellCost + calculateCellCost(nx, ny) +
                          calculateHeuristic(nx, ny);
            if (cost[nx][ny] == -1 || newCost < cost[nx][ny]) {
                cost[nx][ny] = newCost;
                parent[nx][ny] = {x, y};
                queue.push({newCost, nx, ny});
            }
        }
    }
    // now need to build the path
    int x = problem.goal[0], y = problem.goal[1];
    while (x != problem.start[0] || y != problem.start[1]) {
        problem.solution.push_back({x, y});
        auto a = parent[x][y];
        int nx = a.first;
        int ny = a.second;
        x = nx;
        y = ny;
    }
    std::reverse(problem.solution.begin(), problem.solution.end());
    std::cout << "Solution found!" << std::endl;
    solved = true;
}