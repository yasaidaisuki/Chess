#include "computerlvl3.h"
#include "piece.h"

// c random library
#include <random>
#include <iostream>

bool isPieceTaken(Board &temp, int, Position);

bool Computerlvl3::playerMove(bool allowUndo) {
     std::vector<Move> takeMoves = {};
     std::vector<Move> checkMoves = {}; 
     std::vector<Move> moves = {};
     std::vector<Move> avoidCapture = {};
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
     int team = getTeam(); 

     std::vector<Move> capturingMoves = {};
     // Getting all moves that can capture a piece
     for (int i = 0; i < 8; i++) {
          for (int j = 0; j < 8; j++) {
               if (boardPieces[i][j]->getTeam() == team * -1) {
                    for (auto nextPos : boardPieces[i][j]->possibleTakes(board)) {
                         Board temp(board);
                         bool random = false;
                         if (temp.movePieces(Position{i,j}, nextPos, team * -1, false, random)) capturingMoves.emplace_back(Move(Position{i,j}, nextPos));
                    }
               }
          }
     }

     // Getting all moves that avoids all the capture a piece move
     for (auto takeMove : capturingMoves) {
          int row = takeMove.getNextPosn().row;
          int col = takeMove.getNextPosn().col;

          for (auto nextPos: boardPieces[row][col]->possibleMoves(board)) {
               Board temp(board);
               bool promote = false;
               if (temp.movePieces(Position{row,col}, nextPos, team, false, promote)) {
                    if (!isPieceTaken(temp, team, nextPos)) avoidCapture.emplace_back(Move(Position{row,col}, nextPos));
               }
          }
          for (auto nextPos: boardPieces[row][col]->possibleTakes(board)) {
               Board temp(board);
               bool promote = false;
               if (temp.movePieces(Position{row,col}, nextPos, team, false, promote)) {
                    if (!isPieceTaken(temp, team, nextPos)) avoidCapture.emplace_back(Move(Position{row,col}, nextPos));
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

     std::vector<Move> nonSuicideMoves = {};
     // remove suicide option in moves
     for (auto it = moves.begin(); it != moves.end(); ) {
          if (moves.size() == 1) break;
               Board temp(board);
               bool promote;
               temp.movePieces(it->getCurrPosn(), it->getNextPosn(), getTeam(), false, promote);
          if (isPieceTaken(temp, getTeam(), it->getNextPosn())) {
               it = moves.erase(it);  // erase returns the next iterator
          } else {
               ++it;  // only increment if not erasing
          }
     }
     srand(time(NULL));
     bool promotion = false;
     // prio 1 avoiding
     if (!avoidCapture.empty()) {
          makeMove(avoidCapture, promotion);
     }
     // prio 2 capturing
     else if (!takeMoves.empty()) {
          makeMove(takeMoves, promotion);
     }
     // prio 3 checking
     else if (!checkMoves.empty()) {
          makeMove(checkMoves, promotion);
     }
     // prio 4 simple move
     else if (!moves.empty()){
          makeMove(moves, promotion);
     }
     return false;
}

void Computerlvl3::makeMove(std::vector<Move> &moveSet, bool promotion) {
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
}


bool isPieceTaken(Board &board, int team, Position nextPos) {
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

