#pragma once

#include <cstdint>
#include <mind_os/serialization/serialization.h>
namespace sensor_msgs {

struct MachineMode {
  enum MACHINE_MODE_NUM { 
		NULL_0 = 0, 
		MACHINE_SLEEP, 
		MACHINE_WAIT, 
		MACHINE_DC_CHARGE, 
		MACHINE_AUTO_CHARGE, 
		MACHINE_RUN, 
		MACHINE_TEST, 
		MACHINE_WIFI_SLEEP 
		};

  uint8_t machineModeData;
  std::uint64_t timeStamp;
};

}  // namespace sensor_msgs
namespace mind_os {
template <>
inline void serialize(const sensor_msgs::MachineMode& data, BytesBuffer& buffer) {
   serialize(data.machineModeData, buffer);
   serialize(data.timeStamp, buffer);
}; /* serialization of sensor_msgs::BatteryEvent */

template <>
inline void deserialize(BytesBuffer& buffer, sensor_msgs::MachineMode& data) {
   deserialize(buffer,data.machineModeData);
   deserialize(buffer,data.timeStamp);
};
}  // namespace mind_os
