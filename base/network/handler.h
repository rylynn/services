#ifndef HANDLER_H
#define HANDLER_H
#include <string>
#include <boost/shared_ptr.hpp>
namespace service {
using std::string;
class Codec;
class Handler {
public:
  virtual void onReceiveMessage(int channel_id, string message) = 0;
  virtual void onHandleError(int channel_id, string message) = 0;
  virtual void onConnect(int channel_id);
protected:
  boost::shared_ptr<Codec> codec_;
};
}
#endif
