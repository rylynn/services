#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <shared_ptr.hpp>
#include <map>
#include <vector>
#include "socket.h"
#include "event.h"
namespace service {
static const int READABLE = 1;
static const int WRITABLE = 2;
class Poller;
class EventLoop
{
public:
  EventLoop():poll_(NULL),event_size_(1024){;}
  bool Init(Poller* poll);
  int Run();
  ::std::vector<ActiveEvent>& PollData(){return active_events_;}
  void SetEventSize(uint32_t size) {event_size_ = size;}
private:
  std::map<int , boost::shared_ptr<Socket> > event_map_;
  ::std::vector<ActiveEvent> active_events_;
  Poller* poll_;
  uint32_t event_size_;
  int server_fd_;
};
}
#endif