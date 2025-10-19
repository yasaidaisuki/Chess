#include "king.h"
#include "position.h"


King::King(Position posn, int team, bool hasMoved): Piece{posn, team, KING, hasMoved} {} // Constructor

// Virtual Methods
std::vector<Position> King::possibleTakes(const Board& board) {
     std::vector<Position> takes;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     int newCol = posn.col;
     int newRow = posn.row;

     // Checking top row  
     if (++newRow <= 7) {
          for (int i = newCol - 1; i <= newCol + 1; i++) {
               if (i <= 7 && i >= 0 && pieces[newRow][i]->getTeam() == getTeam() * -1) takes.emplace_back(Position{newRow, i});
          }
     }

     // Checking middle row
     newRow = posn.row;
     if (newCol - 1 >= 0 && newCol - 1 <= 7 && pieces[newRow][newCol - 1]->getTeam() == getTeam() * -1) takes.emplace_back(Position{newRow, newCol - 1});
     if (newCol + 1 >= 0 && newCol + 1 <= 7 && pieces[newRow][newCol + 1]->getTeam() == getTeam() * -1) takes.emplace_back(Position{newRow, newCol + 1});     

     // Checking bottom row
     if (--newRow >= 0) {
          for (int i = newCol - 1; i <= newCol + 1; i++) {
               if (i <= 7 && i >= 0 && pieces[newRow][i]->getTeam() == getTeam() * -1) takes.emplace_back(Position{newRow, i});
          }
     }

     return takes;
}; 

std::vector<Position> King::possibleMoves(const Board& board) {
     std::vector<Position> moves;
     std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();
     int newCol = posn.col;
     int newRow = posn.row;

     // Checking top row  
     if (++newRow <= 7) {
          for (int i = newCol - 1; i <= newCol + 1; i++) {
               if (i <= 7 && i >= 0 && pieces[newRow][i]->getTeam() == 0) moves.emplace_back(Position{newRow, i});
          }
     }

     // Checking middle row
     newRow = posn.row;
     if (newCol - 1 >= 0 && newCol - 1 <= 7 && pieces[newRow][newCol - 1]->getTeam() == 0) moves.emplace_back(Position{newRow, newCol - 1});
     if (newCol + 1 >= 0 && newCol + 1 <= 7 && pieces[newRow][newCol + 1]->getTeam() == 0) moves.emplace_back(Position{newRow, newCol + 1});     

     // Checking bottom row
     if (--newRow >= 0) {
          for (int i = newCol - 1; i <= newCol + 1; i++) {
               if (i <= 7 && i >= 0 && pieces[newRow][i]->getTeam() == 0) moves.emplace_back(Position{newRow, i});
          }
     }

     // Check castling
     if (!board.isCheck(getTeam())) {
          int theRow = 0;
          if (getTeam() == -1) theRow = 7;
          std::vector<std::vector <std::shared_ptr<Piece>>> pieces = board.getPieces();

          if (!pieceMoved() && posn == Position{theRow, 4}) {
               if (!pieces[theRow][0]->pieceMoved() && pieces[theRow][0]->getType() == ROOK && pieces[theRow][0]->getTeam() == getTeam()) {  // check if left rook is castleable
                    if (pieces[theRow][1]->getType() == BLANK && pieces[theRow][2]->getType() == BLANK && pieces[theRow][3]->getType() == BLANK) { // check if there are pieces in between left rook and king
                         moves.emplace_back(Position{theRow, 2});
                    }
               }

               if (!pieces[theRow][7]->pieceMoved() && pieces[theRow][7]->getType() == ROOK && pieces[theRow][7]->getTeam() == getTeam()) {  // check if right rook is castleable
                    if (pieces[theRow][5]->getType() == BLANK && pieces[theRow][6]->getType() == BLANK) { // check if there are pieces in between right rook and king
                         moves.emplace_back(Position{theRow, 6});
                    }
               }
          }
     }

     return moves;
}

