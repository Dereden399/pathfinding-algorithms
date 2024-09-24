#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "problem.hpp"
#include "utils.hpp"
#include <string>
#include <queue>

struct Compare {
    bool operator()(const std::pair<int, Cell *> &a,
                    const std::pair<int, Cell *> &b) const {
        return a.first > b.first;
    }
};

class Solver {
   protected:
    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}};

   public:
    bool solved = false;
    bool solving = false;
    double lastStepTime = 0.0;

    virtual ~Solver() = default;
    virtual void solve(Problem &problem) = 0;
    virtual int stepSolve(Problem &problem) = 0;
    virtual std::string getName() = 0;
};

class AStarSolver : public Solver {
    int calculateHeuristic(Cell *goal, int x, int y) {
        return std::abs(x - (int)goal->x) + std::abs(y - (int)goal->y);
    };

    std::priority_queue<std::pair<int, Cell *>,
                        std::vector<std::pair<int, Cell *>>, Compare>
        queue;
    std::vector<std::vector<int>> cost;
    std::vector<std::vector<int>> fValues;
    std::vector<std::vector<Cell *>> parent;

   public:
    void solve(Problem &problem) override;
    int stepSolve(Problem &problem) override;
    std::string getName() override { return "A* Solver"; }
};

class DijkstraSolver : public Solver {
    std::priority_queue<std::pair<int, Cell *>,
                        std::vector<std::pair<int, Cell *>>, Compare>
        queue;
    std::vector<std::vector<int>> cost;
    std::vector<std::vector<Cell *>> parent;

   public:
    void solve(Problem &problem) override;
    int stepSolve(Problem &problem) override;
    std::string getName() override { return "Dijkstra Solver"; }
};

#endif  // SOLVER_HPP