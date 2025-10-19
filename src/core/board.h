#ifndef BOARD_H
#define BOARD_H

#include <memory>
#include <vector>

#include "piece.h"
#include "position.h"

class Piece;

// concrete subject 
class Board {
    int state;  // 0 = neutral, 1 = white check, -1 = black check, -2 = black checkmate, 2 = white checkmate, 3 = stalemate, -4 = black resign, 4 = white reign
    int resignState; // 0 = neutral, 1 = white resign, -1 = black resign
    Position blackKing, whiteKing, enPassantPawn; 
    bool allowAtomic;
    std::vector<std::vector <std::shared_ptr<Piece>>> pieces;
    public :
        // Big 5 
        Board(Position blackKing, Position whiteKing, 
        std::vector<std::vector <std::shared_ptr<Piece>>> pieces, bool allowAtomic);
        Board(const Board &other);
        Board &operator=(const Board &other);
        Board &operator=(Board &&other);
        ~Board() = default;
        
        // Other methods
        bool movePieces(Position current, Position next, int team, bool printOut, bool &promote);
        void promote(Position pos, char type, int team);
        void updateState(int team);
        bool isCheck(int team) const;
        bool isMate(int team) const;
        bool isStalemate(int team) const;
        void swapPieces(Position currPos, Position nextPos);
        void addPiece(Position pos, char type, int team, bool hasMoved);
        void killPiece(Position pos);
        bool nextMovesAreCheck(int team) const;
        bool isMoveThisPieceCheck(int team, int row, int col, std::vector<Position> nextMoves) const;
        void undo(int moves);
        void resign(int team);
        
        std::vector<std::vector<std::shared_ptr<Piece>>> clonePieces();
        // Getters 
        const std::vector<std::vector <std::shared_ptr<Piece>>>& getPieces() const {return pieces;}
        const Position &getEnPassantRef() const {return enPassantPawn;}
        int getState() {return state;}

        // setters 
        void setBlackKing(Position p) {blackKing = p;}
        void setWhiteKing(Position p) {whiteKing = p;}
        Position getBlack() {return blackKing;}
        Position getWhite() {return whiteKing;}

};

#endif
