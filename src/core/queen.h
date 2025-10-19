#ifndef QUEEN_H
#define QUEEN_H

#include <vector>
#include "position.h"
#include "piece.h"

class Queen : public Piece {
    public:
        Queen(Position posn, int team, bool hasMoved);
        std::vector<Position>  possibleTakes(const Board& board) override;
        std::vector<Position>  possibleMoves(const Board& board) override;
};

#endif
