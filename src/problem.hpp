#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <vector>

#include "cell.hpp"
#include "utils.hpp"

struct Problem {
    int n;
    int m;
    std::vector<std::vector<Cell>> grid;
    std::vector<Cell*> solution;
    Cell* start;
    Cell* goal;

    Problem(int n, int m);
    void clearSolution();
    void setCell(unsigned int x, unsigned int y, DrawMode mode);
    void clearCell(unsigned int x, unsigned int y);
};

#endif