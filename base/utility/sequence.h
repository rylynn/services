#ifndef SERVICE_SEQUENCE
#define SERVICE_SEQUENCE
#define CACHE_LINE_SIZE 64
#include <atomic>
namespace utility
class Sequence{
public:
  Sequence():seq_(0){;}

  uint64_t GetSequence() {
    seq_.load(std::memory_order_acquire);
  }
  
  void SetSequence(uint64_t seq) {
    seq_.store(seq, std::memory_order_release);
  }

  uint64_t IncreaseAndGet(uint64_t inc) {
    return seq_.fetch_add(inc, std::memory_order_acq_rel) + inc;
  }

private:
  char padding[CACHE_LINE_SIZE - sizeof(std::atomic<uint64_t>)]; //avoid fake sharing memory
  std::atomic<uint64_t> seq_;
  char padding[CACHE_LINE_SIZE];
};

}
#endif
