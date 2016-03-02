#include "eventloop.h"
#include "kqueuepoller.h"
#include <shared_ptr.hpp>
#include "eventdispatcher.h"
#include "demonhandler.h"
#include "timermanager.h"
#include <iostream>
#include <boost/bind.hpp>
#include <utility/tick.h>
using namespace service;
using namespace utility;
using std::cout;
void test(int& time) {
  cout << "timerevent call_back "<< time <<std::endl;
  ++time;
  LOG_FUNCTION_DURATION
}
int main() {
  boost::shared_ptr<KqueuPoller> poller_ptr(new KqueuPoller());
  poller_ptr->Init(1024);
  boost::shared_ptr<EventLoop> loop_ptr(new EventLoop());
  boost::shared_ptr<TimerManager> timer_manager_ptr(new TimerManager());
  int cycle = 1;
  boost::function<void(void)> call_back_test = boost::bind(&test, cycle);
  TimerEvent timer_event(1, call_back_test, true);
  timer_manager_ptr->AddEvent(timer_event);
  boost::shared_ptr<EventDispatcher> eventdispatcher_ptr(new EventDispatcher(loop_ptr.get()));
  DemonHandler* handler = new DemonHandler(eventdispatcher_ptr.get());
  eventdispatcher_ptr->RegisterHandler(READEVENT|WRITEEVENT|CONNECTEVENT, handler);
  
  if (loop_ptr->Init(poller_ptr.get(), eventdispatcher_ptr.get(), timer_manager_ptr.get()))
    return loop_ptr->Run();
  else 
    return -1;
}
