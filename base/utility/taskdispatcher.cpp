#include "taskdispatcher.h"
#include "common.h"
#include "sequence.h"
namespace utility {
void TaskDispatcher::AddThreadTaskMap(int hash_key, ThreadPool_Task_Pair* pair) {
  std::lock_guard<std::mutex> auto_guard(lock_); 
  boost::shared_ptr<ThreadPool_Task_Pair> pair_ptr(pair);
  thread_task_map_.insert(std::make_pair(hash_key, pair_ptr));
}

int TaskDispatcher::Dispatch(int hash_key, Task* task) {
  int ret = kReturnok;
  do {
    auto iter = thread_task_map_.find(hash_key);
    if (iter != thread_task_map_.end()) {
      iter->second->task_ring_buffer_[iter->second->cur_pub_seq_.IncreaseAndGet(1)] = task;
      break;
    } else {
      ret = kReturnArgumentErr;
      break;
    }
  } while(0);
  return ret;;
}

}
