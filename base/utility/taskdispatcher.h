#ifndef UTILITY_TASKDISPATCHER_H
#define UTILITY_TASKDISPATCHER_H
#include "task.h"
#include <map>
#include <atomic>
#include "ringbuffer.h"
#include "threadpool.h"
#include "sequence.h"
#include <boost/shared_ptr.hpp>
#include <mutex>
namespace utility {

struct ThreadPool_Task_Pair {
  ThreadPool thread_pool_;
  RingBuffer<Task*, 1024> task_ring_buffer_;
  Sequence cur_pub_seq_;
  Sequence cur_worker_seq_;
};

class TaskDispatcher : public boost::noncopyable{
public:
  void AddThreadTaskMap(int hash_key, ThreadPool_Task_Pair* pair);
  int Dispatch(int hash_key, Task* task);
private:
  std::map<int, boost::shared_ptr<ThreadPool_Task_Pair> > thread_task_map_;
  std::mutex lock_;
};
}
#endif
