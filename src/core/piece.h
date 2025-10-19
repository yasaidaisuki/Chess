#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "position.h"
#include "board.h"

class Board;

const char KING = 'K', QUEEN = 'Q', BISHOP = 'B';
const char ROOK = 'R', KNIGHT = 'N', PAWN = 'P';
const char BLANK = '_';

class Piece {
    int team; 
    char type;
    bool hasMoved;
    protected:
        const int rows = 8;
        const int cols = 8;
    friend class Board;
    public :
        Position posn;
        // Big 5
        Piece(Position posn, int team, char type, bool hasMoved): team{team}, type{type}, hasMoved{hasMoved}, posn{posn} {}
        Piece(const Piece &&other);
        ~Piece() = default;

        Piece &operator=(Piece &&other);
        // other
        std::vector<Position> virtual possibleTakes(const Board& board) = 0;
        std::vector<Position> virtual possibleMoves(const Board& board) = 0;
        // getter
        int getTeam() const {return team;}
        char getType() const {return type;}
        char getState() const;
        void moved() {hasMoved = true;}
        bool pieceMoved() {return hasMoved;}
};

#endif
