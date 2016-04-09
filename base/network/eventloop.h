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
class TimerManager;
class EventLoop
{
public:
  EventLoop():poll_(NULL),event_size_(1024), server_fd_(0) ,addr_(const_cast<char*>(ip.c_str()), port){;}
  bool Init(Poller* poll, EventDispatcher* dispatcher, TimerManager* timer_manager);
  int Run();
  ::std::vector<ActiveEvent>& PollData(){return active_events_;}
  void SetEventSize(uint32_t size) {event_size_ = size;}
  int AddEventChannel(Channel* channel, bool is_server = false);
public:
  int SendMessage(int channel_id, const char* data, uint32_t size);
  uint32_t GetNow();
private:
  void RemoveChannel(boost::shared_ptr<Channel>& channel);
  void AddChannel(int fd);
private:
  std::map<int , boost::shared_ptr<Channel> > event_map_;
  ::std::vector<ActiveEvent> active_events_;
  Poller* poll_;
  EventDispatcher* dispatcher_;
  TimerManager* timer_manager_;
  uint32_t event_size_;
  int server_fd_;
  //Address addr_;
};
}
#endif
