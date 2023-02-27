#include <RobotDebug.hpp>
#include <common.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>
using namespace std;
double f(double a) { return (4.0 / (1.0 + a * a)); }
int main() {
  long n = 1000000000;
  long double h = 1.0 / (long double)n;

  std::thread thread1([h, n]() {
    TimeUsedGet time;
    while (1) {
      long double sum = 0.0;
      long i;
      long double x;
      time.mark();
      for (i = 1; i <= n; i++) {
        x = h * (i - 0.5);
        sum += f(x);
      }
      long double pi = h * sum;
      RobotINFO("%Lf, %f", pi, (double)time.Get() / 1000000);
    }
  });

  std::thread thread2([h, n]() {
    TimeUsedGet time;
    while (1) {
      long double sum = 0.0;
      long i;
      long double x;
      time.mark();
      for (i = 1; i <= n; i++) {
        x = h * (i - 0.5);
        sum += f(x);
      }
      long double pi = h * sum;
      RobotINFO("%Lf, %f", pi, (double)time.Get() / 1000000);
    }
  });

  std::thread thread3([h, n]() {
    TimeUsedGet time;
    while (1) {
      long double sum = 0.0;
      long i;
      long double x;
      time.mark();
      for (i = 1; i <= n; i++) {
        x = h * (i - 0.5);
        sum += f(x);
      }
      long double pi = h * sum;
      RobotINFO("%Lf, %f", pi, (double)time.Get() / 1000000);
    }
  });

  while (true) {
    sleep(100000000);
  }

  return 0;
}
