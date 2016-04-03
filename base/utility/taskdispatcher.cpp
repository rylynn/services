#include "taskdispatcher.h"
namespace utility {
int Dispatch(int hash_key) {
  std::lock_guard<std::mutex> lck(lock_, std::adopt_lock);
}

}
