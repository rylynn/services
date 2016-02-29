#ifndef CHANNEL_H
#define CHANNEL_H
#include "socket.h"
#include "buffer.h"
namespace service {
class Channel {
public:
  Channel(Socket& socket):socket_(socket){;}
  int fd() {return socket_.fd_;}
  int Read();
  int Write(const char* buffer, uint32_t size);
  int Write();
  void AsynWrite(const char* buffer, uint32_t size);
  int Accept() { return socket_.Accept();}
  Buffer& GetRdBuffer(){ return rd_buffer_;}
  Buffer& GetWrBuffer() {return wr_buffer_;}
  void ResetWrBuffer() { wr_buffer_.Clear();}
  void ResetRdBuffer() { rd_buffer_.Clear();}
  void Close() { 
    socket_.Close();
    wr_buffer_.Clear();
    rd_buffer_.Clear();
  }
private:
  Socket socket_;
  Buffer wr_buffer_;
  Buffer rd_buffer_;
};
}
#endif
