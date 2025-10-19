#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"

class Board;

class Player {
    double score;
    int team;
    
    protected :
        Board &board;
    public :
        // Big 5
        Player(double score, int team, Board &board): score{0.0}, team{team}, board{board} {}
        //~Player();
        virtual bool playerMove(bool allowUndo) = 0;
        int getTeam() {return team;}
        double getScore() {return score;}
        void addScore(double add) {score += add;}
};  

#endif
