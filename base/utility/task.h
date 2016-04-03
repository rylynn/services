#ifndef SERVICE_TASK_H
#define SERVICE_TASK_H
namespace utility {
class Task {
public:
  virtual void Action() = 0;
  virtual void Release() = 0;
  virtual ~Task(){;}
};
}
#endif
