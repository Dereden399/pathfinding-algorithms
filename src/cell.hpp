#ifndef CELL_HPP
#define CELL_HPP

#include "utils.hpp"

struct Cell {
    unsigned int x, y;
    CellType type = CellType::CLEAR;
    Cell(unsigned int x_, unsigned int y_) : x(x_), y(y_) {};
};

#endif