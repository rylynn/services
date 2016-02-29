#ifndef EVENT_H
#define EVENT_H
#include <boost/function.hpp>
namespace service {
struct ActiveEvent {
  int fd;
  int mask;
};

class TimerEvent {
public:
  TimerEvent(uint32_t time_out,   boost::function<void(void)> call_back, bool is_cycle = false): time_out_(time_out), is_cycle_(is_cycle), call_back_(call_back){;}
  virtual void OnTime(){
    call_back_();
  }
  uint32_t timeout() {return time_out_;}
  bool is_cycle_;
private:
  uint32_t time_out_;
  boost::function<void(void)> call_back_;
};

}
#endif
