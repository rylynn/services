#ifndef AUTOGUARD_H
#define AUTOGUARD_H
namespace service {
template <class T>
class AutoGuard {
  AutoGuard(T) {T.Lock();}
  ~AutoGuard() {T.UnLock();}
};
}
#endif
