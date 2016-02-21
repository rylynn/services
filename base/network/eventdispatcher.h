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
  CONNECTEVENT = 0,
  READEVENT = 1,
  WRITEEVENT = 2,
  ERROREVENT = 3
};
class Buffer;
class Handler;
class EventLoop;
class EventDispatcher {
public:
  int RegisterHandler(EVENTTYPE event, boost::shared_ptr<Handler>& handler);
  int RemoveHandler(int handler_key);

  int DispatcherEvent(EVENTTYPE event, int channel_id, Buffer& buffer);
private:
  map<EVENTTYPE, map<int, boost::shared_ptr<Handler> > > handler_maps_; 
};

}
#endif
