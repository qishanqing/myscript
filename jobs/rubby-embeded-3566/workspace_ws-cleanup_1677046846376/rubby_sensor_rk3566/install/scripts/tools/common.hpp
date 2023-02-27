#ifndef __COMMON__HPP
#define __COMMON__HPP
#include <stdint.h>

uint16_t Swap16(uint16_t value);
uint32_t Swap32(uint32_t value);
uint64_t GetSystimeMs(void);
uint64_t GetSystimeUs(void);
uint64_t AttachToSystemTime(uint16_t time);
void MsleepUs(uint32_t us);
void MsleepMs(uint32_t ms);

class TimeUsedGet {
  private:
  uint64_t markTime;

  public:
  TimeUsedGet() {}
  ~TimeUsedGet() {}
  void mark() { markTime = GetSystimeUs(); }
  uint64_t Get() {
    uint64_t now = GetSystimeUs();
    uint64_t timeUsed = now - markTime;
    return timeUsed;
  }
};

#endif