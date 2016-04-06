#include "taskqueue.h"
#include "task.h"
namespace utility {

void TaskEntity::Release() {
  delete this;
}

int TaskQueue::PushBack(Task* task) {
  int ret = kReturnok;
  do {
    if (task == nullptr) {
      ret = kReturnArgumentErr;
      break;
    }
    uint32_t length = length_.fetch_add(1) + 1; // atomic add length
    if (length == 1) {
      Lock();
      begin_node_ = new TaskEntity(task); // change begin_node_ should lock
      end_node_ = begin_node_;
      end_node_->SetNext(nullptr);
      UnLock();
    } else if (length > 1){
      TaskEntity* entity = new TaskEntity(task);
      Lock();
      end_node_->SetNext(entity);
      end_node_ = entity;
      //end_node_->SetNext(nullptr);
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
      begin_node_ = end_node_;
      break;
    } 
    length_.fetch_sub(1);
    TaskEntity* entity = begin_node_;
    ret = entity->Node();
    Lock();
    begin_node_ = begin_node_->Next();
    entity->Release();
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
      begin_node_->Release();
      end_node_ = begin_node_ = nullptr;
      end_node_->SetNext(nullptr);
      UnLock();
    } else {
      ret = end_node_->Node();
      Lock();
      end_node_->Release();
      TaskEntity* entity  = begin_node_;
      for (;entity->Next() != end_node_; entity = entity->Next())  {/*get second last node*/}
      end_node_ = entity;
      end_node_->SetNext(nullptr);
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
