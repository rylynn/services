#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H
#include <list>
#include "event.h"
using std::list;
namespace service {

class TimerManager {
public:
  void AddEvent(TimerEvent& event);
  void ClearEvent();
  void NotifyTimeOutEvent(uint32_t timeout);
private:
  list<TimerEvent> timerevent_list_;
};

}
#endif
