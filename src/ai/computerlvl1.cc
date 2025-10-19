#include "computerlvl1.h"

// c random library
#include <random>
#include <iostream>

bool Computerlvl1::playerMove(bool allowUndo) {
     std::vector<Move> moves = {};
     std::string input = {};
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
                    for (auto movesposition: board.getPieces()[i][j]->possibleMoves(board)) {
                         moves.emplace_back(Position{i,j}, Position{movesposition.row, movesposition.col});
                    }
                    // storing attacks 
                    for (auto movesposition: board.getPieces()[i][j]->possibleTakes(board)) {
                         moves.emplace_back(Position{i,j}, Position{movesposition.row, movesposition.col});
                    } 
               }
          }
     }
     srand(time(NULL));
     int selectMove = 0; 

     if (!moves.empty()) selectMove = rand() % moves.size();

     bool promotion = false;
     // validates moves and removes invalid moves
     while (!board.movePieces(moves[selectMove].getCurrPosn(),moves[selectMove].getNextPosn()
          , getTeam(), false, promotion)) {
          moves.erase(moves.begin() + selectMove);
          if (!moves.empty()) selectMove = rand() % moves.size();
          else selectMove = 0;
          promotion = false;
     }
     // if can promote, always promote to queen
     if (promotion == true) board.promote(moves[selectMove].getNextPosn(), QUEEN, getTeam());
     std::cout << moves[selectMove].getCurrPosn() << " to " << moves[selectMove].getNextPosn() << std::endl;
     return false;
}
