#ifndef MOVE_H
#define MOVE_H

#include "position.h"

class Move {
   Position currPosn, nextPosn;
   bool promoteQueen;
   public: 
      Move(Position currPosn, Position nextPosn) : currPosn{currPosn}, nextPosn{nextPosn}, promoteQueen{false} {}
      Move(Position currPosn, Position nextPosn, bool promoteQueen) : currPosn{currPosn}, nextPosn{nextPosn}, promoteQueen{promoteQueen} {}
      Position getCurrPosn() {return currPosn;}
      Position getNextPosn() {return nextPosn;}
      bool getPromoteQueen() {return promoteQueen;}
};

#endif

