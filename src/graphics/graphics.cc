#include "graphics.h"

const int ROW = 8, COL = 8;

Graphics::Graphics(Board &board): prevBoard{ROW, std::vector<char>(COL)} {
    for (int i = ROW - 1; i >= 0; i--) {
        int posY = (((ROW-1)-i) * PIXEL) + SPACE;
        int charY = posY + PIXEL/2 - 1;
        // printing rows and cols label
        window.drawString(SPACE/2, charY, std::to_string(i+1) , Xwindow::Black);
        window.drawString(charY, 670 + SPACE/2, std::string(1, 'h' - i) , Xwindow::Black);
        for (int j = 0; j < COL; j++) {
            // position for rectangles
            int posX = (j * PIXEL) + SPACE;
            if (j%2 == 0 && i%2 == 1) 
                window.fillRectangle(posX, posY, PIXEL, PIXEL, Xwindow::Woof);
            else if (j%2 == 1 && i%2 == 0) 
                window.fillRectangle(posX, posY, PIXEL, PIXEL, Xwindow::Woof);
            else 
                window.fillRectangle(posX, posY, PIXEL, PIXEL, Xwindow::Mreow);
            // storing previous board 
            prevBoard[i][j] = BLANK;
        }
    }
}

void Graphics::notify(Board &board) {
    for (int i = ROW - 1; i >= 0 ; i--) {
        for (int j = 0 ; j < COL; j++) {
            // position for rectangles
            int posX = (j * PIXEL) + SPACE;
            int posY = (((ROW-1)-i) * PIXEL) + SPACE;
            // position for text
            int charX = posX + PIXEL/2 - 2;
            int charY = posY + PIXEL/2 + 2;

            // faster drawing
            if (prevBoard[i][j] != board.getPieces()[i][j]->getState()) {
                //print blank | in graphics it will overwrite the previous draw
                if (j%2 == 0 && i%2 == 1) 
                    window.fillRectangle(posX, posY, PIXEL, PIXEL, Xwindow::Woof);
                else if (j%2 == 1 && i%2 == 0) 
                    window.fillRectangle(posX, posY, PIXEL, PIXEL, Xwindow::Woof);
                else 
                    window.fillRectangle(posX, posY, PIXEL, PIXEL, Xwindow::Mreow);
                // print king
                if (board.getPieces()[i][j]->getType() == KING) {
                    if ((board.getPieces()[i][j]->getTeam() == 1)) 
                        window.drawString(charX, charY, std::string(1, KING), Xwindow::White);
                    else 
                        window.drawString(charX, charY, std::string(1, KING+32), Xwindow::Black);
                }
                // print rook
                else if (board.getPieces()[i][j]->getType() == ROOK) {
                    if ((board.getPieces()[i][j]->getTeam() == 1)) 
                        window.drawString(charX, charY, std::string(1, ROOK), Xwindow::White);
                    else 
                        window.drawString(charX, charY, std::string(1, ROOK+32), Xwindow::Black);
                }
                // print queen
                else if (board.getPieces()[i][j]->getType() == QUEEN) {
                    if (board.getPieces()[i][j]->getTeam() == 1) 
                        window.drawString(charX, charY, std::string(1, QUEEN), Xwindow::White);
                    else 
                        window.drawString(charX, charY, std::string(1, QUEEN+32), Xwindow::Black);
                }
                // print bishop
                else if (board.getPieces()[i][j]->getType() == BISHOP) {
                    if (board.getPieces()[i][j]->getTeam() == 1) 
                        window.drawString(charX, charY, std::string(1, BISHOP), Xwindow::White);
                    else 
                        window.drawString(charX, charY, std::string(1, BISHOP+32), Xwindow::Black);
                }
                // knight
                else if (board.getPieces()[i][j]->getType() == KNIGHT) {
                    if (board.getPieces()[i][j]->getTeam() == 1) 
                        window.drawString(charX, charY, std::string(1, KNIGHT), Xwindow::White);
                    else 
                        window.drawString(charX, charY, std::string(1, KNIGHT+32), Xwindow::Black);
                }
                // pawn
                else if (board.getPieces()[i][j]->getType() == PAWN) {
                    if (board.getPieces()[i][j]->getTeam() == 1) 
                        window.drawString(charX, charY, std::string(1, PAWN), Xwindow::White);
                    else 
                        window.drawString(charX, charY, std::string(1, PAWN+32), Xwindow::Black);
                }
            }
            // setting previous board to current board
            prevBoard[i][j] = board.getPieces()[i][j]->getState();
        }
    }
    //prevState = board;
}

