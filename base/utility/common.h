#ifndef UTILITY_COMMON_H
#define UTILITY_COMMON_H
#include <boost/noncopyable.hpp>
#include <thread>
#ifdef DEFINE_INT_TYPES
#include <cinttypes>
#include <inttypes.h>
#endif
static int kReturnok = 0;
static int kReturnSysErr = -1;
static int kReturnArgumentErr = 1;
static int kReturnCloseFd = 101;
static int kReturnEAGAIN = 102;

static int kNullChannel = 0;

#endif
