#include <stdlib.h>
#include <string.h>

#include <RobotDebug.hpp>
#include <common.hpp>
#include <iostream>
#include <map>

#define TEST_SIZE (1024 * 1024 * 1024)

static uint8_t *g_ptr;

// 分别对TEST_SIZE 空间做测试, 返回 copy速度， 单位 Byte
double TestMemSpeed(int blocksize) {
  uint8_t *ptr = (uint8_t *)malloc(TEST_SIZE);
  uint8_t *data = (uint8_t *)malloc(blocksize);

  for (long i = 0; i < blocksize; i++) {
    data[i] = i;
  }

  TimeUsedGet time;

  time.mark();
  long copy_times = TEST_SIZE / blocksize;
  for (long i = 0; i < copy_times; i++) {
    memcpy(data, ptr + i * blocksize, blocksize);
  }
  double write_speed = TEST_SIZE / ((double)(time.Get()) / 1000000);
  if (blocksize < 1024) {
    RobotINFO("block %d, speed %fGB/s", blocksize, write_speed / 1024 / 1024 / 1024);

  } else if (blocksize < 1024 * 1024) {
    RobotINFO("block %dK, speed %fGB/s", blocksize / 1024, write_speed / 1024 / 1024 / 1024);

  } else {
    RobotINFO("block %dM, speed %fGB/s", blocksize / 1024 / 1024, write_speed / 1024 / 1024 / 1024);
  }
  free(ptr);
  free(data);
  return write_speed;
}

int main() {
  long block = 2;
  long num = 0;
  double write_speed = 0;
  std::map<long, double> speedmap;
  for (int i = 1; i < 22; i++) {
    block *= 2;
    write_speed = TestMemSpeed(block);
    num = write_speed/block; // 每秒操作n个block
    RobotWARN("num=%ld", num);
    speedmap.insert(std::make_pair(block, write_speed));
  }

  double memocc=0.75;
  long test_div = 100; //分100次测试完
  long per_test_count=0;
  uint64_t per_sleep_time = 0;
  TimeUsedGet time;
  TimeUsedGet time_perloop;
  block = 4096;
  num = speedmap[block]/block;

  long testnum = num * memocc;
  per_test_count = testnum / test_div;
  per_sleep_time = 1000000 / test_div;

  RobotWARN("now test %ld, speed %f, numblocks %ld, every test sleep %uus", block, speedmap[block], num, per_sleep_time);
  uint8_t *ptr = (uint8_t *)malloc(TEST_SIZE);
  uint8_t *data = (uint8_t *)malloc(block);
  for (long i = 0; i < block; i++) {
    data[i] = i;
  }

  int sum = 0;
  uint64_t used_time = 0;
  uint64_t timedecrease = 0;

  while (1) {
    if(sum == 0){
      time.mark();
    }
    time_perloop.mark();
    for (long i = 0; i < per_test_count; i++) {
      memcpy(data, ptr + i * block, block);
    }
    sum += per_test_count;
    if(sum >= testnum){
      sum = 0;
      used_time = time.Get();
      if(used_time > 1000000){
        timedecrease = used_time - 1000000;
        timedecrease = timedecrease / 100;
      }else{
        timedecrease = 1000000 - used_time;
        timedecrease = timedecrease / 100;
      }
      RobotERR("used time %fs", (double)used_time/1000000);
    }
    uint64_t sleep_time = per_sleep_time - time_perloop.Get() - timedecrease;
    if(sleep_time > 1000000){
      sleep_time = 10000;
    }
    MsleepUs(sleep_time);
  }
  return 0;
}





