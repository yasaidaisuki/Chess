#include "rook.h"
Rook::Rook(Position posn, int team, bool hasMoved): Piece{posn, team, ROOK, hasMoved} {} // Constructor

// Virtual Methods

std::vector<Position> Rook::possibleTakes(const Board& board) {
     std::vector<Position> takes;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     
     // up
     for (int i = posn.row + 1; i < 8 && pieces[i][posn.col]->getTeam() != getTeam(); i++) {
          // if hit a piece
          if (pieces[i][posn.col]->getTeam() == getTeam() * -1) {
               takes.emplace_back(Position{i,posn.col});
               break;
          }
     }
     // down
     for (int i = posn.row - 1; i >= 0 && pieces[i][posn.col]->getTeam() != getTeam(); i--) {
          if (pieces[i][posn.col]->getTeam() == getTeam() * -1) {
               takes.emplace_back(Position{i,posn.col});
               break;
          }
     }
     // left
     for (int i = posn.col - 1; i >= 0 && pieces[posn.row][i]->getTeam() != getTeam(); i--) {
          if (pieces[posn.row][i]->getTeam() == getTeam() * -1 ) {
               takes.emplace_back(Position{posn.row,i});
               break;
          }
     }
     // right
     for (int i = posn.col + 1; i < 8 && pieces[posn.row][i]->getTeam() != getTeam(); i++) {
          if (pieces[posn.row][i]->getTeam() == getTeam() * -1 ) {
               takes.emplace_back(Position{posn.row,i});
               break;
          }
     }
     return takes;
}

std::vector<Position> Rook::possibleMoves(const Board& board) {
     std::vector<Position> moves;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     
     // up
     for (int i = posn.row + 1; i < 8 
               && pieces[i][posn.col]->getTeam() == 0; i++) {
          moves.emplace_back(Position{i,posn.col});
     }
     // down
     for (int i = posn.row - 1; i >= 0 
               && pieces[i][posn.col]->getTeam() == 0; i--) {
          moves.emplace_back(Position{i,posn.col});
     }

     // left
     for (int i = posn.col - 1; i >= 0 
               && pieces[posn.row][i]->getTeam() == 0; i--) {
          moves.emplace_back(Position{posn.row, i});
     }
     // right
     for (int i = posn.col + 1; i < 8 
               && pieces[posn.row][i]->getTeam() == 0 ; i++) {
          moves.emplace_back(Position{posn.row, i});
     }
     return moves;
}
