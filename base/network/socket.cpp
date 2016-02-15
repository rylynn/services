#include "socket.h"
#include "stdlib.h"
#include <unistd.h>
#include <fcntl.h> 
#include <errno.h>
#include <iostream>
namespace service {
Socket::Socket(Address& addr, PROTOCOL protocol):fd_(-1), w_index_(0),addr_(addr) {
  if (protocol == TCP) {
    fd_ = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  } else {
    fd_ = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  }
}

int Socket::Bind() {
  if (fd_ == -1) {
    std::cout << "fd == -1" << std::endl;
    return kReturnSysErr;
  }
  sockaddr_in sock_addr;
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_port = htons(addr_.port_);
  if (atoi(addr_.ip_) == 0)
    sock_addr.sin_addr.s_addr = INADDR_ANY;
  else 
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    //sock_addr.sin_addr.s_addr = htonl(inet_addr(addr_.ip_));
  int ret = bind(fd_, (sockaddr*) &sock_addr, sizeof(sock_addr));
  return ret;
}

int Socket::Listen(int size) {
  return listen(fd_,size);
}

void Socket::Close() {
  std::cout<<"fd:" << fd_ << " close" << std::endl; 
  close(fd_);
  fd_ = -1;
}

int Socket::Connect() {
  sockaddr_in sock_addr;
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_port = htons(addr_.port_);
  sock_addr.sin_addr.s_addr = htonl(inet_addr(addr_.ip_));
  return connect(fd_, (sockaddr*)&sock_addr, sizeof(sock_addr));
}

int Socket::Read(uint32_t& size) {
  std::cout<<"[+]Socket::Read()"<< std::endl;
  int rbytes = 0;
  int r_ret = 0;
  do {
    r_ret = read(fd_, rbuf,READFRAME);
    if (r_ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      r_ret = kReturnok;
      break;
    } else if (r_ret < 0) {
      return kReturnSysErr;
    } else if (r_ret == 0) {
      return kReturnCloseFd;
    }
    rbytes += r_ret;
  } while(1);
  std::cout<<"total bytes"<<rbytes<<" socket recv:" << rbuf << std::endl;
  std::cout<<"[-]Socket::Read(),ret:"<<r_ret<< std::endl;
  size = rbytes;
  return r_ret;
}

int Socket::Write(char* data, uint32_t size) {
  if (size == 0) {
    return kReturnSysErr;
  }
  std::cout<<"[+]Socket::Write" << std::endl;
  int wbytes = 0;
  int w_ret = 0;
  uint32_t real_size = size;
  memcpy(wbuf+w_index_, data, size>RDBUF_SIZE?RDBUF_SIZE:size);
  if (w_index_ != 0) {
    real_size = strnlen(wbuf+w_index_, RDBUF_SIZE);
  }
  do {
    w_ret = write(fd_,wbuf+w_index_,real_size>RDBUF_SIZE?RDBUF_SIZE:real_size);
    if (w_ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      if (wbytes < size) {
        w_index_ = wbytes;
      }
      w_ret = kReturnEAGAIN;
      break;
    } else if(w_ret < 0) {
      wbytes = kReturnSysErr;
      break;
    }
    wbytes += w_ret;
    std::cout<<"loop write bytes:" << wbytes << std::endl;
    if (wbytes >= real_size) break;
  } while(1);
  std::cout<<"[-]Socket::Write, ret:"<<w_ret<< " write data:"<< data <<std::endl;
  return wbytes;
}

void Socket::SetNoneBlock() {
  fcntl(fd_,F_SETFL,O_NONBLOCK);
}

void Socket::SetNoDelay() {
  int optval =1;
  ::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY,
      &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::SetReUseAddr() {
  int optval = 1;
  ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR,
      &optval, static_cast<socklen_t>(sizeof optval));
}

int Socket::Accept() {
  return accept(fd_, NULL, NULL);
}

void Socket::ResetRdBuffer() {
  memset(rbuf, 0 ,RDBUF_SIZE);
}

void Socket::ResetWrBuffer() {
  memset(wbuf, 0 ,RDBUF_SIZE);
}
}
