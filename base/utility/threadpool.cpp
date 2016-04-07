#include "threadpool.h"
#include "thread.h"
namespace utility {
void ThreadPool::StartAll(bool detach) {
  for(auto iter = pool_.begin(); iter != pool_.end(); ++iter) {
    (*iter)->Start();
    if (detach)
      (*iter)->Detach();
  }
}

void ThreadPool::StopAll() {
  while(!pool_.empty()) {
    pool_[pool_.size()]->Stop();
    pool_.pop_back();
  }
}

void ThreadPool::AddThread(Thread* thread) {
  boost::shared_ptr<Thread> thread_ptr(thread);
  pool_.push_back(thread_ptr);
}

}
