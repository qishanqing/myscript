#pragma once

#include <iostream>
#include <map>

namespace sensor_srvs {
enum SelfTestResult{
	ALLSUCCESS = 0,
	IMUFAILED, 
	IMGFAILED,
	IMUAUXIFAILED,
	CHARGEPOSITIONFAILED,
	MOTORCURRENTFAILED,
	CLIFFFAILED,
	EMERGENCYFAILED,
	BATTERYFAILED,
	LIGHTFLOWFAILED,
	ENCODERFAILED,
	VELOCITYFAILED,
	WALLSENSORFAILED,
	ALL

};
struct SelfTest {
  struct Request {
    std::uint64_t timeStamp;
  } request;
  struct Response {
		uint8_t selfTest[ALL];
		std::uint64_t timeStamp; //ms
	};
};

}  // namespace sensor_srvs