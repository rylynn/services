#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <assert.h>
#include <string>
namespace service {
using std::string;
static const uint16_t INITSIZE = 2048;
static const uint8_t PREPARE_SIZE = 10;
class Buffer {
public:
  Buffer();
  const char* Peek() { return &*buffer_.begin()+read_index_;}
  void Append(const char* data, uint32_t size) {
    EnsureWritable(size);
    std::copy(data, data + size, buffer_.begin() + write_index_);
    HasWrite(size);
  }

  string ReadAll();

  void Reset() { 
    read_index_ = write_index_ = 0;
  }

  void HasRead(uint32_t len);
  void HasWrite(uint32_t size);
  void Clear() {
    write_index_ = read_index_ = 0;
    buffer_.clear(); 
    buffer_.resize(INITSIZE);
  }

  uint32_t ReadableSize() {return write_index_ - read_index_;}
private:
  void EnsureWritable(size_t len) {
    if (buffer_.capacity() < write_index_ + len) {
      buffer_.reserve(write_index_ + len + PREPARE_SIZE);
    }
  }
private:
  std::vector<char> buffer_;
  uint32_t read_index_;
  uint32_t write_index_;
};
}
#endif
