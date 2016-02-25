#include "demonhandler.h"
#include <iostream>
#include "buffer.h"
#include "eventdispatcher.h"
using std::cout;
void DemonHandler::onReceiveMessage(int channel_id, string message) {
  cout << "DemonHandler::onReceiveMessage() channel_id" << channel_id << " message: " << message << std::endl; 
  Buffer buffer;
  buffer.Append(message.data(), message.length());
  eventdispatcher_->DispatcherEvent(WRITEEVENT, channel_id, buffer);
}

void DemonHandler::onConnect(int channel_id) {
  cout << "DemonHandler::onConnect() channel_id" << channel_id << std::endl; 
}
