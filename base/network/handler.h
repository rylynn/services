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
<<<<<<< HEAD
  virtual void onConnect(int channel_id) = 0;
private:
=======
  virtual void onConnect(int channel_id);
protected:
>>>>>>> 262e1516f4ee4af5242c7a3f1a8b2ec513a5a151
  boost::shared_ptr<Codec> codec_;
};
}
#endif
