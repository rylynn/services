#ifndef DEMON_TASK_H
#define DEMON_TASK_H
#include "task.h"
#include <iostream>
using namespace utility;
class DemonTask : public Task {
  virtual void Action() {
    std::cout<< "task:"<< this << " acion" <<std::endl;
    std::cout.flush();
  }
  virtual void Release(){delete this;}
  virtual ~DemonTask(){Release();}
};
#endif
