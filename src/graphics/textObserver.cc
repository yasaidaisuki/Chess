#include "textObserver.h"

TextObserver::TextObserver() : out(std::cout) {}

void TextObserver::notify(Board &board) {
    out << "-- The Board --" << std::endl;
    for (int i = ROW - 1; i >= 0 ; i--) {
        out << i + 1 << " ";
        for (int j = 0 ; j < COL; j++) {
            // print blank
            if (board.getPieces()[i][j]->getType() == BLANK) {
                if (j%2 == 0 ) out << "_";
                else out << " ";
            }
            // print everything
            else out << board.getPieces()[i][j]->getState();
        }
        out << std::endl;
    }
    out << "-- The Board --" << std::endl;
}
