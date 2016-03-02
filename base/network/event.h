#ifndef EVENT_H
#define EVENT_H
#include <sys/time.h>
#include <boost/function.hpp>
namespace service {
struct ActiveEvent {
  int fd;
  int mask;
};

class TimerEvent {
public:
  TimerEvent(uint32_t interval,   boost::function<void(void)>& call_back, bool is_cycle = false): interval_(interval), is_cycle_(is_cycle), call_back_(call_back){
    struct timeval tvp;
    gettimeofday(&tvp, NULL);
    time_out_ = tvp.tv_sec + interval_;
  }
  virtual void OnTime(){
    call_back_();
  }
  void reset() {time_out_ += interval_;}
  uint32_t timeout() {return time_out_;}
  bool is_cycle() {return is_cycle_;}
  
private:
  uint32_t time_out_;
  uint32_t interval_;
  bool is_cycle_;
  boost::function<void(void)> call_back_;
};

}
#endif
