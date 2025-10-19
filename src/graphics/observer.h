#ifndef OBSERVER_H_
#define OBSERVER_H_

#include "board.h"

// abstract Observer
class Observer {
 public:
  virtual void notify(Board &board) = 0;
  virtual ~Observer() = default;
};

#endif

