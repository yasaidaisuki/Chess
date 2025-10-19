#ifndef TEXTOBSERVER_H_
#define TEXTOBSERVER_H_

#include <iostream>
#include "board.h"
#include "observer.h"

const int ROW = 8, COL = 8;

class TextObserver : public Observer {    
    std::ostream& out;
    public:
        TextObserver();
        void notify(Board &board) override;
        ~TextObserver() = default;
};

#endif
