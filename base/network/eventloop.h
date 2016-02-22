#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>
#include "socket.h"
#include "event.h"
#include <assert.h>
namespace service {
static const int READABLE = 1;
static const int WRITABLE = 2;
static const int ERROR = 4;
class Poller;
class Channel;
class EventDispatcher;
class EventLoop
{
public:
  EventLoop():poll_(NULL),event_size_(1024){;}
  bool Init(Poller* poll, EventDispatcher* dispatcher);
  int Run();
  ::std::vector<ActiveEvent>& PollData(){return active_events_;}
  void SetEventSize(uint32_t size) {event_size_ = size;}
private:
  void RemoveChannel(boost::shared_ptr<Channel>& channel);
private:
  std::map<int , boost::shared_ptr<Channel> > event_map_;
  ::std::vector<ActiveEvent> active_events_;
  Poller* poll_;
  EventDispatcher* dispatcher_;
  uint32_t event_size_;
  int server_fd_;
};
}
#endif
