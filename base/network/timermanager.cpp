#include "timermanager.h" 
namespace service {

void TimerManager::AddEvent(TimerEvent& event) {
  timerevent_list_.push_back(event); 
}

void TimerManager::ClearEvent() {
  timerevent_list_.clear();
}

void  TimerManager::NotifyTimeOutEvent(uint32_t timeout) {
  for(list<TimerEvent>::iterator iter = timerevent_list_.begin(); iter != timerevent_list_.end();) {
    if (iter->timeout() <= timeout) {
      iter->OnTime();
      timerevent_list_.erase(iter);
    } else {
      iter++;
    }
  }
}

}
