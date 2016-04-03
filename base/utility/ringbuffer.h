#ifndef UTILITY_RINGBUFFER_H
#define UTILITY_RINGBUFFER_H
#include "common.h"
namespace utility {
#define RINGBUFFER_SIZE 2 << 12
template <class T, size_t kDefaultBufferSize = RINGBUFFER_SIZE>
class RingBuffer : public boost::noncopyable {
public:
  RingBuffer() {
    static_assert(((kDefaultBufferSize > 0) && ((kDefaultBufferSize & (~kDefaultBufferSize + 1)) == kDefaultBufferSize)), "RingBuffer's size must be a positive power of 2");
  }

  T& operator [](int index) {
    return buffer_[index & (kDefaultBufferSize - 1)];
  }

  const T& operator [](int index) const {
    return buffer_[index & (kDefaultBufferSize - 1)];
  }
private:
  T buffer_[kDefaultBufferSize];
};
}
#endif
