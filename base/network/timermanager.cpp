#include "timermanager.h" 
#include <iostream>
namespace service {

void TimerManager::AddEvent(TimerEvent& event) {
  timerevent_list_.push_back(event); 
}

void TimerManager::ClearEvent() {
  timerevent_list_.clear();
}

void  TimerManager::NotifyTimeOutEvent(uint32_t timeout) {
  if (timerevent_list_.empty()) {
    return;
  }
  for(list<TimerEvent>::iterator iter = timerevent_list_.begin(); iter != timerevent_list_.end();iter++) {
    if (iter->timeout() <= timeout) {
      iter->OnTime();
      if (!iter->is_cycle()) {
        timerevent_list_.erase(iter);
        std::cout<<"erase timer event"<<std::endl;
      } else {
        iter->reset();
      }
    } 
  }
}

}
