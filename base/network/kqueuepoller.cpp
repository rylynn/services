#include "kqueuepoller.h"
#include <assert.h>
#include <stdlib.h>
#include "socket.h"
#include "eventloop.h"
#include "common.h"
#include <iostream>
namespace service {
int KqueuPoller::Init(uint32_t size) {
  kevent_ptr_ = (struct kevent*)malloc(sizeof(struct kevent) * kEventSize);
  assert(kevent_ptr_);
  kqueufd_ = kqueue(); 
  assert(kqueufd_ != -1);
  return kReturnok;
}

int KqueuPoller::Poll(EventLoop& loop , struct timespec& timeout) {
  int retval = 0;
  if (timeout.tv_sec == 0 && timeout.tv_nsec == 0) {
    retval = kevent(kqueufd_, NULL, 0, kevent_ptr_, kEventSize, NULL);
  } else {
    retval = kevent(kqueufd_, NULL, 0, kevent_ptr_, kEventSize, &timeout);
  }
  if (retval > 0) {
    for (int i = 0; i < retval; ++i) {
      ActiveEvent ae;
      ae.mask = 0;
      ae.fd = kevent_ptr_[i].ident;
      if (kevent_ptr_[i].filter == EVFILT_READ) ae.mask |= READABLE;
      if (kevent_ptr_[i].filter == EVFILT_WRITE) ae.mask |= WRITABLE;
      loop.PollData().push_back(ae);
    }
  }
  return retval;
}

int KqueuPoller::AddEvent(ActiveEvent& ev) {
  struct kevent ke;
  if (ev.mask & READABLE) {
    EV_SET(&ke, ev.fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kqueufd_, &ke, 1, NULL, 0, NULL) == -1) return kReturnSysErr;
  }
  if (ev.mask & WRITABLE) {
    EV_SET(&ke, ev.fd, EVFILT_WRITE , EV_ADD | EV_DISABLE, 0, 0, NULL);
    if (kevent(kqueufd_, &ke, 1, NULL, 0, NULL) == -1) return kReturnSysErr;
  }
  return kReturnok;
}

void KqueuPoller::RemoveEvent(ActiveEvent& ev) {
  struct kevent ke;
  if (ev.mask & READABLE) {
    EV_SET(&ke, ev.fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kqueufd_, &ke, 1, NULL, 0, NULL);
  }
  if (ev.mask & WRITABLE) {
    EV_SET(&ke, ev.fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(kqueufd_, &ke, 1, NULL, 0, NULL);
  }
}

void KqueuPoller::EnableWrite(int fd) {
  struct kevent ke;
  EV_SET(&ke, fd, EVFILT_WRITE ,EV_ENABLE, 0, 0, NULL);
  kevent(kqueufd_, &ke, 1, NULL, 0, NULL);
}

}
