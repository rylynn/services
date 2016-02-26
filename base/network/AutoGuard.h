#ifndef AUTOGUARD_H
#define AUTOGUARD_H
namespace service {
template <class T>
class AutoGuard {
public:
  AutoGuard(T* t, T::*lock(), T::*Unlock()) {
    t_ = t;
    if (lock != NULL && t_ != NULL) {
      lock(t_);
    }
    undo_ = Unlock;
  }
  ~AutoGuard() { 
    if (t_ != NULL && undo_ != NULL)
      undo_(t_);
  }
private:
  T* t_;
  void (T::*undo_)();
};
}
#endif
