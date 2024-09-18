#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "problem.hpp"
#include <string>

class Solver {
   public:
    bool solved = false;

    virtual ~Solver() = default;
    virtual int solve(Problem &problem) = 0;
    virtual std::string getName() = 0;
};

class AStarSolver : public Solver {
   public:
    int solve(Problem &problem) override;
    std::string getName() override { return "A* Solver"; }
};

class DijkstraSolver : public Solver {
   public:
    int solve(Problem &problem) override;
    std::string getName() override { return "Dijkstra Solver"; }
};

#endif  // SOLVER_HPP