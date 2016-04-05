#include "taskqueue.h"
#include "task.h"
namespace utility {

void TaskEntity::Release() {
  next = nullptr;
  node = nullptr;
  delete this;
}

int TaskQueue::PushBack(Task* task) {
  int ret = kReturnok;
  do {
    if (task == nullptr) {
      ret = kReturnArgumentErr;
      break;
    }
    uint32_t length = length_.fetch_add(1) + 1;
    if (length == 1) {
      Lock();
      begin_node_ = new TaskEntity(task);
      UnLock();
    } else if (length > 1){
      TaskEntity* entity = new TaskEntity(task);
      TaskEntity* tail = (begin_node_ + length -2);
      Lock();
      tail->SetNext(entity);
      UnLock();
    } else {
      ret = kReturnSysErr;
      break;
    }
  } while(0);
  return ret;
}

Task* TaskQueue::PopFront() {
  Task* ret = nullptr;
  do {
    uint32_t length = length_.load(std::memory_order_acquire);
    if (length != 0 && begin_node_ == nullptr) {
      break;
    }
    if (length == 0) {
      break;
    } 
    length = length_.fetch_sub(1);
    TaskEntity* temp = begin_node_;
    ret = temp->Node();
    Lock();
    begin_node_ = begin_node_->Next();
    UnLock();
  } while(0);
  return ret;
}

Task* TaskQueue::PopBack() {
  Task* ret = nullptr;
  do {
    uint32_t length = length_.load(std::memory_order_acquire);
    if (length == 0) {
      break;
    }
    if (length != 0 && begin_node_ == nullptr) {
      length_.store(0,std::memory_order_release);
      break;
    }
    length = length_.fetch_sub(1) -1 ;
    if (length == 0) {
      ret = begin_node_->Node();
      Lock();
      begin_node_ = nullptr;
      UnLock();
    } else {
      TaskEntity* entity = begin_node_+ length - 1;
      ret = entity->Next()->Node();
      Lock();
      entity->SetNext(nullptr);
      UnLock();
    }
  } while(0);
  return ret;
}

void TaskQueue::Clear() {
  for (TaskEntity* entity = begin_node_; entity != nullptr; entity = entity->Next()) {
    Lock();
    PopFront();
    UnLock();
  }
}

}
