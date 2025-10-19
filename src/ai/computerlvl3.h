#ifndef COMPUTERLVL3_H
#define COMPUTERLVL3_H

#include "computer.h"

class Computerlvl3 : public Computer {
   public:
      Computerlvl3(int team, Board &board): Computer{ team, board} {}
      bool playerMove(bool allowUndo) override;
      void makeMove(std::vector<Move> &moveSet, bool promotion);
};

#endif 
