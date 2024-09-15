#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "problem.hpp"

class Solver {
   public:
    bool solved = false;

    virtual ~Solver() = default;
    virtual void solve(Problem &problem) = 0;
};

class AStarSolver : public Solver {
   public:
    void solve(Problem &problem) override;
};

#endif  // SOLVER_HPP