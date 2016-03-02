#ifndef TICKER_H
#define TICKER_H
#include "cinttypes"
#include <string>
namespace utility {
#define OUTPUT_TIME(func, tick) std::cout<<func<<" "<<tick<<std::endl;
inline uint64_t tick(void)
{
    uint32_t lo, hi;
      __asm__ __volatile__ (
          "rdtscp" : "=a"(lo), "=d"(hi)
          );
    return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}

class Ticker {
public:
  Ticker(const char* function_name):function_name_(function_name) { 
    start_ = tick();
  }
  ~Ticker() {
   OUTPUT_TIME(function_name_, start_ - tick());
  } //can log duration
private:
  uint64_t start_;
  const char* function_name_;
};

#define LOG_FUNCTION_DURATION Ticker ticker(__FUNCTION__);
}
#endif
