#include <iostream>
#include <math.h>

#include "solver.hpp"

void AStarSolver::solve(Problem& problem) {
    if (this->solved || this->solving) {
        std::cout << "Already solving a problem..." << std::endl;
        return;
    }
    std::cout << "Solving problem with A* algorithm..." << std::endl;
    std::printf("Start: (%u, %u). Goal: (%u, %u)\n", problem.start->x,
                problem.start->y, problem.goal->x, problem.goal->y);
    if (!problem.start || !problem.goal) {
        std::cout << "Start or goal not set!" << std::endl;
        return;
    }
    problem.restart();
    std::priority_queue<std::pair<int, Cell*>,
                        std::vector<std::pair<int, Cell*>>, Compare>
        newQ;
    std::swap(queue, newQ);

    cost = std::vector<std::vector<int>>(problem.n,
                                         std::vector<int>(problem.m, INT_MAX));
    cost[problem.start->x][problem.start->y] = 0;

    fValues = std::vector<std::vector<int>>(problem.n,
                                            std::vector<int>(problem.m, -1));
    fValues[problem.start->x][problem.start->y] = 0;

    parent = std::vector<std::vector<Cell*>>(
        problem.n, std::vector<Cell*>(problem.m, nullptr));

    queue.push({0, problem.start});
    solving = true;
}

int AStarSolver::stepSolve(Problem& problem) {
    if (!solving) {
        std::cout << "Hasn't started solving yet" << std::endl;
        return 0;
    }
    // found the solution during previous steps
    if (solved) {
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
        std::cout << "Solution found!" << std::endl;
        solved = true;
        solving = false;
        return totalCost;
    }
    auto a = queue.top();
    Cell* cell = a.second;
    queue.pop();
    cell->visited = true;
    // Found the goal, next step will build the solution
    if (cell == problem.goal) {
        solved = true;
        return 0;
    }
    int x = cell->x;
    int y = cell->y;
    for (auto [dx, dy] : directions) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx < 0 || nx >= problem.n || ny < 0 || ny >= problem.m) continue;
        int newCost = cost[x][y] + static_cast<int>(problem.grid[x][y].type);
        int heur = calculateHeuristic(problem.goal, nx, ny);
        int fVal = newCost + heur;
        if (fValues[nx][ny] == -1 || fValues[nx][ny] > fVal) {
            cost[nx][ny] = newCost;
            fValues[nx][ny] = fVal;
            parent[nx][ny] = cell;
            queue.push({fVal, &problem.grid[nx][ny]});
        }
    }
    return 0;
};

void DijkstraSolver::solve(Problem& problem) {
    if (this->solved || this->solving) {
        std::cout << "Already solving a problem..." << std::endl;
        return;
    }
    std::cout << "Solving problem with Dijkstra algorithm..." << std::endl;
    std::printf("Start: (%u, %u). Goal: (%u, %u)\n", problem.start->x,
                problem.start->y, problem.goal->x, problem.goal->y);
    if (!problem.start || !problem.goal) {
        std::cout << "Start or goal not set!" << std::endl;
        return;
    }
    problem.restart();
    std::priority_queue<std::pair<int, Cell*>,
                        std::vector<std::pair<int, Cell*>>, Compare>
        newQ;
    std::swap(queue, newQ);

    cost = std::vector<std::vector<int>>(problem.n,
                                         std::vector<int>(problem.m, INT_MAX));
    cost[problem.start->x][problem.start->y] = 0;

    parent = std::vector<std::vector<Cell*>>(
        problem.n, std::vector<Cell*>(problem.m, nullptr));

    queue.push({0, problem.start});
    solving = true;
}

int DijkstraSolver::stepSolve(Problem& problem) {
    if (!solving) {
        std::cout << "Hasn't started solving yet" << std::endl;
        return 0;
    }
    // found the solution during previous steps
    if (solved) {
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
        std::cout << "Solution found!" << std::endl;
        solved = true;
        solving = false;
        return totalCost;
    }
    auto a = queue.top();
    Cell* cell = a.second;
    cell->visited = true;
    int cellCost = a.first;
    queue.pop();
    if (cell == problem.goal) {
        solved = true;
        return 0;
    }

    int x = cell->x;
    int y = cell->y;

    for (auto [dx, dy] : directions) {
        int nx = x + dx;
        int ny = y + dy;
        if (nx < 0 || nx >= problem.n || ny < 0 || ny >= problem.m) continue;
        int newCost = cellCost + static_cast<int>(cell->type);
        if (newCost < cost[nx][ny]) {
            cost[nx][ny] = newCost;
            parent[nx][ny] = cell;
            queue.push({newCost, &problem.grid[nx][ny]});
        }
    }
    return 0;
};