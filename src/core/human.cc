#include "human.h"
#include <iostream> 
#include <string>
#include <sstream>


const int WHITE_TEAM = 1;
const int BLACK_TEAM = -1;

Human::Human(int team, Board &board): Player{0, team, board} {}

bool getSquare(Position &pos);

bool Human::playerMove(bool allowUndo){
     std::string input;
     bool doneCommand = false;
     while (!doneCommand) {
          while (std::cin >> input && input != "move" && input != "resign") {
               if (allowUndo && input == "undo") break;
               std::cout << "Please enter 'move' first" << std::endl;
               if (std::cin.eof()) break;
          }

          if (allowUndo && input == "undo") {
               return true;
          } 

          if (input == "resign") {
               board.resign(getTeam());
               return false;
          }

          if (std::cin.eof()) break;

          Board pastBoard(board);

          Position currPos;
          Position nextPos;
          bool promote = false;
          
          if (!getSquare(currPos)) {
               std::cout << "Invalid First Square, write command again bozo" << std::endl << std::endl;
               continue;
          }

          if (!getSquare(nextPos)) {
               std::cout << "Invalid Second Square, write command again bozo" << std::endl << std::endl;
               continue;
          }

          if (!board.movePieces(currPos, nextPos, getTeam(), true, promote)) {
               continue;
          }
          
          std::string promoteInput;
          if (promote && std::cin >> promoteInput) {
               if (promoteInput.size() == 1 && (promoteInput[0] == KNIGHT || promoteInput[0] == QUEEN || 
                    promoteInput[0] == ROOK || promoteInput[0] == BISHOP))
                    board.promote(nextPos, promoteInput[0], getTeam());
               else {
                    board = std::move(pastBoard);
                    std::cout << "Promote to either {'Q', 'R', 'B', 'N'}" << std::endl << std::endl;
                    continue;
               }
          }
          doneCommand = true;
     }
     return false;
}

bool getSquare(Position &pos) {     
     char col;
     int row;
     std::string input;
     std::cin >> input;
     std::istringstream iss1{input};
     iss1 >> col >> row;
     
     if (col < 'a' || col > 'h' || row > 8 || row < 1) {
          std::cin.ignore();
          std::cin.clear();
          return false;
     }
     pos.col = col - 'a';
     pos.row = row - 1;

     return true;
}

