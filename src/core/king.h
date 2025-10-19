#ifndef KING_H
#define KING_H

#include <vector>
#include "position.h"
#include "piece.h"

class King : public Piece {
    friend class Board;
    public:
        King(Position posn, int team, bool hasMoved); // Constructor
        std::vector<Position> possibleTakes(const Board& board) override;
        std::vector<Position> possibleMoves(const Board& board) override;
};  

#endif

