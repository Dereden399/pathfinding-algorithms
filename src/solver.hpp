#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "problem.hpp"
#include <string>

class Solver {
   public:
    bool solved = false;

    virtual ~Solver() = default;
    virtual void solve(Problem &problem) = 0;
    virtual std::string getName() = 0;
};

class AStarSolver : public Solver {
   public:
    void solve(Problem &problem) override;
    std::string getName() override { return "A* Solver"; }
};

#endif  // SOLVER_HPP