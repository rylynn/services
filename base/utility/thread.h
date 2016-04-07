#ifndef UTILITY_THREAD_H
#define UTILITY_THREAD_H
#include "common.h"
#include <functional>
#include <boost/shared_ptr.hpp>
namespace utility {
class Thread {
public:
  Thread(std::function<void(void*)>& func, void* param) : func_(func), param_(param), is_running_(false), is_detach_(false){;}
  virtual ~Thread(){;}
  virtual void Join() { 
    if (is_running_ && cur_thread_ptr->joinable())
      cur_thread_ptr->join();
  }
  virtual void Detach() {
    if(is_running_) {
      cur_thread_ptr->detach();
      is_detach_ = true;
    }
  }
  virtual void Start() {
    if (!is_running_) {
      cur_thread_ptr.reset(new std::thread(func_, param_));
      is_running_ = true;
    }
  }

  virtual void Release() {
    delete this;
  }

  virtual void Stop() {
    if(is_running_) {
      cur_thread_ptr.reset();
      is_running_ = false;
    }
  }
private:
  boost::shared_ptr<std::thread> cur_thread_ptr;
  std::function<void(void*)> func_;
  void* param_;
  bool is_running_;
  bool is_detach_;
};
}
#endif
