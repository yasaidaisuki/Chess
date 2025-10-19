#include "queen.h"

Queen::Queen(Position posn, int team, bool hasMoved): Piece{posn, team, QUEEN, hasMoved} {}

// Virtual Methods
std::vector<Position> Queen::possibleTakes(const Board& board) {
     std::vector<Position> takes;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     // bishop type moves
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
     // Rook type moves
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
}; 

std::vector<Position> Queen::possibleMoves(const Board& board) {
     std::vector<Position> moves;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();

     // bishop type moves
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
     // rook type moves
     // up
     for (int i = posn.row + 1; i < 8 
               && pieces[i][posn.col]->getTeam() == 0; i++ ) {
          moves.emplace_back(Position{i,posn.col});
     }
     // down
     for (int i = posn.row -1; i >= 0 
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
