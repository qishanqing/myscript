#include "searchCharging.h"
namespace findChargingpile
{
    searchCharging::searchCharging() 
    : number (0),  number1 (0), number2(0),  sumX(0.0),sumY(0.0),sumYaw(0.0), receiveData(false),deleteDataNumber(0)
    {
        Init();
    }
    void searchCharging::Init()
    {
        sumX=0.0;
        sumY=0.0;
        number=0;
        number2=0;
        intersectionPoint .x=0.0;
        intersectionPoint .y=0.0;
        ERegionPos.x = 0.0;
        ERegionPos.y =0.0;
        ERegionPos.Yaw = 0.0;
        positionInfrared.x = 0.0;
        positionInfrared.y = 0.0;
        positionInfrared.Yaw = 0.0;
        positionInfrared.Ir_left = BMS::Outside_region;
        positionInfrared.Ir_leftB = BMS::Outside_region;
        positionInfrared.Ir_right = BMS::Outside_region;
        positionInfrared.Ir_rightB = BMS::Outside_region;
        keyPoint.clear();          // 关键点
        secondaryKeyPoint.clear(); // 次关键点
        genericPoint.clear();      // 一般点
        receiveData=true;
    }
   void searchCharging::dataRelease(){
        keyPoint.clear();          // 关键点
        secondaryKeyPoint.clear(); // 次关键点
        genericPoint.clear();      // 一般点
         receiveData=false;
   }
    bool searchCharging::searchCharginginit()
    {
        Init();
    }

    void searchCharging::PointClassification(const SensorInfo *sensor)
    {
       // if ((sqrt(pow(sensor->Posture.pose.x - 0, 2) + pow(sensor->Posture.pose.y - 0, 2)) > 0.20)) 距离充电桩0.2m后开始接受数据
       if(receiveData)
        {
            if (sensor->Bms.Ir_left || sensor->Bms.Ir_leftB || sensor->Bms.Ir_right || sensor->Bms.Ir_rightB)
            {
                if (sensor->Bms.Ir_left != positionInfrared.Ir_left ||
                    sensor->Bms.Ir_leftB != positionInfrared.Ir_leftB ||
                    sensor->Bms.Ir_right != positionInfrared.Ir_right ||
                    sensor->Bms.Ir_rightB != positionInfrared.Ir_rightB ||
                    (sqrt(pow(sensor->Posture.pose.x - positionInfrared.x, 2) + pow(sensor->Posture.pose.y - positionInfrared.y, 2)) > 0.03))
                {
                    positionInfrared.x = sensor->Posture.pose.x;
                    positionInfrared.y = sensor->Posture.pose.y;
                    positionInfrared.Yaw = sensor->Posture.euler.yaw;
                    positionInfrared.Ir_left = sensor->Bms.Ir_left;
                    positionInfrared.Ir_leftB = sensor->Bms.Ir_leftB;
                    positionInfrared.Ir_right = sensor->Bms.Ir_right;
                    positionInfrared.Ir_rightB = sensor->Bms.Ir_rightB;
                    if (sensor->Bms.Ir_left == BMS::E_region || sensor->Bms.Ir_leftB == BMS::E_region ||
                        sensor->Bms.Ir_right == BMS::E_region || sensor->Bms.Ir_rightB == BMS::E_region ||
                        (sensor->Bms.Ir_leftB == BMS::C_region && sensor->Bms.Ir_rightB == BMS::B_region))
                    {
                        NAVLOG(INFO) << "get  keyPoint ";
                        keyPoint.push_back(positionInfrared);
                    }
                    /*else if (sensor->Bms.Ir_left != BMS::E_region && sensor->Bms.Ir_left != BMS::Outside_region ||
                              sensor->Bms.Ir_right != BMS::E_region && sensor->Bms.Ir_right != BMS::Outside_region ||
                              (sensor->Bms.Ir_leftB != BMS::E_region && sensor->Bms.Ir_leftB != BMS::Outside_region &&
                               sensor->Bms.Ir_rightB != BMS::E_region && sensor->Bms.Ir_rightB != BMS::Outside_region))
                     {
                         NAVLOG(INFO) << "get SecondarykeyPoint ";
                         secondaryKeyPoint.push_back(positionInfrared);
                     }*/
                    else
                    {
                        NAVLOG(INFO) << "get  GenericPoint";
                        genericPoint.push_back(positionInfrared);
                    }
                }
            }
        }
    }

    searchCharging::Line1 searchCharging::CalculatingLines(const PositionInfrared currpoint)
    {
        Line1 line;
        line.k = tan(-currpoint.Yaw * (180 / M_PI));
        line.b = currpoint.y - line.k * currpoint.x;
        NAVLOG(INFO) << " [searchCharging1] Calculating line.k ,  line.b={" << line.k << ", " << line.b << "}";
        return line;
    }
    searchCharging::Point1 searchCharging::CalculatingPoint(const Line1 line1, const Line1 line2)
    {
        Point1 point;
        point.x = (line2.b - line1.b) / (line1.k - line2.k);
        point.y = line1.k * point.x + line1.b;
        NAVLOG(INFO) << "[searchCharging1]  Calculating point.x ,  point.y={" << point.x << ", " << point.y << "}";
        return point;
    }

    bool searchCharging::chargingPilePosition(PoseStamped &pileposestampd)
    {
        if (!keyPoint.empty())
        {
            currPos1 = DealWithkeyPoint();
            currPos.Yaw = currPos1.Yaw;
            if (!genericPoint.empty())
            {
                currPos2 = DealWithgenericPoint();
                currPos.x = currPos2.x;
                currPos.y = currPos2.y;
                NAVLOG(INFO) << "  [chargepilePosition] keyPoint() and genericPoint()";
            }
            else
            {
                currPos.x = currPos1.x;
                currPos.y = currPos1.y;
                NAVLOG(INFO) << "  [chargepilePosition]  keyPoint() ";
            }
        }
        else
        {
            currPos = DealWithgenericPoint();
            NAVLOG(INFO) << "  [chargepilePosition]  genericPoint()";
        }

        // currPos = DealWithgenericPoint();
        NAVLOG(INFO) << "  [chargepilePosition] currPos.x , currPos.Y,  currPos.Yaw={" << currPos.x << ", " << currPos.y << ", " << currPos.Yaw << "}";
        pileposestampd.pose.x = currPos.x;
        pileposestampd.pose.y = currPos.y;
        pileposestampd.euler.yaw = currPos.Yaw;
        return true;
    }

    searchCharging::CurrPosition searchCharging ::DealWithkeyPoint()
    {
        for (auto i : keyPoint)
        {
            NAVLOG(INFO) << "   [searchCharging1]  ERegionPos i.x ,  i.Y,  i.Yaw={" << i.x << ", " << i.y << ", " << i.Yaw << "}";
            if ((i.Ir_leftB == BMS::D_region && i.Ir_rightB == BMS::E_region) || (i.Ir_leftB == BMS::E_region && i.Ir_rightB == BMS::A_region))
            {
                if (i.Ir_leftB == BMS::D_region && i.Ir_rightB == BMS::E_region)
                {
                    i.Yaw = i.Yaw - M_PI / 36;
                }
                else
                {
                    i.Yaw = i.Yaw + M_PI / 36;
                }
            }
            if (i.Ir_left == BMS::E_region || i.Ir_right == BMS::E_region)
            {
                if (i.Ir_left == BMS::E_region)
                {
                    i.Yaw = i.Yaw - M_PI_2;
                }
                else
                {
                    i.Yaw = i.Yaw + M_PI_2;
                }
            }
            sumYaw += i.Yaw;
            if (number > 1)
            {
                if (i.Ir_leftB == BMS::E_region || i.Ir_rightB == BMS::E_region || (i.Ir_leftB == BMS::C_region && i.Ir_rightB == BMS::B_region))
                {
                    NAVLOG(INFO) << "  [searchCharging1]  number={" << number << "}";
                    intersectionPoint = CalculatingPoint(CalculatingLines(keyPoint[number - 1]), CalculatingLines(i));
                    NAVLOG(INFO) << "[searchCharging1]  getintersectionpoint.x ,  point.y={" << intersectionPoint.x << ", " << intersectionPoint.y << "}";
                    sumX += intersectionPoint.x;
                    sumY += intersectionPoint.y;
                } 
                NAVLOG(INFO) << "  [searchCharging1]  sumX ,sumY={" << sumX << "," << sumY << "}";
            }
            number++;
        }
        ERegionPos.x = sumX / (number - 2);
        ERegionPos.y = sumY / (number - 2);
        ERegionPos.Yaw = sumYaw / number;
        NAVLOG(INFO) << "  [searchCharging1]  ERegionPos.x ,  ERegionPos.Y,  ERegionPos.Yaw={" << ERegionPos.x << ", " << ERegionPos.y << ", " << ERegionPos.Yaw << "}";
        return ERegionPos;
    }
    searchCharging::CurrPosition searchCharging::DealWithSecondarykeyPoint()
    {
        for (auto i : secondaryKeyPoint)
        {
            NAVLOG(INFO) << "   [searchCharging1]  ERegionPos i.x ,  i.Y,  i.Yaw={" << i.x << ", " << i.y << ", " << i.Yaw << "}";
            if (i.Ir_left || i.Ir_right)
            {
                if (i.Ir_left == BMS::E_region)
                {
                    i.Yaw = i.Yaw - M_PI_2;
                }
                else
                {
                    i.Yaw = i.Yaw + M_PI_2;
                }
            }
            if (number1 > 1)
            {
                for (int j = 1; j < number1; j++)
                {
                    intersectionPoint = CalculatingPoint(CalculatingLines(secondaryKeyPoint[number1 - j]), CalculatingLines(i));
                    sumX += intersectionPoint.x;
                    sumY += intersectionPoint.y;
                }
            }
            number1++;
        }
        pointNumber = (number1 - 1) + (number1 - 1) * (number1 - 2) / 2;
        ERegionPos.x = sumX / pointNumber;
        ERegionPos.y = sumY / pointNumber;
        // ERegionPos.Yaw=sumYaw/number;
        NAVLOG(INFO) << "  [searchCharging1]  ERegionPos.x ,  ERegionPos.Y,  ERegionPos.Yaw={" << ERegionPos.x << ", " << ERegionPos.y << ", " << ERegionPos.Yaw << "}";
        return ERegionPos;
    }

    searchCharging::CurrPosition searchCharging::DealWithgenericPoint()
    {
        std::uint16_t numflag = 0;
        std::uint16_t numflag1= 0;
        std::vector<Point1> Pointv1;
        double pointv1SumX=0.0;
        double pointv1SumY=0.0;
        for (auto i : genericPoint)
        {
            NAVLOG(INFO) << "   [searchCharging2]  ERegionPos i.x ,  i.Y,  i.Yaw={" << i.x << ", " << i.y << ", " << i.Yaw << "}";
            if (number2 > 0)
            {
                for (int j = 1; j <= number2; j++)
                {
                    intersectionPoint = CalculatingPoint(CalculatingLines(genericPoint[number2 - j]), CalculatingLines(i));
                    sumX += intersectionPoint.x;
                    sumY += intersectionPoint.y;
                    Pointv1.push_back(intersectionPoint);
                    numflag++;
                }
            }
            number2++;
            NAVLOG(INFO) << "  [searchCharging2]number2={" << number2 << "}";
        }

        if (numflag > 0)
        {
            ERegionPos.x = sumX / numflag;
            ERegionPos.y = sumY / numflag;
        }
      NAVLOG(INFO) << "  [searchCharging2]numflag={" <<numflag << "}";
       std::vector<ManhattanDistance> mhDis;
       if(numflag>6){
        for(int data=0;data<Pointv1.size();data++){
            manhattanDistance.num=data;
            manhattanDistance.distance= sqrt(pow(Pointv1[data].x-ERegionPos.x, 2) + pow(Pointv1[data].y-ERegionPos.y, 2)) ;
            mhDis.push_back(manhattanDistance);
        }
        NAVLOG(INFO) << "  [searchCharging2]manhattanDistance";
        std::sort(mhDis.begin(),mhDis.end());
        deleteDataNumber=numflag-ceil(numflag*0.8);
         NAVLOG(INFO) << "  [searchCharging2]deleteDataNumber"<< deleteDataNumber;
        for(int n=0;n<deleteDataNumber;n++){
            Pointv1[mhDis[n].num].x=0.0; 
            Pointv1[mhDis[n].num].y=0.0; 
            Pointv1[mhDis[mhDis.size()-1-n].num].x=0.0; 
            Pointv1[mhDis[mhDis.size()-1-n].num].y=0.0; 
        }
        for(auto i:Pointv1){
            pointv1SumX+= i.x;
           pointv1SumY+= i.y;
           numflag1++;
        }
         NAVLOG(INFO) << " [searchCharging2]numflag1++"<<numflag1-2*deleteDataNumber;
         ERegionPos.x =  pointv1SumX/ (numflag1-2*deleteDataNumber);
         ERegionPos.y = pointv1SumY / (numflag1-2*deleteDataNumber);
    }
        NAVLOG(INFO) << "  [searchCharging2]  ERegionPos.x ,  ERegionPos.Y,  ERegionPos.Yaw={" << ERegionPos.x << ", " << ERegionPos.y << ", " << ERegionPos.Yaw << "}";
        return ERegionPos;
    }

} // namespace
