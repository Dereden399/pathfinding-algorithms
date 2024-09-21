#include <iostream>
#include <queue>
#include <math.h>

#include "solver.hpp"

int AStarSolver::solve(Problem& problem) {
    std::cout << "Solving problem with A* algorithm..." << std::endl;
    std::printf("Start: (%u, %u). Goal: (%u, %u)\n", problem.start->x,
                problem.start->y, problem.goal->x, problem.goal->y);
    if (!problem.start || !problem.goal) {
        std::cout << "Start or goal not set!" << std::endl;
        return -1;
    }
    problem.solution.clear();
    auto compare = [](const std::pair<int, Cell*>& a,
                      const std::pair<int, Cell*>& b) {
        return std::get<0>(a) > std::get<0>(b);
    };
    std::priority_queue<std::pair<int, Cell*>,
                        std::vector<std::pair<int, Cell*>>, decltype(compare)>
        queue(compare);

    std::vector<std::vector<int>> cost(problem.n,
                                       std::vector<int>(problem.m, INT_MAX));
    cost[problem.start->x][problem.start->y] = 0;

    std::vector<std::vector<int>> fValues(problem.n,
                                          std::vector<int>(problem.m, -1));
    fValues[problem.start->x][problem.start->y] = 0;

    std::vector<std::vector<Cell*>> parent(
        problem.n, std::vector<Cell*>(problem.m, nullptr));

    auto calculateHeuristic = [&problem](int x, int y) {
        return std::abs(x - (int)problem.goal->x) +
               std::abs(y - (int)problem.goal->y);
    };
    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    queue.push({0, problem.start});
    int cellsChecked = 0;
    while (!queue.empty()) {
        auto a = queue.top();
        Cell* cell = a.second;
        queue.pop();
        cellsChecked++;
        if (cell == problem.goal) break;
        int x = cell->x;
        int y = cell->y;
        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx < 0 || nx >= problem.n || ny < 0 || ny >= problem.m)
                continue;
            int newCost =
                cost[x][y] + static_cast<int>(problem.grid[x][y].type);
            int heur = calculateHeuristic(nx, ny);
            int fVal = newCost + heur;
            if (fValues[nx][ny] == -1 || fValues[nx][ny] > fVal) {
                cost[nx][ny] = newCost;
                fValues[nx][ny] = fVal;
                parent[nx][ny] = cell;
                queue.push({fVal, &problem.grid[nx][ny]});
            }
        }
    }
    // now need to build the path
    Cell* cell = problem.goal;
    int totalCost = 0;
    while (cell != problem.start) {
        problem.solution.push_back(cell);
        totalCost += static_cast<int>(cell->type);
        Cell* p = parent[cell->x][cell->y];
        cell = p;
    }
    totalCost += static_cast<int>(cell->type);
    problem.solution.push_back(cell);
    std::reverse(problem.solution.begin(), problem.solution.end());
    std::printf("Solution found! Cells checked: %d\n", cellsChecked);
    solved = true;
    return totalCost;
}

int DijkstraSolver::solve(Problem& problem) {
    std::cout << "Solving problem with Dijkstra algorithm..." << std::endl;
    std::printf("Start: (%u, %u). Goal: (%u, %u)\n", problem.start->x,
                problem.start->y, problem.goal->x, problem.goal->y);
    if (!problem.start || !problem.goal) {
        std::cout << "Start or goal not set!" << std::endl;
        return -1;
    }
    problem.solution.clear();
    auto compare = [](const std::pair<int, Cell*>& a,
                      const std::pair<int, Cell*>& b) {
        return std::get<0>(a) > std::get<0>(b);
    };
    std::priority_queue<std::pair<int, Cell*>,
                        std::vector<std::pair<int, Cell*>>, decltype(compare)>
        queue(compare);

    std::vector<std::vector<int>> cost(problem.n,
                                       std::vector<int>(problem.m, INT_MAX));
    cost[problem.start->x][problem.start->y] = 0;

    std::vector<std::vector<Cell*>> parent(
        problem.n, std::vector<Cell*>(problem.m, nullptr));

    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    queue.push({0, problem.start});
    int cellsChecked = 0;
    while (!queue.empty()) {
        auto a = queue.top();
        Cell* cell = a.second;
        int cellCost = a.first;
        queue.pop();
        cellsChecked++;
        if (cell == problem.goal) break;

        int x = cell->x;
        int y = cell->y;

        for (auto [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx < 0 || nx >= problem.n || ny < 0 || ny >= problem.m)
                continue;
            int newCost = cellCost + static_cast<int>(cell->type);
            if (newCost < cost[nx][ny]) {
                cost[nx][ny] = newCost;
                parent[nx][ny] = cell;
                queue.push({newCost, &problem.grid[nx][ny]});
            }
        }
    }
    Cell* cell = problem.goal;
    int totalCost = 0;
    while (cell != problem.start) {
        problem.solution.push_back(cell);
        totalCost += static_cast<int>(cell->type);
        Cell* p = parent[cell->x][cell->y];
        cell = p;
    }
    totalCost += static_cast<int>(cell->type);
    problem.solution.push_back(cell);
    std::reverse(problem.solution.begin(), problem.solution.end());
    std::printf("Solution found! Cells checked: %d\n", cellsChecked);
    solved = true;
    return totalCost;
}