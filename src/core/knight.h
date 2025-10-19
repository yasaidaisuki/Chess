#ifndef KNIGHT_H
#define KNIGHT_H

#include <vector>
#include "position.h"
#include "piece.h"

class Knight : public Piece {
    public:
        Knight(Position posn, int team, bool hasMoved); // Constructor
        std::vector<Position>  possibleTakes(const Board& board) override;
        std::vector<Position>  possibleMoves(const Board& board) override;
        std::vector<Position> 
            getMoves(const std::vector<std::vector <std::shared_ptr<Piece>>>& pieces, int team);

#endif
};

