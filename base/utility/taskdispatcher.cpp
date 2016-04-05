#include "taskdispatcher.h"
namespace utility {
int TaskDispatcher::Dispatch(int hash_key) {
  std::lock_guard<std::mutex>(lock_, std::adopt_lock);
  return 0;
}

}
