#include "eventdispatcher.h"
#include <string>
#include "handler.h"
#include "buffer.h"
#include "common.h"
#include "eventloop.h"
#include <iostream>
namespace service {
using std::string;
int EventDispatcher::RegisterHandler(EVENTTYPE event, Handler* handler) {
    if (handler == NULL) {
      return kReturnArgumentErr;
    }
    std::map<EVENTTYPE, map<int, boost::shared_ptr<Handler> > >::iterator handler_maps_iter = handler_maps_.find(event);
    if (handler_maps_iter == handler_maps_.end()) {
      map<int, boost::shared_ptr<Handler> > handler_map;
      boost::shared_ptr<Handler> handler_ptr(handler);
      handler_map.insert(std::make_pair(event, handler_ptr));
      handler_maps_.insert(std::make_pair(event, handler_map));
    } else {
      boost::shared_ptr<Handler> handler_ptr(handler);
      handler_maps_iter->second.insert(std::make_pair(event , handler_ptr));
    }
    return kReturnok;
}

int EventDispatcher::RemoveHandler(int handler_key) { 
  for (map<EVENTTYPE, map<int, boost::shared_ptr<Handler> > >::iterator handler_maps_iter = handler_maps_.begin(); handler_maps_iter != handler_maps_.end(); ++handler_maps_iter) {
    if (handler_maps_iter->second.find(handler_key) != handler_maps_iter->second.end()) {
      handler_maps_iter->second.erase(handler_key);
      break;
    }
  }
  return kReturnok;
}

int EventDispatcher::DispatcherEvent(EVENTTYPE event, int channel_id, Buffer& buffer) {
  assert(channel_id != kNullChannel);
  int ret =kReturnok;
  string message = buffer.ReadAll();
  do {
    std::map<EVENTTYPE, map<int, boost::shared_ptr<Handler> > >::iterator handler_map_iter = handler_maps_.find(event);
    if ( handler_map_iter == handler_maps_.end() ) {
      ret = kReturnSysErr;
      break;
    }
   for (map<int, boost::shared_ptr<Handler> >::iterator handler_iter = handler_map_iter->second.begin(); handler_iter != handler_map_iter->second.end(); ++handler_iter) {
     if (event == READEVENT) {
       handler_iter->second->onReceiveMessage(channel_id, message);
     } else if (event == CONNECTEVENT){
       handler_iter->second->onConnect(channel_id);
     } else if (event == ERROREVENT){
       handler_iter->second->onHandleError(channel_id, message);
     } else if (event == WRITEEVENT) {
       std::cout<<"disptach write event" << std::endl;
       eventloop_->SendMessage(channel_id, message.c_str(), message.length());
     }
   }
  } while(false); 
  return ret;
}

}
