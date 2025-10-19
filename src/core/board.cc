#include <memory>
#include <vector>
#include <utility>
#include "board.h"
#include <iostream>
#include "blank.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"

Board::Board(Position blackKing, Position whiteKing, 
        std::vector<std::vector <std::shared_ptr<Piece>>> pieces, bool allowAtomic): state{0}, resignState{0}, blackKing{blackKing},
        whiteKing{whiteKing}, enPassantPawn{-10,-10}, allowAtomic{allowAtomic}, pieces{std::move(pieces)} {}

Board::Board(const Board &other): state{other.state}, resignState{other.resignState}, blackKing{other.blackKing}, 
          whiteKing{other.whiteKing}, enPassantPawn{other.enPassantPawn}, allowAtomic{other.allowAtomic} {
     pieces = const_cast<Board&>(other).clonePieces();
}

Board &Board::operator=(const Board &other) {
     state = other.state;
     resignState = other.resignState;
     blackKing = other.blackKing;
     whiteKing = other.whiteKing;
     allowAtomic = other.allowAtomic;
     enPassantPawn = other.enPassantPawn;
     pieces = const_cast<Board&>(other).clonePieces();
     return *this;
}

Board &Board::operator=(Board &&other) {
     std::swap(state, other.state);
     std::swap(resignState, other.resignState);
     std::swap(blackKing, other.blackKing);
     std::swap(whiteKing, other.whiteKing);
     std::swap(enPassantPawn, other.enPassantPawn);
     std::swap(allowAtomic, allowAtomic);
     std::swap(pieces, other.pieces);
     return *this;
}

void Board::promote(Position pos, char type, int team) {
     addPiece(pos, type, team, true);
}

void Board::killPiece(Position pos) {
     pieces[pos.row][pos.col] = std::make_shared<Blank>(Position{pos.row,pos.col}, 0, false);
}

void Board::addPiece(Position pos, char type, int team, bool hasMoved) {
     if (type == QUEEN) {
          pieces[pos.row][pos.col] = std::make_shared<Queen>(Position{pos.row,pos.col}, team, hasMoved);
     } else if (type == ROOK) {
          pieces[pos.row][pos.col] = std::make_shared<Rook>(Position{pos.row,pos.col}, team, hasMoved);
     } else if (type == BISHOP) { 
          pieces[pos.row][pos.col] = std::make_shared<Bishop>(Position{pos.row,pos.col}, team, hasMoved);
     } else if (type == KNIGHT) {
          pieces[pos.row][pos.col] = std::make_shared<Knight>(Position{pos.row,pos.col}, team, hasMoved);
     } else if (type == PAWN) {
          pieces[pos.row][pos.col] = std::make_shared<Pawn>(Position{pos.row,pos.col}, team, hasMoved);
     } else if (type == KING) {
          pieces[pos.row][pos.col] = std::make_shared<King>(Position{pos.row,pos.col}, team, hasMoved);
     }
}

bool Board::movePieces(Position currPos, Position nextPos, int team, bool printOut, bool &promote) {
      // Getting piece
     std::shared_ptr<Piece> &lePiece = pieces[currPos.row][currPos.col];
     
     // Check if right team
     if (team != lePiece->getTeam()) {
          if (printOut) std::cout << "That aint yo piece boi, go write the command again" << std:: endl;
          return false;
     }
     
     // Check if possible move
     int action = 0; // 0 = not possible, -1 = take, 1 = move, 2 = enpassant, 3 = castling
     for (auto moveposition : lePiece->possibleMoves(*this)) {
          if (moveposition == nextPos) {
               action = 1;
               break;
          }
     }

     if (action == 0){
          for (auto moveposition : lePiece->possibleTakes(*this)) {
               if (moveposition == nextPos && pieces[nextPos.row][nextPos.col]->getTeam() == 0) {
                    action = 2;
               } else if (moveposition == nextPos) {
                    action = -1;
               }
          }
     }
     
     if (action == 0) {
          if (printOut) std::cout << "BRO YOU DIDNT GIVE US A POSSIBLE PLACE TO PUT THE PIECE, RETRY" << std::endl;
          return false;
     } 

     // Store board things in case of undo
     Board thePast(*this);

     // Enpassant add pawn
     Position enPassantPastPawn = enPassantPawn;
     if (lePiece->getType() == PAWN && !lePiece->pieceMoved() && (nextPos.row - currPos.row == 2 || currPos.row - nextPos.row == 2)) {
          enPassantPawn = nextPos;
     } else {
          enPassantPawn = {-10, -10};
     }
     
     bool castleLeft = false;
     // Indicate we are castling
     if (pieces[currPos.row][currPos.col]->getType() == KING && (currPos.col - nextPos.col == 2 || nextPos.col - currPos.col == 2)) {
          action = 3;
          if (currPos.col - nextPos.col == 2) castleLeft = true;
          else castleLeft = false;
     }

     if (action == 1) {
          if (team == 1 && lePiece->getType() == KING) whiteKing = nextPos;
               else if (team == -1 && lePiece->getType() == KING) blackKing = nextPos;
               swapPieces(currPos, nextPos);
          // checking for promote
          if (pieces[nextPos.row][nextPos.col]->getType() == PAWN && (nextPos.row == 0 || nextPos.row == 7)) promote = true;
     } else if (action == -1) {
          if (allowAtomic) {
               if (team == 1 && nextPos.row - 1 <= whiteKing.row && nextPos.row + 1 >= whiteKing.row && nextPos.col + 1 >= whiteKing.col && nextPos.row - 1 <= whiteKing.col) {
                     std::cout << "You cannot kill your own king in the process" << std::endl;
                     return false;
               } else if (team == -1 && nextPos.row - 1 <= blackKing.row && nextPos.row + 1 >= blackKing.row && nextPos.col + 1 <= blackKing.col && nextPos.row - 1 >= blackKing.col) {
                    std::cout << "You cannot kill your own king in the process" << std::endl;
                    return false;
               }
               
               killPiece(currPos);
               killPiece(nextPos);
               for (int i = nextPos.row - 1; i <= nextPos.row + 1; i++) {
                    for (int j = nextPos.col - 1; j <= nextPos.col + 1; j++) {
                         if (i < 8 && i >= 0 && j < 8 && j >= 0 && pieces[i][j]->getType() != PAWN && pieces[i][j]->getType() != BLANK) {
                             if (pieces[i][j]->getType() == KING) {
                                   if (pieces[i][j]->getTeam() == 1) {
                                        whiteKing.row = -123;
                                        whiteKing.col = -123;
                                   }
                                   else {
                                        blackKing.row = -123;
                                        blackKing.col = -123;
                                   }
                             }
                             killPiece(Position{i,j});
                         }
                    }
               }
          } else {
               if (team == 1 && lePiece->getType() == KING) whiteKing = nextPos;
               else if (team == -1 && lePiece->getType() == KING) blackKing = nextPos;
               killPiece(nextPos);
               swapPieces(currPos, nextPos);
          }
          // checking for promote
          if (pieces[nextPos.row][nextPos.col]->getType() == PAWN && (nextPos.row == 0 || nextPos.row == 7)) promote = true;
     } else if (action == 2) {
          if (allowAtomic) {
               if (team == 1 && nextPos.row - 1 <= whiteKing.row && nextPos.row + 1 >= whiteKing.row && nextPos.col + 1 >= whiteKing.col && nextPos.row - 1 <= whiteKing.col) {
                     std::cout << "You cannot kill your own king in the process" << std::endl;
                     return false;
               } else if (team == -1 && nextPos.row - 1 <= blackKing.row && nextPos.row + 1 >= blackKing.row && nextPos.col + 1 <= blackKing.col && nextPos.row - 1 >= blackKing.col) {
                    std::cout << "You cannot kill your own king in the process" << std::endl;
                    return false;
               }
               
               killPiece(currPos);
               killPiece(nextPos);
               for (int i = nextPos.row - 1; i <= nextPos.row + 1; i++) {
                    for (int j = nextPos.col - 1; j <= nextPos.col + 1; j++) {
                         if (i < 8 && i >= 0 && j < 8 && j >= 0 && pieces[i][j]->getType() != PAWN && pieces[i][j]->getType() != BLANK) {
                             if (pieces[i][j]->getType() == KING) {
                                   if (pieces[i][j]->getTeam() == 1) {
                                        whiteKing.row = -123;
                                        whiteKing.col = -123;
                                   }
                                   else {
                                        blackKing.row = -123;
                                        blackKing.col = -123;
                                   }
                             }
                             killPiece(Position{i,j});
                         }
                    }
               }
          } else {
               killPiece(enPassantPastPawn);
               swapPieces(currPos, nextPos);
          }
          
     } else if (action == 3) {
          pieces[currPos.row][currPos.col]->moved();
          if (castleLeft) {
               swapPieces(currPos, nextPos);
               swapPieces(Position{currPos.row,0}, Position{currPos.row, nextPos.col + 1});
               if (team == 1) whiteKing.col = 2;
               else blackKing.col = 2;
          } else {
               swapPieces(currPos, nextPos);
               swapPieces(Position{currPos.row,7}, Position{currPos.row, nextPos.col - 1});
               if (team == 1) whiteKing.col = 6;
               else blackKing.col = 6;
          }
     }

     if (isCheck(team)) {
          if (printOut) std::cout << "brother in christ, you under check if you do this" << std::endl;
          *this = thePast;
          return false;
     }

     // Check for the rook pos in castling
     if (action == 3) {
          Board temp(*this);
          if (castleLeft) {
               if (team == 1) temp.whiteKing.col = 3;
               else temp.blackKing.col = 3;
          } else {
               if (team == 1) temp.whiteKing.col = 5;
               else temp.blackKing.col = 5;
          }

          if (temp.isCheck(team)) {
               if (printOut) std::cout << "your rook is under check when castling..." << std::endl;
               *this = thePast;
               return false;
          }
     }

     return true;
}   

void Board::swapPieces(Position currPos, Position nextPos) {
     pieces[currPos.row][currPos.col]->moved();
     std::swap(pieces[currPos.row][currPos.col]->posn, pieces[nextPos.row][nextPos.col]->posn);
     std::swap(pieces[currPos.row][currPos.col],pieces[nextPos.row][nextPos.col]);
}

bool Board::isCheck(int team) const {
     for (int i = 0; i < 8; i++) {
          for(int j = 0; j < 8; j++) {
               // if opposite team, then check for possible moves on the king
               if (pieces[i][j]->getTeam() == team*-1) {
                    for (auto moveposition : pieces[i][j]->possibleTakes(*this)) {
                         if (moveposition == (team == 1 ? whiteKing : blackKing)) {
                              return true;
                         }
                    } 
               }
          }
     }
     return false;
}

std::vector<std::vector<std::shared_ptr<Piece>>> Board::clonePieces() {
     
     std::vector<std::vector<std::shared_ptr<Piece>>> clone(8, std::vector<std::shared_ptr<Piece>>(8));
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               if (pieces[i][j]->getType() == BLANK) {
                    clone[i][j] = 
                         std::make_shared<Blank>(pieces[i][j]->posn, pieces[i][j]->getTeam(), pieces[i][j]->hasMoved);
               }
               else if (pieces[i][j]->getType() == KING) {
                    clone[i][j] = 
                         std::make_shared<King>(pieces[i][j]->posn, pieces[i][j]->getTeam(), pieces[i][j]->hasMoved);
               }
               else if (pieces[i][j]->getType() == QUEEN) {
                    clone[i][j] = 
                         std::make_shared<Queen>(pieces[i][j]->posn, pieces[i][j]->getTeam(), pieces[i][j]->hasMoved);
               }
               else if (pieces[i][j]->getType() == BISHOP) {
                    clone[i][j] = 
                         std::make_shared<Bishop>(pieces[i][j]->posn, pieces[i][j]->getTeam(), pieces[i][j]->hasMoved);
               }
               else if (pieces[i][j]->getType() == KNIGHT) {
                    clone[i][j] = 
                         std::make_shared<Knight>(pieces[i][j]->posn, pieces[i][j]->getTeam(), pieces[i][j]->hasMoved);
               }
               else if (pieces[i][j]->getType() == ROOK) {
                    clone[i][j] = 
                         std::make_shared<Rook>(pieces[i][j]->posn, pieces[i][j]->getTeam(), pieces[i][j]->hasMoved);
               }
               else if (pieces[i][j]->getType() == PAWN) {
                    clone[i][j] = 
                         std::make_shared<Pawn>(pieces[i][j]->posn, pieces[i][j]->getTeam(), pieces[i][j]->hasMoved);
               }
          }
     }
     return clone;
}

bool Board::isMoveThisPieceCheck(int team, int row, int col, std::vector<Position> nextMoves) const {
     Board leCopy(*this);
     bool promotion = false;
     for (auto nextPos : nextMoves) {
          Board lePast(leCopy);
          if (leCopy.movePieces(Position{row,col}, nextPos, team, false, promotion) && !leCopy.isCheck(team)) {
               leCopy = std::move(lePast);
               return false;
          }
          leCopy = lePast;
     }
     return true;
}

bool Board::nextMovesAreCheck(int team) const{
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               if (pieces[i][j]->getTeam() == team) {
                    if (!isMoveThisPieceCheck(team, i, j, pieces[i][j]->possibleTakes(*this)) ||
                         !isMoveThisPieceCheck(team, i, j, pieces[i][j]->possibleMoves(*this))
                    ) return false;
               }
          }
     }
     return true;
}

bool Board::isMate(int team) const {
      if (allowAtomic) {
          if (team == 1 && whiteKing.row == -123 && whiteKing.col == -123) return true;
          if (team == -1 && blackKing.row == -123 && blackKing.col == -123) return true;
      }
     if (!isCheck(team)) return false;
     return nextMovesAreCheck(team);
}

bool Board::isStalemate(int team) const {
     if (isCheck(team)) return false;
     return nextMovesAreCheck(team);
}

void Board::updateState(int team) {
     if (allowAtomic) {
          if (isMate(team)) {
               state = team * 2;
               return;
          } else if (isMate(team*-1)) {
               state = team * -2;
               return;
          }
          
     }
     
     if (resignState == 1) {
          state = 4;
          std::cout << "Black Wins!" << std::endl;
     }
     else if (resignState == -1) {
          state = -4;
          std::cout << "White Wins!" << std::endl;
     }
     else if (isCheck(1)) {
          state = 1;
          if (isMate(1)) {
               state = 2;
          } else std::cout << "White is in check." << std::endl;
     } else if (isCheck(-1)) {
          state = -1;
          if (isMate(-1)) {
               state = -2;
          } else std::cout << "Black is in check." << std::endl;

     } else if (isStalemate(team * -1)){
          state = 3; 
     } else {
          state = 0;
     }
     
}

void Board::resign(int team) {
     resignState = team;
}

