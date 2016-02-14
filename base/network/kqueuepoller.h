#ifndef KQUEUEPOLLER_H
#define KQUEUEPOLLER_H
#include "poller.h"
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
namespace service {
static const int kEventSize = 1024;
class EventLoop;
struct ActiveEvent;
class KqueuPoller : public Poller {
public:
  KqueuPoller(){;}
  ~KqueuPoller(){if(kevent_ptr_) delete kevent_ptr_;}
  int Init(uint32_t size);
  int Poll(EventLoop& loop , struct timespec& tvp);
  int AddEvent(ActiveEvent& ev); 
  void RemoveEvent(ActiveEvent& ev);
  void EnableWrite(int fd);
private:
  int kqueufd_;
  struct kevent* kevent_ptr_;
};
}
#endif
