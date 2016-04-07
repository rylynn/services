#ifndef UTILITY_THREADPOOL_H
#define UTILITY_THREADPOOL_H
#include "common.h"
#include <vector>
#include <string>
#include "thread.h"
#include <boost/shared_ptr.hpp>
namespace utility {
class ThreadPool {
public:
  int Init();
  void AddThread(Thread* thread);
  size_t Size() {return pool_.size();}
  void StopAll();
  void StartAll(bool detach = false);
private:
  std::vector<boost::shared_ptr<Thread> > pool_;
};
}
#endif
