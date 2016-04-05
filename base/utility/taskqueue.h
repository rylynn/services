#ifndef UTILITY_TASKQUEUE_H
#define UTILITY_TASKQUEUE_H
#include <atomic>
#include "common.h"
namespace utility {
class Task;

class TaskEntity {
public:
  TaskEntity(Task* task):next(nullptr),node(task){;}

  void SetNext(TaskEntity* entity) {
    next = entity;
  }

  TaskEntity* Next() {return next;}
  Task* Node() { return node;}
  void Release();

private:
  TaskEntity* next;
  Task* node;
};

class TaskQueue {
public:
  TaskQueue():begin_node_(nullptr), length_(0), lock_(ATOMIC_FLAG_INIT){;}
  int PushBack(Task* task);
  Task* PopBack();
  Task* PopFront();
  void Clear();
  uint32_t size() { return length_.load(std::memory_order_acquire);}
  void Lock() {
    while(lock_.test_and_set(std::memory_order_acquire)) {
      std::this_thread::yield();
    }
  }
  void UnLock() {
    lock_.clear(std::memory_order_release);
  }
private:
  TaskEntity* begin_node_;
  std::atomic<uint32_t> length_;
  std::atomic_flag lock_;
};
}
#endif
