#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"

class Human : public Player {
    public:
        Human(int isWhite, Board &theBoard);
        bool playerMove(bool allowUndo) override;    
};

#endif

