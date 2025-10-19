#ifndef BLANK_H
#define BLANK_H

#include <vector>
#include "position.h"
#include "piece.h"

class Blank : public Piece {
    public:
        Blank(Position posn, int team, bool hasMoved): Piece{posn, team, BLANK, hasMoved} {}
        std::vector<Position>  possibleTakes(const Board& board) override {return {}; }
        std::vector<Position>  possibleMoves(const Board& board) override {return {}; }
};
#endif
