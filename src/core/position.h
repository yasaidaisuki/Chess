#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <string>

class Position {
    public: 
        int row, col;
        Position(int r = 0, int c = 0) : row(r), col(c) {}
        bool operator==(const Position &other) const;
        bool operator!=(const Position &other) const;
};

std::ostream& operator<<(std::ostream& out, const Position &pos);

#endif
