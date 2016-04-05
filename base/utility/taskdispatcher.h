#ifndef UTILITY_TASKDISPATCHER_H
#define UTILITY_TASKDISPATCHER_H
#include "task.h"
#include "ringbuffer.h"
#include <map>
#include <mutex>
namespace utility {
class TaskDispatcher : public boost::noncopyable{
  void CreateTaskQueue(int hash_key);
  int Dispatch(int hash_key);
private:
  std::map<int, RingBuffer<Task, 2048> > taskqueue_map_;
  std::mutex lock_;
};
}
#endif
