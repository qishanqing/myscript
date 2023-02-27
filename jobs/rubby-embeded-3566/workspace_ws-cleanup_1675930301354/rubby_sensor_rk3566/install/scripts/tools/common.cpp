#include "common.hpp"
#include <sys/time.h>
uint16_t Swap16(uint16_t value) {
  return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

uint32_t Swap32(uint32_t value) {
  return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) |
         ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

uint64_t GetSystimeMs(void) 
{
  struct timeval timenow;
  gettimeofday(&timenow, nullptr);
  return (uint64_t)timenow.tv_sec*1000 + (uint64_t)timenow.tv_usec/1000;
}


uint64_t GetSystimeUs(void) 
{
  struct timeval timenow;
  gettimeofday(&timenow, nullptr);
  return (uint64_t)timenow.tv_sec*1000000 + (uint64_t)timenow.tv_usec;
}

void MsleepMs(uint32_t ms) {
  struct timeval delay;
  delay.tv_sec = ms / 1000;
  delay.tv_usec = (ms * 1000) % 1000000;
  select(0, 0, 0, 0, &delay);
}
void MsleepUs(uint32_t us) {
  struct timeval delay;
  delay.tv_sec = us / 1000000;
  delay.tv_usec = us % 1000000;
  select(0, 0, 0, 0, &delay);
}

uint64_t AttachToSystemTime(uint16_t time) {
  uint64_t now = GetSystimeMs();
  uint16_t timeTail = now & 0xffff;
  uint64_t targetTime = 0;
  if (timeTail > time) {
    if (timeTail - time > 50000) {
      targetTime = (now & (~0xffff)) + ((uint64_t)time & 0xffff) + 0x10000; //传感器时间超前
    } else {
      targetTime = (now & (~0xffff)) + ((uint64_t)time & 0xffff);  //数据正常
    }
  } else {
    if (time - timeTail > 30000) {
      targetTime = (now & (~0xffff)) + ((uint64_t)time & 0xffff) - 0x10000; //传感器时间正常，但是系统时间16位溢出
    } else {
      targetTime = (now & (~0xffff)) + ((uint64_t)time & 0xffff);  //传感器时间超前
    }
  }
  return targetTime * 1000;
}