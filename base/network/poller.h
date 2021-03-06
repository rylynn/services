#ifndef POLLER_H
#define POLLER_H
#include "common.h"
#include "event.h"
#include <inttypes.h>
#include <time.h>
namespace service {
class EventLoop;
class Poller {
public:
  virtual ~Poller(){};
  virtual int Init(uint32_t size) = 0; 
  virtual int Poll(EventLoop& loop , struct timespec& tvp)=0;
  virtual int AddEvent(ActiveEvent& ev) = 0;
  virtual void RemoveEvent(ActiveEvent& ev) = 0;
};
}
#endif
