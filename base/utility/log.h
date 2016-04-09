#ifndef UTILITY_LOG_H
#define UTILITY_LOG_H
#include <syslog.h>
namespace utility {
#define LOG_OPEN(name) sysopen(name,LOG_NDELAY|LOG_PID,LOG_USER);
#define LOG_DEBUG(...) syslog(LOG_DEBUG, __ARG__);
#define LOG(pri,...) syslog(pri, __ARG__);
#define LOG_CLOSE() closelog();
}
#endif
