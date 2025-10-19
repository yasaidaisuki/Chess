#include "pawn.h"
#include "position.h"
#include <iostream>

Pawn::Pawn(Position posn, int team, bool hasMoved): Piece{posn, team, PAWN, hasMoved} {} // Constructor
// Virtual Methods
std::vector<Position> Pawn::possibleTakes(const Board& board) {
     std::vector<Position> takes;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     //black team
     if(getTeam() == -1) {
          // left side take
          // within bounds 
          if ((posn.row - 1 >= 0 && posn.col - 1 >= 0) && 
                   // not on the same team
                    (pieces[posn.row -1][posn.col -1]->getTeam() == -1*getTeam() ||
                    // Enpassantable
                    (posn.row == board.getEnPassantRef().row && 
                    posn.col - 1 == board.getEnPassantRef().col))) {
               takes.emplace_back(Position{posn.row -1, posn.col -1});
          }
          // right side take
          if ((posn.row - 1 >= 0 && posn.col + 1 <= 7) &&
                   // not on the same team
                    (pieces[posn.row - 1][posn.col + 1]->getTeam() ==  -1 * getTeam() ||
                    // Enpassantable
                    (posn.row == board.getEnPassantRef().row && 
                    posn.col + 1 == board.getEnPassantRef().col))) {
               takes.emplace_back(Position{posn.row -1, posn.col + 1});
          }
          
     }
     else {
          // right side take
          // within bounds 
          if ((posn.row + 1 <= 7 && posn.col - 1 >= 0) &&
                   // not on the same team
                    (pieces[posn.row + 1 ][posn.col -1]->getTeam() == -1 * getTeam() ||
                    // Enpassantable
                    (posn.row == board.getEnPassantRef().row && 
                    posn.col - 1 == board.getEnPassantRef().col))) {
               takes.emplace_back(Position{posn.row + 1, posn.col -1});
          }
          // right side take
          if ((posn.row + 1 <= 7 && posn.col + 1 <= 7) &&
                   // not on the same team
                    (pieces[posn.row + 1 ][posn.col + 1]->getTeam() == -1 * getTeam() ||
                    // Enpassantable
                    (posn.row == board.getEnPassantRef().row && 
                    posn.col + 1 == board.getEnPassantRef().col))) {
               takes.emplace_back(Position{posn.row + 1, posn.col + 1});
          }
     }
     return takes;
}; 

std::vector<Position> Pawn::possibleMoves(const Board& board) {
     std::vector<Position> moves;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     // black team
     if (getTeam() == -1) {
          // checking 2 ahead
          // if within bounds (errors can happen where custom setup occurs) and checks for blank and has moved
          if ((posn.row - 2 >= 0 ) 
                    && pieces[posn.row-1][posn.col]->getType() == BLANK
                    && (pieces[posn.row-2][posn.col]->getType() == BLANK && !pieceMoved()) ) {
               moves.emplace_back(Position{posn.row-2, posn.col});
          }
          // checking 1 ahead
          // if within bounds & 1 step ahead is empty
          if ((posn.row - 1 >= 0 ) 
                    && (pieces[posn.row-1][posn.col]->getType() == BLANK) ) {
               moves.emplace_back(Position{posn.row-1, posn.col});
          }
     }
     // white team i.e getTeam() == 1
     else {
          // checking opposite bounds

          if ((posn.row + 2 <= 7 ) 
                    && pieces[posn.row+1][posn.col]->getType() == BLANK
                    && (pieces[posn.row+2][posn.col]->getType() == BLANK && !pieceMoved()) ) {
               moves.emplace_back(Position{posn.row+2, posn.col});
          }
          // checking 1 ahead
          // if within bounds & 1 step ahead is empty
          if ((posn.row + 1 <= 7 ) 
                    && (pieces[posn.row+1][posn.col]->getType() == BLANK) ) {
               moves.emplace_back(Position{posn.row+1, posn.col});
          }
     }
     return moves;
}

 bool Pawn::canPromote() {
     if (posn.row == 7) {
          return true;
     }
     return false;
 }

