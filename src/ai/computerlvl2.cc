#include "computerlvl2.h"

// c random library
#include <random>
#include <iostream>

bool Computerlvl2::playerMove(bool allowUndo) {
   std::vector<Move> moves = {};
   std::vector<Move> takeMoves = {};
   std::vector<Move> checkMoves = {};
   std::string input;
   // just here. bot will only move after you type move and undo if allowed
   while(std::cin >> input && input != "move" && input != "resign") {
      if (allowUndo && input == "undo") break; 
   }
   if (allowUndo && input == "undo") return true;
   if (input == "resign") {
          board.resign(getTeam());
          return false;
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
   if (!takeMoves.empty()) {
      makeMove(takeMoves, promotion);
   }
   // prio 2 checking
   else if (!checkMoves.empty()) {
      makeMove(checkMoves, promotion);
   }
   // prio 3 simple move
   else if (!moves.empty()){
      makeMove(moves, promotion);
   }
   return false;
} 

void Computerlvl2::makeMove(std::vector<Move> &moveSet, bool promotion) {
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

