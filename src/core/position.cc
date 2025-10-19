#include "position.h"

bool Position::operator==(const Position &other) const {
     return (row == other.row && col == other.col);
}

bool Position::operator!=(const Position &other) const {
     return !(*this == other);
}

std::ostream& operator<<(std::ostream& out, const Position &pos) {
     out << std::string(1,'a'+ pos.col) << pos.row +1;
     return out;
}
