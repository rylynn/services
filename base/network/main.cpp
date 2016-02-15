#include "eventloop.h"
#include "kqueuepoller.h"
#include <shared_ptr.hpp>
using namespace service;
int main() {
  boost::shared_ptr<KqueuPoller> poller_ptr(new KqueuPoller());
  poller_ptr->Init(1024);
  boost::shared_ptr<EventLoop> loop_ptr(new EventLoop());
  if (loop_ptr->Init(poller_ptr.get()))
    return loop_ptr->Run();
  else 
    return -1;
}
