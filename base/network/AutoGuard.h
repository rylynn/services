#ifndef AUTOGUARD_H
#define AUTOGUARD_H
namespace service {
template <class T>
class AutoGuard {
public:
  AutoGuard(T* t) {
    t_ = t;
    t_->Lock();
  }
  ~AutoGuard() {t_->UnLock();}
private:
  T* t_;
};
}
#endif
