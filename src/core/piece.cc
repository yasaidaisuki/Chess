#include "piece.h"
#include <memory>

Piece::Piece(const Piece &&other): 
    team{std::move(other.team)}, type{std::move(other.type)}, posn{std::move(other.posn)} {}

Piece &Piece::operator=(Piece &&other) {
    std::swap(posn, other.posn);
    std::swap(team, (other.team));
    std::swap(type, other.type);
    return *this;
}

char Piece::getState() const {
    if (type == BLANK) return BLANK;
    return team == 1 ? 
        type : type + 32; 
}

