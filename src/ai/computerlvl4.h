#include "computer.h"

// c random library
#include <random>
#include <iostream>

class Computerlvl4 : public Computer {
   public:
      Computerlvl4(int team, Board &board): Computer{team, board} {}
      bool playerMove(bool undo) override;
      void makeMove(std::vector<Move> &moveSet, bool promotion, int order);
};

