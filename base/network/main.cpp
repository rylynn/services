#include "eventloop.h"
#include "kqueuepoller.h"
#include <shared_ptr.hpp>
#include "eventdispatcher.h"
#include "demonhandler.h"
#include "timermanager.h"
using namespace service;
int main() {
  boost::shared_ptr<KqueuPoller> poller_ptr(new KqueuPoller());
  poller_ptr->Init(1024);
  boost::shared_ptr<EventLoop> loop_ptr(new EventLoop());
  boost::shared_ptr<TimerManager> timer_manager_ptr(new TimerManager());
  boost::shared_ptr<EventDispatcher> eventdispatcher_ptr(new EventDispatcher(loop_ptr.get()));
  DemonHandler* handler = new DemonHandler(eventdispatcher_ptr.get());
  eventdispatcher_ptr->RegisterHandler(READEVENT, handler);
  eventdispatcher_ptr->RegisterHandler(WRITEEVENT, handler);
  eventdispatcher_ptr->RegisterHandler(CONNECTEVENT, handler);
  
  if (loop_ptr->Init(poller_ptr.get(), eventdispatcher_ptr.get(), timer_manager_ptr.get()))
    return loop_ptr->Run();
  else 
    return -1;
}
