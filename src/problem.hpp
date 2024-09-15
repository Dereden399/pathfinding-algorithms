#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <vector>

struct Problem {
    int n;
    int m;
    std::vector<std::vector<short>> grid;
    std::vector<std::vector<int>> solution;
    int start[2];
    int goal[2];

    Problem(int n, int m) {
        this->n = n;
        this->m = m;
        grid = std::vector<std::vector<short>>(n, std::vector<short>(m, 0));
        solution = {};
        start[0] = -1;
        start[1] = -1;
        goal[0] = -1;
        goal[1] = -1;
    }
};

#endif