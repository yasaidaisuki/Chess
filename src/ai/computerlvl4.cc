#include "computerlvl4.h"
#include "piece.h"

// c random library
#include <random>
#include <iostream>

void addCheckMates(std::vector<Position>, Board &board, int row, int col, std::vector<Move> &theVec, int team);
void addAvoidCheckMate(int team, std::vector<Position> ourMoves, int i, int j, Board &board, std::vector<Move> &theVec, Move &checkMateMoves);
void moveItselfSafe(Board &board, int row, int col, int team, std::vector<Move> &theVec);
void fillSave(char type, int team, std::vector<Move> &theVec, Board &board, int row, int col);

bool Computerlvl4::playerMove(bool allowUndo) {
     std::vector<Move> avoidMate = {};
     std::vector<Move> saveQueen = {};
     std::vector<Move> saveRook = {};
     std::vector<Move> saveBishopKnight = {};
     std::vector<Move> savePawn = {};
     std::vector<Move> takeMoves = {};
     std::vector<Move> checkMoves = {}; 
     std::vector<Move> leftoverMoves = {};
     std::vector<Move> moves = {};
     std::vector<Move> mateMove = {};
     std::string input;
     // just here. bot will only move after you type move and undo if allowed
     while(std::cin >> input && input != "move" && input != "resign") {
          if (allowUndo && input == "undo") break; 
     }
     if (input == "undo") return true;
     if (input == "resign") {
               board.resign(getTeam());
               return false;
     }

     std::vector<std::vector<std::shared_ptr<Piece>>> boardPieces = board.getPieces();

     // Store all checkmate moves
     std::vector<Move> enemyCheckMateMoves = {};
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               if (boardPieces[i][j]->getTeam() == getTeam() * -1) {
                    addCheckMates(boardPieces[i][j]->possibleMoves(board), board, i, j, enemyCheckMateMoves, getTeam());
                    addCheckMates(boardPieces[i][j]->possibleTakes(board), board, i, j, enemyCheckMateMoves, getTeam());
               }
          }
     }
     // See what moves can avoid checkmate
     for (auto checkMateMoves : enemyCheckMateMoves) {
          for (int i = 0; i < 8; i++) {
               for (int j = 0; j < 8; j++) {
                    if (boardPieces[i][j]->getTeam() == getTeam()) {
                         addAvoidCheckMate(getTeam(), boardPieces[i][j]->possibleMoves(board), i, j, board, avoidMate, checkMateMoves);
                         addAvoidCheckMate(getTeam(), boardPieces[i][j]->possibleTakes(board), i, j, board, avoidMate, checkMateMoves);
                    }
                         
               }
          }       
     }

     std::vector<Move> capturingMoves = {};
     // Getting all moves that can capture a piece
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               if (boardPieces[i][j]->getTeam() == getTeam() * -1) {
                    for (auto nextPos : boardPieces[i][j]->possibleTakes(board)) {
                         Board temp(board);
                         bool random = false;
                         if (temp.movePieces(Position{i,j}, nextPos, getTeam() * -1, false, random)) capturingMoves.emplace_back(Move(Position{i,j}, nextPos));
                    }
               }
          }
     }

     
     // Ways to save pieces
     for (auto attackMove : capturingMoves) {
          int row = attackMove.getNextPosn().row;
          int col = attackMove.getNextPosn().col;
          Board temp(board);
          if (boardPieces[row][col]->getType() == QUEEN) {
               fillSave(QUEEN, getTeam(), saveQueen, temp, row, col);
          } if (boardPieces[row][col]->getType() == PAWN) {
               fillSave(PAWN, getTeam(), savePawn, temp, row, col);
          } if (boardPieces[row][col]->getType() == KNIGHT) {
               fillSave(KNIGHT, getTeam(), saveBishopKnight, temp, row, col);
          } if (boardPieces[row][col]->getType() == BISHOP) {
               fillSave(BISHOP, getTeam(), saveBishopKnight, temp, row, col);
          } if (boardPieces[row][col]->getType() == ROOK) {
               fillSave(ROOK, getTeam(), saveRook, temp, row, col);
          } 
     }
     
     // Adding checkmate stuff
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               if (boardPieces[i][j]->getTeam() == getTeam()) {
                    for (auto leMoves : boardPieces[i][j]->possibleMoves(board)){
                         Board temp(board);
                         bool promote = false;
                         if (temp.movePieces(Position{i,j}, leMoves, getTeam(), false, promote)) {
                              if (promote) {
                                   temp.promote(leMoves, QUEEN, getTeam());
                                   if (temp.isMate(getTeam() * -1)) mateMove.emplace_back(Position{i,j}, leMoves, true);
                                   temp.promote(leMoves, KNIGHT, getTeam());
                                   if (temp.isMate(getTeam() * -1)) mateMove.emplace_back(Position{i,j}, leMoves, false);
                              } else {
                                   if (temp.isMate(getTeam() * -1)) mateMove.emplace_back(Position{i,j}, leMoves, false);
                              }
                         }
                    }
                    for (auto leMoves : boardPieces[i][j]->possibleTakes(board)){
                         Board temp(board);
                         bool promote = false;
                         if (temp.movePieces(Position{i,j}, leMoves, getTeam(), false, promote)) {
                              if (promote) {
                                   temp.promote(leMoves, QUEEN, getTeam());
                                   if (temp.isMate(getTeam() * -1)) mateMove.emplace_back(Position{i,j}, leMoves, true);
                                   temp.promote(leMoves, KNIGHT, getTeam());
                                   if (temp.isMate(getTeam() * -1)) mateMove.emplace_back(Position{i,j}, leMoves, false);
                              } else {
                                   if (temp.isMate(getTeam() * -1)) mateMove.emplace_back(Position{i,j}, leMoves, false);
                              }
                         }
                    }
               }
          }
     }

   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
         if (board.getPieces()[i][j]->getTeam() == getTeam() ) {
            // storing moves
            for (auto movesposition: board.getPieces()[i][j]->possibleTakes(board)) {
               Board temp(board);
               bool promotion = false;
               // validates move
               if (temp.movePieces(Position{i,j}, movesposition, getTeam(), false, promotion)) {
                     // if move puts opposing into check
                     if (promotion) {
                         temp.promote(movesposition, KNIGHT, getTeam());
                         if (temp.isCheck(getTeam()*-1)) takeMoves.emplace_back(Position{i,j}, movesposition, false);
                         temp.promote(movesposition, QUEEN, getTeam());
                         if (temp.isCheck(getTeam()*-1)) takeMoves.emplace_back(Position{i,j}, movesposition, true);
                     } else if (temp.isCheck(getTeam()*-1)) takeMoves.emplace_back(Position{i,j}, movesposition);
                     else {
                        takeMoves.emplace_back(Position{i,j}, movesposition);
                    }
               }
            }
            // storing attacks 
            for (auto movesposition: board.getPieces()[i][j]->possibleMoves(board)) {
               Board temp(board);
               bool promotion = false;
               // validating move
                if (temp.movePieces(Position{i,j}, movesposition, getTeam(), false, promotion)) {
                     // if move puts opposing into check
                     if (promotion) {
                         temp.promote(movesposition, KNIGHT, getTeam());
                         if (temp.isCheck(getTeam()*-1)) checkMoves.emplace_back(Position{i,j}, movesposition, false);
                         temp.promote(movesposition, QUEEN, getTeam());
                         if (temp.isCheck(getTeam()*-1)) checkMoves.emplace_back(Position{i,j}, movesposition, true);
                     } else if (temp.isCheck(getTeam()*-1)) checkMoves.emplace_back(Position{i,j}, movesposition);
                     else {
                        moves.emplace_back(Position{i,j}, movesposition);
                    }
               }
            } 
         }
      }
   }

   srand(time(NULL));
   bool promotion = false;
   // prio 1 capturing
   if(!mateMove.empty()) {
     makeMove(mateMove, promotion, 1);
   }
   
   else if (!avoidMate.empty()) {
     makeMove(avoidMate, promotion, 1);
   }
   
   else if (!saveQueen.empty()) {
      makeMove(saveQueen, promotion, 1);
   }

   else if (!saveRook.empty()) {
      makeMove(saveRook, promotion, 1);
   }

   else if (!saveBishopKnight.empty()) {
      makeMove(saveBishopKnight, promotion, 1);
   }

   else if (!takeMoves.empty()) {
      makeMove(takeMoves, promotion, 1);
   }
   // prio 2 checking
   else if (!checkMoves.empty()) {
      makeMove(checkMoves, true, 1);
   }
   // prio 3 simple move
   else if (!moves.empty()){
      makeMove(moves, promotion, 0);
   }
   return false;
}
// for order: 0 = random, 1 = attack first, 2 = attack order
void Computerlvl4::makeMove(std::vector<Move> &moveSet, bool promotion, int order) {
   if (promotion) {
     bool promoteQueen = false;
     for (auto move : moveSet) {
          int row = move.getNextPosn().row;
          int col = move.getNextPosn().col;
               if (getTeam() == 1 && row <= board.getBlack().row + 1 && row >= board.getBlack().row - 1 && col <= board.getBlack().col + 1 && col >= board.getBlack().col - 1) continue;
               if (getTeam() == 0 && row <= board.getWhite().row + 1 && row >= board.getWhite().row - 1 && col <= board.getWhite().col + 1 && col >= board.getWhite().col - 1) continue;
               bool promote = false;
               bool temp = board.movePieces(move.getCurrPosn(), move.getNextPosn(), getTeam(), false, promote);
               
               if (promote == true) {
                    if (!move.getPromoteQueen()) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                    else {
                         // first promotes to queen
                         board.promote(move.getNextPosn(), QUEEN, getTeam());
                         // prioritizes checking
                         // thus if queen does not check, then we will see if knight checks
                         if (!board.isCheck(getTeam() * -1)) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                         // if knight does not check, simply promote to queen
                         if (!board.isCheck(getTeam() * -1)) {
                              board.promote(move.getNextPosn(), QUEEN, getTeam());
                              promoteQueen = true;
                         }
                    }
               }
               // temp does not matter in this case, it will always print
               if (temp) std::cout << move.getCurrPosn() << " to " << move.getNextPosn();
               if (promotion) std::cout << ": Promote to " << (promoteQueen ? "Queen" : "Knight") << std::endl;
               return;
       }
   }
   
   if (order == 1) {
          bool promoteQueen = false;
          for (auto move : moveSet) {
               if (board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getTeam() == getTeam() * -1 && board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getType() == QUEEN) {
                    bool temp = board.movePieces(move.getCurrPosn(), move.getNextPosn(), getTeam(), false, promotion);
                    if (promotion == true) {
                         if (!move.getPromoteQueen()) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                         else {
                              // first promotes to queen
                              board.promote(move.getNextPosn(), QUEEN, getTeam());
                              // prioritizes checking
                              // thus if queen does not check, then we will see if knight checks
                              if (!board.isCheck(getTeam() * -1)) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                              // if knight does not check, simply promote to queen
                              if (!board.isCheck(getTeam() * -1)) {
                                   board.promote(move.getNextPosn(), QUEEN, getTeam());
                                   promoteQueen = true;
                              }
                         }
                    }
                    // temp does not matter in this case, it will always print
                    if (temp) std::cout << move.getCurrPosn() << " to " << move.getNextPosn();
                    if (promotion) std::cout << ": Promote to " << (promoteQueen ? "Queen" : "Knight") << std::endl;
                    return;
               }
          }
          for (auto move : moveSet) {
               if (board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getTeam() == getTeam() * -1 && board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getType() == ROOK) {
                    bool temp = board.movePieces(move.getCurrPosn(), move.getNextPosn(), getTeam(), false, promotion);
                    if (promotion == true) {
                         if (!move.getPromoteQueen()) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                         else {
                              // first promotes to queen
                              board.promote(move.getNextPosn(), QUEEN, getTeam());
                              // prioritizes checking
                              // thus if queen does not check, then we will see if knight checks
                              if (!board.isCheck(getTeam() * -1)) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                              // if knight does not check, simply promote to queen
                              if (!board.isCheck(getTeam() * -1)) {
                                   board.promote(move.getNextPosn(), QUEEN, getTeam());
                                   promoteQueen = true;
                              }
                         }
                    }
                    // temp does not matter in this case, it will always print
                    if (temp) std::cout << move.getCurrPosn() << " to " << move.getNextPosn();
                    if (promotion) std::cout << ": Promote to " << (promoteQueen ? "Queen" : "Knight") << std::endl;
                    return;
               }
          }
          for (auto move : moveSet) {
               if (board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getTeam() == getTeam() * -1 && 
                         (board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getType() == BISHOP || board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getType() == KNIGHT)) {
                    bool temp = board.movePieces(move.getCurrPosn(), move.getNextPosn(), getTeam(), false, promotion);
                    if (promotion == true) {
                         if (!move.getPromoteQueen()) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                         else {
                              // first promotes to queen
                              board.promote(move.getNextPosn(), QUEEN, getTeam());
                              // prioritizes checking
                              // thus if queen does not check, then we will see if knight checks
                              if (!board.isCheck(getTeam() * -1)) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                              // if knight does not check, simply promote to queen
                              if (!board.isCheck(getTeam() * -1)) {
                                   board.promote(move.getNextPosn(), QUEEN, getTeam());
                                   promoteQueen = true;
                              }
                         }
                    }
                    // temp does not matter in this case, it will always print
                    if (temp) std::cout << move.getCurrPosn() << " to " << move.getNextPosn();
                    if (promotion) std::cout << ": Promote to " << (promoteQueen ? "Queen" : "Knight") << std::endl;
                    return;
               }
          }
          for (auto move : moveSet) {
               if (board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getTeam() == getTeam() * -1 && 
                         (board.getPieces()[move.getNextPosn().row][move.getNextPosn().col]->getType() == PAWN)) {
                    bool temp = board.movePieces(move.getCurrPosn(), move.getNextPosn(), getTeam(), false, promotion);
                    if (promotion == true) {
                         if (!move.getPromoteQueen()) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                         else {
                              // first promotes to queen
                              board.promote(move.getNextPosn(), QUEEN, getTeam());
                              // prioritizes checking
                              // thus if queen does not check, then we will see if knight checks
                              if (!board.isCheck(getTeam() * -1)) board.promote(move.getNextPosn(), KNIGHT, getTeam());
                              // if knight does not check, simply promote to queen
                              if (!board.isCheck(getTeam() * -1)) {
                                   board.promote(move.getNextPosn(), QUEEN, getTeam());
                                   promoteQueen = true;
                              }
                         }
                    }
                    // temp does not matter in this case, it will always print
                    if (temp) std::cout << move.getCurrPosn() << " to " << move.getNextPosn();
                    if (promotion) std::cout << ": Promote to " << (promoteQueen ? "Queen" : "Knight") << std::endl;
                    return;
               }
          }
     }
     
     bool promoteQueen = false;
     int selectMove;
     if (moveSet.size() == 0) selectMove = 0;
     else selectMove = rand() % moveSet.size();
     bool temp = board.movePieces(moveSet[selectMove].getCurrPosn(), moveSet[selectMove].getNextPosn(), getTeam(), false, promotion);
     if (promotion == true) {
          if (!moveSet[selectMove].getPromoteQueen()) board.promote(moveSet[selectMove].getNextPosn(), KNIGHT, getTeam());
          else {
               // first promotes to queen
               board.promote(moveSet[selectMove].getNextPosn(), QUEEN, getTeam());
               // prioritizes checking
               // thus if queen does not check, then we will see if knight checks
               if (!board.isCheck(getTeam() * -1)) board.promote(moveSet[selectMove].getNextPosn(), KNIGHT, getTeam());
               // if knight does not check, simply promote to queen
               if (!board.isCheck(getTeam() * -1)) {
                    board.promote(moveSet[selectMove].getNextPosn(), QUEEN, getTeam());
                    promoteQueen = true;
               }
          }
     }
     // temp does not matter in this case, it will always print
     if (temp) std::cout << moveSet[selectMove].getCurrPosn() << " to " << moveSet[selectMove].getNextPosn();
     if (promotion) std::cout << ": Promote to " << (promoteQueen ? "Queen" : "Knight") << std::endl;
     return;
     // temp does not matter in this case, it will always print
     if (temp) std::cout << moveSet[selectMove].getCurrPosn() << " to " << moveSet[selectMove].getNextPosn();
     if (promotion) std::cout << ": Promote to " << (promoteQueen ? "Queen" : "Knight") << std::endl;
}

void addCheckMates(std::vector<Position> theMoves, Board &board, int i, int j, std::vector<Move> &theVec, int team){
     std::vector<std::vector<std::shared_ptr<Piece>>> boardPieces = board.getPieces();

     for (auto nextPos : theMoves) {
          Board temp(board);
          bool promotion = false;
          if (temp.movePieces(Position{i,j}, nextPos, team * -1, false, promotion)) {
               if (promotion) {
                    temp.promote(nextPos, QUEEN, team * -1);
                    if (temp.isMate(team)) theVec.emplace_back(Move(Position{i,j}, nextPos, true));
                    temp.promote(nextPos, KNIGHT, team * -1);
                    if (temp.isMate(team)) theVec.emplace_back(Move(Position{i,j}, nextPos, false));
               } else {
                    if (temp.isMate(team)) theVec.emplace_back(Move(Position{i,j}, nextPos, true));
               }
          }
     }
}

void checkCheckMate(Board &temp, Move checkMateMoves, int team, std::vector<Move> &theVec, int i, int j, Position nextPos){
     bool enemyPromote = false;
     if (!temp.movePieces(checkMateMoves.getCurrPosn(), checkMateMoves.getNextPosn(), -1 * team, false, enemyPromote)) {
          theVec.emplace_back(Move(Position{i,j}, nextPos));
          return;
     }

     if (enemyPromote) {
          if (checkMateMoves.getPromoteQueen()) {
               temp.promote(checkMateMoves.getNextPosn(), QUEEN, team);
               if (!temp.isMate(team)) theVec.emplace_back(Move(Position{i,j}, nextPos));
          } else {
               temp.promote(checkMateMoves.getNextPosn(), KNIGHT, team);
               if (!temp.isMate(team)) theVec.emplace_back(Move(Position{i,j}, nextPos, false));
          }

     }  else {
          if (!temp.isMate(team)) {
               theVec.emplace_back(Move(Position{i,j}, nextPos)); 
          }
     }
}

void addAvoidCheckMate(int team, std::vector<Position> ourMoves, int i, int j, Board &board, std::vector<Move> &theVec, Move &checkMateMoves) {
     for (auto nextPos : ourMoves) {
          Board temp(board);
          bool promote = false;
          if (temp.movePieces(Position{i,j}, nextPos, team, false, promote)) {
               if (promote) {
                    temp.promote(nextPos, QUEEN, team);
                    checkCheckMate(temp, checkMateMoves, team, theVec, i, j, nextPos);

                    temp = board;

                    temp.promote(nextPos, KNIGHT, team);;
                    checkCheckMate(temp, checkMateMoves, team, theVec, i, j, nextPos);
               } else {
                    checkCheckMate(temp, checkMateMoves, team, theVec, i, j, nextPos);
               }
          }
     }
}

bool isPieceTaken4(Board &board, int team, Position nextPos) {
     bool taken = false;
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               if (board.getPieces()[i][j]->getTeam() == team * -1) {
                    for (auto takes : board.getPieces()[i][j]->possibleTakes(board)) {
                         if (nextPos == takes) taken = true;
                    }
               }
          }
     }

     return taken;
}

void moveItselfSafe(Board &board, int row, int col, int team, std::vector<Move> &theVec) {
     for (auto nextPos: board.getPieces()[row][col]->possibleMoves(board)) {
               Board temp(board);
               bool promote = false;
               if (temp.movePieces(Position{row,col}, nextPos, team, false, promote)) {
                    if (!isPieceTaken4(temp, team, nextPos)) theVec.emplace_back(Move(Position{row,col}, nextPos));
               }
     }
     for (auto nextPos: board.getPieces()[row][col]->possibleTakes(board)) {
          Board temp(board);
          bool promote = false;
          if (temp.movePieces(Position{row,col}, nextPos, team, false, promote)) {
               if (!isPieceTaken4(temp, team, nextPos)) theVec.emplace_back(Move(Position{row,col}, nextPos));
          }
     }
}

void fillSave(char type, int team, std::vector<Move> &theVec, Board &board, int row, int col) {
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               if (i == row && j == col) moveItselfSafe(board, row, col, team, theVec);
               else if (!(board.getPieces()[i][j]->getTeam() == team)) continue;
               else if (board.getPieces()[row][col]->getType() != QUEEN && board.getPieces()[i][j]->getType() == QUEEN) continue;
               else if (board.getPieces()[row][col]->getType() == ROOK && board.getPieces()[i][j]->getType() == ROOK) continue;
               else if ((board.getPieces()[row][col]->getType() == BISHOP || board.getPieces()[row][col]->getType() == KNIGHT) && (
                         board.getPieces()[i][j]->getType() == ROOK || board.getPieces()[i][j]->getType() == QUEEN)) continue;
               else if (board.getPieces()[row][col]->getType() == PAWN) continue;
               else {
                    for (auto nextPos : board.getPieces()[i][j]->possibleTakes(board)) {
                         Board temp(board);
                         bool promote = false;
                         if (temp.movePieces(Position{i,j}, nextPos, team, false, promote)) {
                              if (promote) {
                                   temp.promote(nextPos, QUEEN, team);
                                   if (!isPieceTaken4(temp, team, Position{row,col})) theVec.emplace_back(Move(Position{i,j}, nextPos, true));
                                   temp.promote(nextPos, KNIGHT, team);
                                   if (!isPieceTaken4(temp, team, Position{row,col})) theVec.emplace_back(Move(Position{i,j}, nextPos, false));
                              } else {
                                   if (!isPieceTaken4(temp, team, Position{row,col})) theVec.emplace_back(Move(Position{i,j}, nextPos, true));
                              }
                              
                         }
                    }
                    for (auto nextPos : board.getPieces()[i][j]->possibleMoves(board)) {
                         Board temp(board);
                         bool promote = false;
                         if (temp.movePieces(Position{i,j}, Position{nextPos.row, nextPos.col}, team, false, promote)) {
                              if (promote) {
                                   temp.promote(nextPos, QUEEN, team);
                                   if (!isPieceTaken4(temp, team, Position{row,col})) theVec.emplace_back(Move(Position{i,j}, nextPos, true));
                                   temp.promote(nextPos, KNIGHT, team);
                                   if (!isPieceTaken4(temp, team, Position{row,col})) theVec.emplace_back(Move(Position{i,j}, nextPos, false));
                              } else {
                                   if (!isPieceTaken4(temp, team, Position{row,col})) theVec.emplace_back(Move(Position{i,j}, nextPos, true));
                              }
                              
                         }
                    }                     
               }
          }
     }
}

