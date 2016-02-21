#include "buffer.h"
namespace service {
Buffer::Buffer():read_index_(0), write_index_(0) {
  buffer_.reserve(INITSIZE);
}

void Buffer::HasRead(uint32_t len) {
  assert(read_index_ <= write_index_);
  if (len <= write_index_ - read_index_) {
    read_index_ += len;
  } else {
    read_index_ = write_index_;
  }
}

void Buffer::HasWrite(uint32_t size) {
  write_index_ += size;
  assert(write_index_ <= buffer_.capacity());
}

string Buffer::ReadAll() {
  string allbuf(Peek(), ReadableSize());
  Reset();
  return allbuf;
}

}
