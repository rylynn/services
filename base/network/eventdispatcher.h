#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>
#include "common.h"
namespace service {
using std::map;
using std::vector;
enum EVENTTYPE {
  CONNECTEVENT = 1,
  READEVENT = 2,
  WRITEEVENT = 4,
  ERROREVENT = 8
};
class Buffer;
class Handler;
class EventLoop;
class EventDispatcher {
public:
  EventDispatcher(EventLoop* eventloop):eventloop_(eventloop){;}
  int RegisterHandler(uint32_t event_mask, Handler* handler);
  int RemoveHandler(int handler_key);
  int DispatcherEvent(EVENTTYPE event, int channel_id, Buffer& buffer);
private:
  void AddEvent(EVENTTYPE event, Handler* handler);
private:
  map<EVENTTYPE, map<int, boost::shared_ptr<Handler> > > handler_maps_; 
  EventLoop* eventloop_;
};

}
#endif
