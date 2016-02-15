#ifndef EVENT_H
#define EVENT_H
namespace service {
struct ActiveEvent {
  int fd;
  int mask;
};
}
#endif
