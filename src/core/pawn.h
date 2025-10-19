#ifndef PAWN_H
#define PAWN_H

#include <vector>
#include "position.h"
#include "piece.h"

class Pawn : public Piece {
    friend class Board;
    public:
        Pawn(Position posn, int team, bool hasMoved); // Constructor
        // Virtual Methods
        std::vector<Position>  possibleTakes(const Board& board) override;
        std::vector<Position>  possibleMoves(const Board& board) override;
        // other methods
        bool canPromote();
};

#endif
