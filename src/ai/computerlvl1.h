#ifndef COMPUTERLVL1_H
#define COMPUTERLVL1_H

#include "computer.h"

class Computerlvl1 : public Computer {
   public:
      Computerlvl1(int team, Board &board): Computer{team, board} {}
      bool playerMove(bool undo) override;
};

#endif
