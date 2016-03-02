#ifndef DEMON_HANDLER_H
#define DEMON_HANDLER_H
#include "handler.h"
namespace service {
  class EventDispatcher;
}
using namespace service;
class DemonHandler : public Handler {
public:
  DemonHandler(EventDispatcher* eventdispatcher):eventdispatcher_(eventdispatcher){;}
  void onReceiveMessage(int channel_id, string message);
  void onHandleError(int channel_id, string message){;}
  void onConnect(int channel_id);
  ~DemonHandler(){;}
private:
  EventDispatcher* eventdispatcher_;
};
#endif
