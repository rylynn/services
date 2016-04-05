#include "taskqueue.h"
#include <iostream>
#include "demontask.h"
#include <chrono>
#include "tick.h"
using namespace utility;
using namespace std;
void fun1(void* data) {
  TaskQueue* queue = static_cast<TaskQueue*>(data);
  cout<< " thread "<< std::this_thread::get_id()<<" run" << endl;
  std::chrono::milliseconds dura( 5);
  cout.flush();
  for (int i = 0; i < 10000; i++) {
    Task* task = new DemonTask();
    queue->PushBack(task);
    //std::this_thread::sleep_for(dura);
  }
  cout<<"f1:queue size" << queue->size()<<endl;;
  cout.flush();
  Task* task = queue->PopFront();
  task->Action();
  cout<<"f1:queue size" << queue->size()<<endl;;
  cout.flush();
}

void fun2(void* data) {
  TaskQueue* queue = static_cast<TaskQueue*>(data);
  cout<< " thread "<< std::this_thread::get_id()<<" run" << endl;
  std::chrono::milliseconds dura( 50);
  cout.flush();
  for (int i = 0; i < 5000; i++) {

    Task* task = new DemonTask();
    queue->PushBack(task);
    //std::this_thread::sleep_for(dura);
  }
  cout<<"f2:queue size" << queue->size()<<endl;;
  cout.flush();
  Task* task = queue->PopBack();
  task->Action();
  cout<<"f2:queue size" << queue->size()<<endl;;
  cout.flush();
}

int main() {
  LOG_FUNCTION_DURATION
  TaskQueue queue;
  std::thread t1(fun1,&queue);
  std::thread t2(fun2,&queue);
  t1.join();
  t2.join();
}