#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <sstream>

#include <vector>
#include "board.h"
#include "observer.h"
#include "window.h"

const int WIDTH = 750, HEIGTH = 750;
const double PIXEL = 600/8;
const int SPACE = 75;

class Graphics : public Observer {    
    Xwindow window;
    std::vector<std::vector<char>> prevBoard;
    public:
        Graphics(Board &board);
        void notify(Board &board) override;
        ~Graphics() = default;
};

#endif

