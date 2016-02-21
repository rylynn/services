#ifndef SOCKET_H
#define SOCKET_H
#include "stdlib.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/ioctl.h"
#include "sys/poll.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "common.h"
#include <string>
using ::std::string;
namespace service {
#define INVALID_SOCKET -1
#define RDBUF_SIZE 4096
#define READFRAME  2048
enum PROTOCOL {
  TCP = 6,
  UDP = 17
};

struct Address {
  Address(char* ip, uint16_t port) {
    memcpy(ip_, ip, 32);
    port_ = port;
  }
  Address(uint16_t port) {
    memset(ip_,0,32);
    port_ = port;
  }
  Address() {
    memset(ip_,0,32);
    port_ = 0;
  }
  char ip_[32];
  uint16_t port_;
};

class Buffer;

class Socket 
{
public:
  Socket():fd_(-1), w_index_(0),addr_(12345){};
  Socket(Address& addr, PROTOCOL = TCP);
  explicit Socket(int fd):w_index_(0) { fd_ = fd;}
  ~Socket(){};
  void SetNoneBlock();
  void SetNoDelay();
  void SetReUseAddr();
  int Bind();
  int Listen(int size);
  void Close();
  int Connect();
  int Read(uint32_t& size);
  int Write(char* data, uint32_t size);
  int Read(Buffer& buffer);
  int Write(Buffer& buffer, uint32_t size, uint32_t& haswrite);
  int Accept();
  char* GetRdBuffer() {return rbuf;}
  char* GetWrBuffer() {return wbuf;}
  void ResetRdBuffer();
  void ResetWrBuffer();
public:
  int fd_;
private:
  char rbuf[RDBUF_SIZE];
  char wbuf[RDBUF_SIZE];
  uint32_t w_index_;
  Address addr_;
};
}
#endif
