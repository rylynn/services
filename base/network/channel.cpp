#include "channel.h"
#include <iostream>
namespace service{
int Channel::Read() {
  return socket_.Read(rd_buffer_);
}

int Channel::Write(const char* buffer, uint32_t size) {
  wr_buffer_.Append(buffer, size);
  uint32_t haswrite = 0;
  int ret = socket_.Write(wr_buffer_, size, haswrite);
  std::cout<<"Channel::Write(), size:" << size << " haswrite" << haswrite<< std::endl;
  wr_buffer_.HasRead(haswrite);
  return ret;
}

}
