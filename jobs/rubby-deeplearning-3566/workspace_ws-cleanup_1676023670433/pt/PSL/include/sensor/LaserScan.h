#pragma once

#include <vector>
#include "base/Time.h"

namespace swr {
namespace sensor_types {

struct LaserScan {
  struct Header {
    std::uint32_t seq;
    psl::Time stamp;
  } header;
  float angle_min;
  float angle_max;
  float angle_increment;
  float time_increment;
  float scan_time;
  float range_min;
  float range_max;
  std::vector<float> ranges;
  std::vector<float> intensities;
};

}
}