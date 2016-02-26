#ifndef AUTOGUARD_H
#define AUTOGUARD_H
namespace service {
template <class T>
class AutoGuard {
public:
  AutoGuard(T* t, T::*lock(), T::*Unlock()) {
    t_ = t;
    if (lock != NULL) {
      lock();
    }
    undo = Unlock;
  }
  ~AutoGuard() { undo();}
private:
  T* t_;
  void (T::*undo)();
};
}
#endif
