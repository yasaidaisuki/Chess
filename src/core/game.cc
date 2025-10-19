#include <iostream>
#include <sstream>
#include <memory>
#include <utility>
#include <string>
#include <vector>
#include <iomanip>
#include <deque>

//********************** CHESS OBJECTS **********************//

#include "board.h"
// players
#include "player.h"
#include "human.h"
#include "computer.h"
#include "computerlvl1.h"
#include "computerlvl2.h"
#include "computerlvl3.h"
#include "computerlvl4.h"
// observers
#include "observer.h"
#include "textObserver.h"
#include "graphics.h"
// pieces
#include "piece.h"
#include "position.h"
#include "knight.h"
#include "bishop.h"
#include "pawn.h"
#include "queen.h"
#include "king.h"
#include "rook.h"
#include "blank.h"

using namespace std;

const char LOWEST_COL = 'a';
const char HIGHEST_COL = 'h';
const int LOWSET_ROW = 1;
const int HIGHEST_ROW = 8;
const int WHITE_TEAM = 1;
const int BLACK_TEAM = -1;
const int NEUTRAL = 0;
const int NORMAL_BOARD = 0;
const int CHECK_BLACK = -1;
const int CHECKMATE_BLACK = -2;
const int CHECK_WHITE = 1;
const int CHECKMATE_WHITE = 2;
const int STALEMATE = 3;
  
const Position BLACK_KING_POS(7, 4);
const Position WHITE_KING_POS(0, 4);

// game methods
void setup(Board &board, vector<unique_ptr<Observer>> &observers, bool &turn);
void gameOver();

void defaultBoard(vector<vector<shared_ptr<Piece>>> &board);
string getInputs();

bool turnOnGraphics = true;
bool allowUndo = false;
bool allowAtomic = false;
int main(int argc, char *argv[]) {    
    for (int i = 1; i < argc; i++) {
          string str = argv[i];
          if (str == "-undo") allowUndo = true;
          if (str == "-atomic") allowAtomic = true;
    }

    if (allowUndo) std::cout << "UNDO HAS BEEN ALLOWED" << endl;
    if (allowAtomic) std::cout << "Welcome to atomic chess:" << endl << "The rules are simple. When you take a piece, it creates an explosion that kills surrounding non-pawns" << endl << "Checkmate or blow up the enemy king to win... GL" << endl << endl;;


    // for undoing
    deque<Board> pastBoards;
    vector<unique_ptr<Player>> thePlayers;

    vector<vector<shared_ptr<Piece>>> startBoard(ROW, vector<shared_ptr<Piece>>(COL));
    defaultBoard(startBoard);
    Board board(BLACK_KING_POS, WHITE_KING_POS, std::move(startBoard), allowAtomic);
    vector<unique_ptr<Observer>> observers;
    observers.emplace_back(make_unique<TextObserver>());
    if (turnOnGraphics) observers.emplace_back(make_unique<Graphics>(board));
    bool whiteMove = true;
     // Starting the prompts
    string input;
    std::cout <<  "------Main Menu------" << endl;
    while (cin >> input) {
          if (input == "game") {
              // Get players
              cout << endl << "Please enter what player 1 is:" << endl;
              string humanOrComputer = getInputs();
              if (std::cin.eof()) break;
              if (humanOrComputer == "human") thePlayers.emplace_back(make_unique<Human>(WHITE_TEAM, board));
              else if (humanOrComputer == "computer1") thePlayers.emplace_back(make_unique<Computerlvl1>(WHITE_TEAM, board));
              else if (humanOrComputer == "computer2") thePlayers.emplace_back(make_unique<Computerlvl2>(WHITE_TEAM, board));
              else if (humanOrComputer == "computer3") thePlayers.emplace_back(make_unique<Computerlvl3>(WHITE_TEAM, board));
              else if (humanOrComputer == "computer4") thePlayers.emplace_back(make_unique<Computerlvl4>(WHITE_TEAM, board));
              
              cout << endl << "Please enter what player 2 is:" << endl;              
              humanOrComputer = getInputs(); 
              if (std::cin.eof()) break;
              if (humanOrComputer == "human") thePlayers.emplace_back(make_unique<Human>(BLACK_TEAM, board));
              else if (humanOrComputer == "computer1") thePlayers.emplace_back(make_unique<Computerlvl1>(BLACK_TEAM, board));
              else if (humanOrComputer == "computer2") thePlayers.emplace_back(make_unique<Computerlvl2>(BLACK_TEAM, board));
              else if (humanOrComputer == "computer3") thePlayers.emplace_back(make_unique<Computerlvl3>(BLACK_TEAM, board));
              else if (humanOrComputer == "computer4") thePlayers.emplace_back(make_unique<Computerlvl4>(BLACK_TEAM, board));

              // Start game
              cout << endl << "-- Game Starting --" << endl;
              observers[0]->notify(board);
              if (turnOnGraphics) observers[1]->notify(board);
              
              if (!whiteMove) board.updateState(WHITE_TEAM);
              else board.updateState(BLACK_TEAM);
              // Loop until state isn't in a game over state
              while (board.getState() <= CHECK_WHITE && board.getState() >= CHECK_BLACK) {

                   // Make player move
                   Board pastBoard(board);
                   bool undo = false;
                   if (whiteMove) {
                         std::cout << "Player 1 Turn:" << std::endl;
                         undo = thePlayers[0]->playerMove(allowUndo);
                   } else {
                         std::cout << "Player 2 Turn:" << std::endl;
                         undo = thePlayers[1] -> playerMove(allowUndo);
                   }
                   if (!undo && allowUndo) {
                       pastBoards.push_front(pastBoard);
                   }
                   else if (allowUndo) {
                         if (pastBoards.empty()) {
                              cout << "sorry bud, this is the max u can go back" << endl << endl;
                              continue;
                         }
                         board = pastBoards.front();
                         pastBoards.pop_front();
                         cout << "undoing" << endl;
                   }
                   
                   if (std::cin.eof()) break;
                   // Redraw board
                   cout << endl;
                   observers[0]->notify(board);
                   if (turnOnGraphics) observers[1]->notify(board);
                   
                   // Update board state
                   if (whiteMove) board.updateState(WHITE_TEAM);
                   else board.updateState(BLACK_TEAM);

                   // Swap turns
                   whiteMove = !whiteMove;

                   // Add newline
                   cout << endl;
              }
              int leState = board.getState();
              if (leState == -2) {
                    cout << "Checkmate! White wins!" << endl;
                    thePlayers[0]->addScore(1);
              } else if (leState == 2) {
                    cout << "Checkmate! Black wins!" << endl;
                    thePlayers[1]->addScore(1);
              } else if (leState == 3) {
                    cout << "Stalemate! " << endl;
                    thePlayers[0]->addScore(0.5);
                    thePlayers[1]->addScore(0.5);
              } else if (leState == 4) {
                    thePlayers[1]->addScore(1);
              } else if (leState == -4) {
                    thePlayers[0]->addScore(1);
              }
              vector<vector<shared_ptr<Piece>>> startBoard(ROW, vector<shared_ptr<Piece>>(COL));
              defaultBoard(startBoard);
              Board newBoard(BLACK_KING_POS, WHITE_KING_POS, std::move(startBoard), allowAtomic);
              board = move(newBoard);
              whiteMove = true;
              std::cout << endl <<  "------Main Menu------" << endl;
              if (cin.eof()) goto End;            
          } else if (input == "setup") {
               setup(board, observers, whiteMove);
               cout << "Exited setup sucessfully" << endl << endl;
               std::cout <<  "------Main Menu------" << endl;
          }
    }
    End:
     if (thePlayers.size() < 2)  cout << "Final Score" << endl << "White: N/A" << endl << "Black: N/A" << endl;
     else cout << "Final Score" << endl << "White: " <<  dec
          << thePlayers[0]->getScore() << endl << "Black: " << dec << thePlayers[1]->getScore() << endl;
}   



// Helper Methods ----------------------------------------------------
string getInputs () {
     string input;
     cin >> input;
     while (input != "human" && input != "computer1" && input != "computer2" && input != "computer3" && input != "computer4" ){
          cout << "Please enter 'human' or 'computer[1-4]'" << endl;
          cin >> input;
          if (cin.eof()) break;
     }
     return input;
}

void defaultBoard(vector<vector<shared_ptr<Piece>>> &board) {
     board[0][0] = make_shared<Rook>(Position{0,0}, WHITE_TEAM, false);
     board[0][1] = make_shared<Knight>(Position{0,1}, WHITE_TEAM, false);
     board[0][2] = make_shared<Bishop>(Position{0,2}, WHITE_TEAM, false);
     board[0][3] = make_shared<Queen>(Position{0,3}, WHITE_TEAM, false);
     board[0][4] = make_shared<King>(Position{0,4}, WHITE_TEAM, false);
     board[0][5] = make_shared<Bishop>(Position{0,5}, WHITE_TEAM, false);
     board[0][6] = make_shared<Knight>(Position{0,6}, WHITE_TEAM, false);
     board[0][7] = make_shared<Rook>(Position{0,7}, WHITE_TEAM, false);
     for (int i = 0; i < COL; i++) {
          board[1][i] = make_shared<Pawn>(Position{1,i}, WHITE_TEAM, false);
     }

     for(int i = 2; i < 6; i++) {
        for (int j = 0; j < COL ; j++) {
            board[i][j] = make_shared<Blank>(Position{i,j}, NEUTRAL, false);
        }
     }

     board[7][0] = make_shared<Rook>(Position{7,0}, BLACK_TEAM, false);
     board[7][1] = make_shared<Knight>(Position{7,1}, BLACK_TEAM, false);
     board[7][2] = make_shared<Bishop>(Position{7,2}, BLACK_TEAM, false);
     board[7][3] = make_shared<Queen>(Position{7,3}, BLACK_TEAM, false);
     board[7][4] = make_shared<King>(Position{7,4}, BLACK_TEAM, false);
     board[7][5] = make_shared<Bishop>(Position{7,5}, BLACK_TEAM, false);
     board[7][6] = make_shared<Knight>(Position{7,6}, BLACK_TEAM, false);
     board[7][7] = make_shared<Rook>(Position{7,7}, BLACK_TEAM, false);
     for (int i = 0; i < COL; i++) {
          board[6][i] = make_shared<Pawn>(Position{6,i}, BLACK_TEAM, false);
     }
}
bool checkIfValidSquare (char col, int row) {
     if (col < LOWEST_COL || col > HIGHEST_COL || row > HIGHEST_ROW || row < LOWSET_ROW) {
          if (col < 'a' || col > 'h') cout << "Please make sure 'a' <= column <= 'h'" << endl;
          else if (row > 8 || row < 1) cout << "Please make sure 1 <= row <= 8" << endl;
          cin.ignore();
          cin.clear();
          return false;
     }

     return true;
}

bool checkIfValidPiece(string type) {
     if (type[0] != 'K' && type[0] != 'Q' && type[0] != 'N' && type[0] != 'R' && type[0] != 'B' && type[0] != 'P' && type[0] != 'k' 
          && type[0] != 'q' && type[0] != 'n' && type[0] != 'r' && type[0] != 'b' && type[0] != 'p') {
          cout << "Please make sure you only enter a character that is one of {'K', 'Q', 'R', 'N', 'B', 'P', 'k', 'q', 'r', 'n', 'b', 'p'}" << endl;
          cin.ignore();
          cin.clear();
          return false;
     } else return true;
}

void setup(Board &board, vector<unique_ptr<Observer>> &observers, bool &turn) {
    bool done = false;
    cout << "--In Setup Mode--" << endl;

    while(!done) {
        string command;
        cin >> command;
        if (std::cin.eof()) break;

        if (command == "done") {
            int numWhiteKings = 0, numBlackKings = 0;
            for (int i = 0; i < ROW; i++) {
               for (int j = 0; j < COL; j++) {
                    shared_ptr<Piece> lePiece = board.getPieces()[i][j];
                    if(lePiece->getType() == KING ) {
                         if (lePiece->getTeam() == BLACK_TEAM) {
                              board.setBlackKing(board.getPieces()[i][j]->posn);
                              numBlackKings++;
                         }
                         else if (lePiece->getTeam() == WHITE_TEAM) {
                              board.setWhiteKing(board.getPieces()[i][j]->posn);
                              numWhiteKings++;
                         }    
                    }
                    if (lePiece->getType() == PAWN && (i == 7 || i == 0)) {
                         goto BadPawn;
                    }
               }
            }
            
            if (numWhiteKings !=1 || numBlackKings != 1) {
               cout << "There must be exactly 1 white king and 1 black king" << endl;
            }
            else if (board.isCheck(WHITE_TEAM) || board.isCheck(BLACK_TEAM)) {
                    cout << "Black King or White King is at check" << endl;
            }
            else done = true;
            continue;
            BadPawn:
               cout << "No pawns should be on the first and last row" << endl;
        }
        else if (command == "+") {
           string square;
           string type;
           char col;
           int row;
           
           cin >> type;
           if (checkIfValidPiece(type)) {
               cin >> square;
               istringstream iss{square};
               iss >> col >> row;
           } else continue;

           
           if (!checkIfValidSquare(col,row)) continue;

            // Adjusting to fit in margins
            col -= LOWEST_COL;
            row--;

            // white team
            if (type == "Q") board.addPiece(Position{row,col}, QUEEN, WHITE_TEAM, false);
            else if (type == "K") board.addPiece(Position{row,col}, KING, WHITE_TEAM, false);
            else if (type == "N") board.addPiece(Position{row,col}, KNIGHT, WHITE_TEAM, false);
            else if (type == "R") board.addPiece(Position{row,col}, ROOK, WHITE_TEAM, false);
            else if (type == "P") board.addPiece(Position{row,col}, PAWN, WHITE_TEAM, false);
            else if (type == "B") board.addPiece(Position{row,col}, BISHOP, WHITE_TEAM, false);
            // black team
            else if (type == "q") board.addPiece(Position{row,col}, QUEEN, BLACK_TEAM, false);
            else if (type == "k") board.addPiece(Position{row,col}, KING, BLACK_TEAM, false);
            else if (type == "n") board.addPiece(Position{row,col}, KNIGHT, BLACK_TEAM, false);
            else if (type == "r") board.addPiece(Position{row,col}, ROOK, BLACK_TEAM, false);
            else if (type == "p") board.addPiece(Position{row,col}, PAWN, BLACK_TEAM, false);
            else if (type == "b") board.addPiece(Position{row,col}, BISHOP, BLACK_TEAM, false);
            
            observers[0]->notify(board);
            if (turnOnGraphics) observers[1]->notify(board);

            cout << endl;
        } else if (command == "-") {
           string square;
           char col;
           int row;

           cin >> square;
           istringstream iss{square};
           iss >> col >> row;

           if (!checkIfValidSquare(col, row)) continue;
          
           // Adjusting for index
           col -= LOWEST_COL;
           row--;
 
           board.killPiece(Position{row,col});
           observers[0]->notify(board);
           if (turnOnGraphics) observers[1]->notify(board);

           cout << endl;
        } else if (command == "=") {
             string color;
             cin >> color;

             if (color != "white" && color != "black") {
               cout << "Bruh theres only black and whites allowed" << endl;
               cin.ignore();
               cin.clear();
               continue;
             }

             if (color == "white") {
               turn = true;
               cout << "white's turn!" << endl;
             } else {
               turn = false;
               cout << "ITS BLACK TIME" << endl;
             }

             cout << endl;
        } else {
             cout << "Please enter a valid command: {+, -, color}" << endl;
        }
        
    }
}

