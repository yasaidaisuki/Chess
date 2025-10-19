#ifndef BISHOP_H
#define BISHOP_H

#include <vector>
#include "position.h"
#include "piece.h"

class Bishop : public Piece {
    public:
        Bishop(Position posn, int team, bool hasMoved);
        std::vector<Position>  possibleTakes(const Board& board) override;
        std::vector<Position>  possibleMoves(const Board& board) override;
};

#endif
