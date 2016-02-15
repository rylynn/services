#include "eventloop.h"
#include <string.h>
#include <string>
#include "common.h"
#include "socket.h"
#include "poller.h"
#include <iostream>
#include "kqueuepoller.h"
namespace service {
bool EventLoop::Init(Poller* poll) {
  bool ret = true;
  do {
  poll_ = poll;
  poll_->Init(event_size_);
  event_map_.clear();
  Address addr("127.0.0.1",12345);
  boost::shared_ptr<Socket>  share_socket_ptr( new Socket(addr,TCP));
  int s_ret = share_socket_ptr->Bind();
  if (s_ret != kReturnok) {
    std::cout<< "bind error" << std::endl;
    ret = false;
    break;
  }
  s_ret = share_socket_ptr->Listen(20);
  if (s_ret != kReturnok) {
    std::cout << "listen error" << std::endl;
    ret = false;
    break;
  }
  share_socket_ptr->SetNoDelay();
  share_socket_ptr->SetReUseAddr();
  event_map_.insert(std::make_pair(share_socket_ptr->fd_, share_socket_ptr));
  ActiveEvent ae;
  ae.fd = share_socket_ptr->fd_;
  server_fd_ = ae.fd;
  ae.mask |= READABLE;
  poll_->AddEvent(ae);
  } while(0);
  return ret;
}

int EventLoop::Run() {
  if (poll_) {
    std::cout<<"start to poll" << std::endl;
    struct timespec tvp;
    tvp.tv_sec = 0;
    tvp.tv_nsec = 500;
    while (1) {
      int ret = poll_->Poll(*this, tvp);
      if (ret == kReturnSysErr) {
        return ret;
      } else if (ret == 0) {
        continue;
      }
      std::cout<<"event comes" << std::endl;
      for (::std::vector<ActiveEvent>::iterator iter = active_events_.begin(); iter != active_events_.end(); iter++) {
        std::map<int , boost::shared_ptr<Socket> >::iterator socket_iter = event_map_.find(iter->fd);
        if ( socket_iter != event_map_.end()){
          if (iter->mask & READABLE) {
            if (iter->fd == server_fd_) {
              int new_fd = socket_iter->second->Accept();
              std::cout<<"accept event comes, new_fd:" << new_fd <<std::endl;
              boost::shared_ptr<Socket> new_socket(new Socket(new_fd));
              new_socket->SetNoneBlock();
              event_map_.insert(std::make_pair(new_fd, new_socket));
              ActiveEvent ae;
              ae.fd = new_fd;
              ae.mask |= READABLE;
              poll_->AddEvent(ae);
            } else {
              uint32_t read_size = 0;
              int r_ret = socket_iter->second->Read(read_size);
              if (r_ret == kReturnSysErr || r_ret == kReturnCloseFd) {
                ActiveEvent ae;
                ae.fd = socket_iter->first;
                ae.mask |= READABLE;
                event_map_.erase(socket_iter);
                poll_->RemoveEvent(ae);
                socket_iter->second->Close();
              }
              if (read_size > 0) {
                std::cout << "on_message:"<< socket_iter->second->GetRdBuffer() << std::endl;
                //socket_iter->second->Write(socket_iter->second->GetRdBuffer(), strnlen(socket_iter->second->GetRdBuffer(), READFRAME));
                socket_iter->second->ResetRdBuffer();
                //socket_iter->second->ResetWrBuffer();
              }
            }
          }
          if (iter->mask & WRITABLE) {
            uint32_t write_size = strnlen(socket_iter->second->GetWrBuffer(), READFRAME);
            if (write_size > 0) {
              int ret = socket_iter->second->Write(socket_iter->second->GetWrBuffer(), write_size);
              if (ret == kReturnSysErr) {
                ActiveEvent ae;
                ae.fd = socket_iter->first;
                ae.mask |= WRITABLE;
                event_map_.erase(socket_iter);
                poll_->RemoveEvent(ae);
                socket_iter->second->Close();
                continue;
              } else if (ret == kReturnEAGAIN) {
                static_cast<KqueuPoller*>(poll_)->EnableWrite(socket_iter->first);
                continue;
              }
              socket_iter->second->ResetWrBuffer();
            }
          }
        } else {
          return kReturnSysErr;
        }
      }
      active_events_.clear();
    }
  } else {
    return kReturnSysErr;
  }
  return kReturnok;
}
}
