#ifndef SEARCHCHARGING_H
#define SEARCHCHARGING_H

#include <iostream>
#include "nav_log.h"
#include "Common.h"
#include<vector>
namespace findChargingpile{
class searchCharging
{
public:
   searchCharging();
    ~searchCharging() = default;
    bool searchCharginginit();
    void PointClassification(const SensorInfo * sensor);
    bool chargingPilePosition(PoseStamped &pileposestampd);
    void dataRelease();
private:

  struct PositionInfrared
{
    double x;
    double y;
    double Yaw;
    uint8_t Ir_left;
    uint8_t Ir_leftB;
    uint8_t Ir_right;
    uint8_t Ir_rightB;
}positionInfrared;

  struct CurrPosition
{
    double x;
    double y;
    double Yaw;
}currPos;

  struct Point1
{
    double x;
    double y;
}point;

  struct ManhattanDistance
{
    std::uint16_t num;
    float distance;
    bool operator< (const ManhattanDistance& t) const {
        	return distance < t.distance;
        }
}manhattanDistance;

  struct Line1
{
    double k;
    double b;
}line;
CurrPosition currPos1;
CurrPosition currPos2;
CurrPosition currPos3;
//PoseStamped  pileposestampd;
CurrPosition ERegionPos;
Point1 intersectionPoint;
double sumYaw;
double sumX;
double sumY;
std::uint16_t pointNumber=0;
std::uint16_t number;
std::uint16_t number1;
std::uint16_t number2;
std::uint16_t deleteDataNumber;
std::vector<PositionInfrared>keyPoint; //关键点
std::vector<PositionInfrared>secondaryKeyPoint; //次关键点
std::vector<PositionInfrared>genericPoint; //一般点
bool receiveData;

 void Init();
CurrPosition DealWithkeyPoint();
CurrPosition DealWithSecondarykeyPoint();
CurrPosition DealWithgenericPoint();
Line1 CalculatingLines(const PositionInfrared currpoint);
Point1 CalculatingPoint(const Line1 line1,const Line1  line2);


	cv::Point2f go_start = cv::Point(0.0f,0.0f);
    inline double AngleModify(double x)
    {
        if (x > M_PI)
        {
            x = x - 2 * M_PI;
        }
        else if (x < -M_PI)
        {
            x = x + 2 * M_PI;
        }
        return x;
    }
};
}
#endif // ALONGWALL_H
