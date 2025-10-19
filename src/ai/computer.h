#ifndef COMPUTER_H
#define COMPUTER_H

#include "move.h"
#include "player.h"

class Computer : public Player {
   public:
      Computer(int team, Board &board): Player{0, team, board} {}
};

#endif 
