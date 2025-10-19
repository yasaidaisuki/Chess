#include "bishop.h"

Bishop::Bishop(Position posn, int team, bool hasMoved): Piece{posn, team, BISHOP, hasMoved} {} // Constructor

// Virtual Methods
std::vector<Position> Bishop::possibleTakes(const Board& board) {
     std::vector<Position> takes;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     // up left
     for (int i = 1; (posn.row + i <= 7 && posn.col - i >=0); i++) {
          if (pieces[posn.row + i][posn.col - i]->getTeam() != 0) {
               if (pieces[posn.row + i][posn.col - i]->getTeam() == getTeam() * -1 )
                    takes.emplace_back(Position{posn.row + i,posn.col - i});
               break;
          }
     }
     // up right
     for (int i = 1; (posn.row + i <= 7 && posn.col + i <= 7); i++) {
          if (pieces[posn.row + i][posn.col + i]->getTeam() != 0) {
               if (pieces[posn.row + i][posn.col + i]->getTeam() == getTeam() * -1 )
                    takes.emplace_back(Position{posn.row + i,posn.col + i});
               break;
          }
     }
     // down left
     for (int i = 1; (posn.row - i >= 0 && posn.col - i >=0); i++) {
          if (pieces[posn.row - i][posn.col - i]->getTeam() != 0) {
               if (pieces[posn.row - i][posn.col - i]->getTeam() == getTeam() * -1 )
                    takes.emplace_back(Position{posn.row - i,posn.col - i});
               break;
          }
     }
     // down right
     for (int i = 1; (posn.row - i >= 0 && posn.col + i <=7); i++) {
          if (pieces[posn.row - i][posn.col + i]->getTeam() != 0) {
               if (pieces[posn.row - i][posn.col + i]->getTeam() == getTeam() * -1 )
                    takes.emplace_back(Position{posn.row - i,posn.col + i});
               break;
          }
     }

     return takes;
}; 

std::vector<Position> Bishop::possibleMoves(const Board& board) {
     std::vector<Position> moves;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     // up left
     for (int i = 1; (posn.row + i <= 7 && posn.col - i >=0)
               && pieces[posn.row + i][posn.col - i]->getTeam() == 0; i++) {
          moves.emplace_back(Position{posn.row + i, posn.col - i});
     }
     // up right
     for (int i = 1; (posn.row + i <= 7 && posn.col + i <= 7)
               && pieces[posn.row + i][posn.col + i]->getTeam() == 0; i++) {
          moves.emplace_back(Position{posn.row + i, posn.col + i});          
     }
     // down left
     for (int i = 1; (posn.row - i >= 0 && posn.col - i >=0) 
               && pieces[posn.row - i][posn.col - i]->getTeam() == 0; i++) {
          moves.emplace_back(Position{posn.row - i, posn.col - i});            
     }
     // down right
     for (int i = 1; (posn.row - i >= 0 && posn.col + i <= 7) 
                && pieces[posn.row - i][posn.col + i]->getTeam() == 0; i++) {
          moves.emplace_back(Position{posn.row - i, posn.col + i});
     }

     return moves;
}

