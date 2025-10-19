#include "knight.h"
#include "position.h"

Knight::Knight(Position posn, int team, bool hasMoved): Piece{posn, team, KNIGHT, hasMoved} {} // Constructor

// Virtual Methods
std::vector<Position> Knight::possibleTakes(const Board& board) {
     std::vector<Position> takes;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     // black team
     if (getTeam() == -1) takes = getMoves(pieces, 1);
     // white team
     else takes = getMoves(pieces, -1);
     return takes;
}

std::vector<Position> Knight::possibleMoves(const Board& board) {
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     // can only make moves on neutral squares
     std::vector<Position> moves = getMoves(pieces, 0);
     return moves;
}

std::vector<Position> 
          Knight::getMoves(const std::vector<std::vector <std::shared_ptr<Piece>>>& pieces, int team) {
     std::vector<Position> moves;
     // moving backwards
     // (x - 1, y + 2) 
     // if within bounds (errors can happen where custom setup occurs) and checks for blank and has moved
     if ( (posn.row + 2 <= 7 && posn.col - 1 >=0) 
               && (pieces[posn.row + 2][posn.col -1 ]->getTeam() == team) ) {
          moves.emplace_back(Position{posn.row + 2, posn.col -1 });
     }
     // (x + 1, y + 2)
     if ( (posn.row + 2 <= 7 && posn.col + 1 <= 7) 
               && (pieces[posn.row + 2][posn.col + 1 ]->getTeam() == team) ) {
          moves.emplace_back(Position{posn.row + 2, posn.col + 1 });
     }
     // (x - 2, y + 1)
     if ( (posn.row + 1 <= 7 && posn.col - 2 >= 0) 
               && (pieces[posn.row + 1][posn.col -2 ]->getTeam() == team) ) {
          moves.emplace_back(Position{posn.row + 1, posn.col -2 });
     }
     // (x + 2, y + 1)
     if ( (posn.row + 1 <= 7 && posn.col + 2 <= 7) 
               && (pieces[posn.row + 1][posn.col + 2 ]->getTeam() == team) ) {
          moves.emplace_back(Position{posn.row + 1, posn.col + 2 });
     }
     // moving foward
     // (x - 1, y - 2) 
     // if within bounds (errors can happen where custom setup occurs) and checks for blank and has moved
     if ( (posn.row - 2 >= 0 && posn.col - 1 >=0) 
               && (pieces[posn.row - 2][posn.col -1 ]->getTeam() == team) ) {
          moves.emplace_back(Position{posn.row - 2, posn.col -1 });
     }
     // (x + 1, y - 2)
     if ( (posn.row - 2 >= 0 && posn.col + 1 <= 7) 
               && (pieces[posn.row - 2][posn.col + 1 ]->getTeam() == team) ) {
          moves.emplace_back(Position{posn.row - 2, posn.col + 1 });
     }
     // (x - 2, y - 1)
     if ( (posn.row - 1 >= 0 && posn.col - 2 >= 0) 
               && (pieces[posn.row - 1][posn.col -2 ]->getTeam() == team) ) {
          moves.emplace_back(Position{posn.row - 1, posn.col -2 });
     }
     // (x + 2, y - 1)
     if ( (posn.row - 1 >= 0 && posn.col + 2 <= 7) 
               && (pieces[posn.row - 1][posn.col + 2 ]->getTeam() == team) ) {
          moves.emplace_back(Position{posn.row - 1, posn.col + 2 });
     }
     return moves;
}

