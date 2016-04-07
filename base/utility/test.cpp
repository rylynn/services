#include "taskqueue.h"
#include <iostream>
#include "demontask.h"
#include <chrono>
#include "tick.h"
#include "taskdispatcher.h"
#include "threadpool.h"
using namespace utility;
using namespace std;
void fun1(void* data) {
  TaskQueue* queue = static_cast<TaskQueue*>(data);
  cout<< " thread "<< std::this_thread::get_id()<<" run" << endl;
  std::chrono::milliseconds dura( 5);
  cout.flush();
  for (int i = 0; i < 10; i++) {
    Task* task = new DemonTask();
    queue->PushBack(task);
    //std::this_thread::sleep_for(dura);
  }
  cout<<"f1:queue size" << queue->size()<<endl;;
  cout.flush();
  Task* task = queue->PopFront();
  task->Action();
  //task->Release();
  cout<<"f1:queue size" << queue->size()<<endl;;
  cout.flush();
}

void fun2(void* data) {
  TaskQueue* queue = static_cast<TaskQueue*>(data);
  cout<< " thread "<< std::this_thread::get_id()<<" run" << endl;
  std::chrono::milliseconds dura( 50);
  cout.flush();
  for (int i = 0; i < 5; i++) {
    Task* task = new DemonTask();
    queue->PushBack(task);
    //std::this_thread::sleep_for(dura);
  }
  cout<<"f2:queue size" << queue->size()<<endl;;
  cout.flush();
  Task* task = queue->PopFront();
  task->Action();
  task->Release();
  
  cout<<"f2:queue size" << queue->size()<<endl;;
  cout.flush();
  (*queue)[10].Action();
}

void TaskCallBack(void* param) {
  ThreadPool_Task_Pair* pair = static_cast<ThreadPool_Task_Pair*>(param);
  while (1) {
    while(pair->cur_pub_seq_.GetSequence() == 0 || pair->cur_worker_seq_.GetSequence() >= pair->cur_pub_seq_.GetSequence()) {
      std::this_thread::yield();
    }
    uint32_t cur_seq = pair->cur_worker_seq_.IncreaseAndGet(1);
    cout << "worker seq" << cur_seq <<endl;
    cout.flush();
    pair->task_ring_buffer_[cur_seq]->Action();
    pair->task_ring_buffer_[cur_seq]->Release();
    pair->task_ring_buffer_[cur_seq] = nullptr;
    break;
  }
}

int main() {
  LOG_FUNCTION_DURATION
  TaskDispatcher dispatcher;
  ThreadPool_Task_Pair* thread_task_pair = new ThreadPool_Task_Pair();
  dispatcher.AddThreadTaskMap(1, thread_task_pair);
  std::function<void(void*)> callback_func = TaskCallBack;
  Thread* t1 = new Thread(callback_func, (void*)thread_task_pair);
  Thread* t2 = new Thread(callback_func , (void*)thread_task_pair);
  thread_task_pair->thread_pool_.AddThread(t1);
  thread_task_pair->thread_pool_.AddThread(t2);
  thread_task_pair->thread_pool_.StartAll();
  DemonTask* task = new DemonTask();
  DemonTask* task1 = new DemonTask();
  DemonTask* task2 = new DemonTask();
  dispatcher.Dispatch(1,task);
  dispatcher.Dispatch(1,task1);
  dispatcher.Dispatch(1,task2);
  t1->Join();
  t2->Join();
  return 0;
}
