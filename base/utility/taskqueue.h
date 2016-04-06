#ifndef UTILITY_TASKQUEUE_H
#define UTILITY_TASKQUEUE_H
/*
 * A Lock-Free TaskQueue
 * */
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
}; //task list

class TaskQueue {
public:
  TaskQueue():begin_node_(nullptr), end_node_(nullptr), length_(0), lock_(ATOMIC_FLAG_INIT){;}
  int PushBack(Task* task);
  Task* PopBack();
  Task* PopFront();
  void Clear();
  uint32_t size() { return length_.load(std::memory_order_acquire);}
  Task& operator [](uint32_t index) {
    TaskEntity* entity = begin_node_;
    for (int i = 0;entity->Next() != end_node_ && i < index; entity = entity->Next(), ++i)  {}
   return *entity->Node();
  }
  const Task& operator [](uint32_t index) const {
    TaskEntity* entity = begin_node_;
    for (int i = 0;entity->Next() != end_node_ && i < index; entity = entity->Next(), ++i)  {}
   return *entity->Node();  
  }
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
  TaskEntity* end_node_;
  std::atomic<uint32_t> length_;
  std::atomic_flag lock_;//use atomic flag like a simple lock 
};
}
#endif
