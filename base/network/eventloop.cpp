#include "eventloop.h"
#include <string.h>
#include <string>
#include "common.h"
#include "socket.h"
#include "channel.h"
#include "poller.h"
#include <iostream>
#include "kqueuepoller.h"
#include "eventdispatcher.h"
namespace service {
bool EventLoop::Init(Poller* poll, EventDispatcher* dispatcher) {
  bool ret = true;
  do {
  dispatcher_ = dispatcher;
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
  boost::shared_ptr<Channel> shared_channel_ptr(new Channel(*share_socket_ptr.get()));
  event_map_.insert(std::make_pair(shared_channel_ptr->fd(), shared_channel_ptr));
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
    std::cout<<"loop start" << std::endl;
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
        std::map<int , boost::shared_ptr<Channel> >::iterator channel_iter = event_map_.find(iter->fd);
        if ( channel_iter != event_map_.end()){
          if (iter->mask & READABLE) {
            if (iter->fd == server_fd_) {
              int new_fd = channel_iter->second->Accept();
              std::cout<<"accept event comes, new_fd:" << new_fd <<std::endl;
              boost::shared_ptr<Socket> new_socket(new Socket(new_fd));
              new_socket->SetNoneBlock();
              boost::shared_ptr<Channel> new_channel(new Channel(*new_socket.get()));
              event_map_.insert(std::make_pair(new_fd, new_channel));
              ActiveEvent ae;
              ae.fd = new_fd;
              ae.mask |= READABLE;
              poll_->AddEvent(ae);
            } else {
              int r_ret = channel_iter->second->Read();
              if (r_ret == kReturnSysErr || r_ret == kReturnCloseFd) {
                RemoveChannel(channel_iter->second);
                continue;
              } else if (r_ret == kReturnok && channel_iter->second->GetRdBuffer().ReadableSize() > 0) {
                dispatcher_->DispatcherEvent(READEVENT, channel_iter->first, channel_iter->second->GetRdBuffer());
              }
            }
          } else if (iter->mask & WRITABLE) {
            uint32_t write_size = channel_iter->second->GetWrBuffer().ReadableSize();
            if (write_size > 0) {
              int ret = channel_iter->second->Write(channel_iter->second->GetWrBuffer().Peek(), write_size);
              channel_iter->second->GetWrBuffer().HasRead(write_size);
              if (ret == kReturnSysErr) {
                RemoveChannel(channel_iter->second);
                continue;
              } else if (ret == kReturnEAGAIN) {
                static_cast<KqueuPoller*>(poll_)->EnableWrite(channel_iter->first);
                continue;
              }
              if (channel_iter->second->GetWrBuffer().ReadableSize() == 0) {
                channel_iter->second->GetWrBuffer().Clear();
                static_cast<KqueuPoller*>(poll_)->DisableWrite(channel_iter->first);
              }
            }
          } else if (iter->mask & ERROR) {
            dispatcher_->DispatcherEvent(ERROREVENT, channel_iter->first , channel_iter->second->GetWrBuffer());
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

void EventLoop::RemoveChannel(boost::shared_ptr<Channel>& channel) {
    ActiveEvent ae;
    ae.fd = channel->fd();
    ae.mask = WRITABLE | READABLE;
    channel->Close();
    poll_->RemoveEvent(ae);
    event_map_.erase(channel->fd());
}

}
