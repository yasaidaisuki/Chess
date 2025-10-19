#ifndef ROOK_H
#define ROOK_H

#include <vector>
#include "position.h"
#include "piece.h"

class Rook : public Piece {
    friend class Board;
    public:
        Rook(Position posn, int team, bool hasMoved);
        std::vector<Position>  possibleTakes(const Board& board) override;
        std::vector<Position>  possibleMoves(const Board& board) override;
};

#endif
