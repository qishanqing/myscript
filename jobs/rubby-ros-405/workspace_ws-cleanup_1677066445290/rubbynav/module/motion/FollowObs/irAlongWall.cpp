#include "irAlongWall.h"

IrAlongWall::IrAlongWall()
    : vx(0.0f), wz(0.0f), Kp(0.2f), Ki(0.015f), Kd(0.2f), err(0.0f), err_last(0.0f), integral(0.0f), voltage(0.0f), originAngle(0.0f), lastTheta(0.0f),
      originalLeftWallDis(24.0f),
      originalRightWallDis(24.0f),
      objectLeftWallDis(43.0f),
      objectRightWallDis(43.0f)
{
    Init();
}
void IrAlongWall::Init()
{
    AlongWallStep = STEP_IDLE;
    AlongWallMode = MODE_ALONG_OBJECT;

    /*Kp = 0.035; //  cmd.vl = 0.15f时PID参数
    Ki= 0.00001;
    Kd=0.12;
    err = 0.0f;*/

    Kp = 0.02;
    Ki = 0.015;
    Kd = 0.05;
    err = 0.0f;
}

bool IrAlongWall::UpdateVelocity(const SensorInfo *sensor, SpeedControlInfo &cmd)
{
    NAVLOG_EVERY_SEC(INFO, 1, "along wall pub velocity");
    switch (AlongWallStep)
    {
    case STEP_TEST_MANUAL_START:
        NAVLOG_EVERY_SEC(INFO, 1.0, "wait manual start");
        break;
    case STEP_IDLE:
        // IDLE state: begain to turn
        {
            NAVLOG_EVERY_SEC(INFO, 1.0, "along wall STEP_IDLE");
            if (sensor->Followir.Ir_Left_mm < sensor->Followir.Ir_Right_mm)
            {
                if (sensor->Followir.Ir_Left_mm - objectLeftWallDis < 15)
                {
                    AlongWallStep = STEP_LEFTSTRAIGHTWALL;
                    NAVLOG_EVERY_SEC(INFO, 1.0, "along wall STEP_LEFTSTRAIGHTWALL");
                }
            }
            else
            {
                if (sensor->Followir.Ir_Right_mm - objectRightWallDis < 10)
                {
                    AlongWallStep = STEP_RIGHTSTRAIGHTWALL;
                    NAVLOG_EVERY_SEC(INFO, 1.0, "along wall STEP_RIGHTSTRAIGHTWALL");
                }
            }
        }
        break;
    case STEP_LEFTSTRAIGHTWALL:
    {
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            if (sensor->Bumper.Bumper_Right)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_LEFTSTRAIGHTWALL_RIGHTBUMPER_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_LEFTSTRAIGHT_RIGHTPUMBER;
            }
            else
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_LEFTSTRAIGHTWALL_sensor->Bumper.Bumper_Left_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_LEFTSTRAIGHT_LEFTPUMBER;
            }
        }
        else
        {
            LeftStraightWall(sensor, cmd);
        }
    }
    break;
    case STEP_RIGHTSTRAIGHTWALL:
    {
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            AlongWallStep = STEP_RIGHTPUMBER;
            originAngle = AngleModify(sensor->Posture.euler.yaw);
        }
        else
        {
            RightStraightWall(sensor, cmd);
        }
    }
    break;
    case STEP_LEFTARCWALL:
        // Left Go ARC Along Wall
        {
            if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
            {
                if (sensor->Bumper.Bumper_Right)
                {
                    NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_LEFTARCWALL_RIGHTBUMPER_TRIGGER");
                    originAngle = AngleModify(sensor->Posture.euler.yaw);
                    AlongWallStep = STEP_LEFTSTRAIGHT_RIGHTPUMBER;
                }
                else
                {
                    NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_LEFTARCWALL_LEFTBUMPER_TRIGGER");
                    originAngle = AngleModify(sensor->Posture.euler.yaw);
                    AlongWallStep = STEP_LEFTSTRAIGHT_LEFTPUMBER;
                }
            }
            else
            {
                cmd.vl = 0.0601f;
                cmd.va = 0.3f;
                if (sensor->Followir.Ir_Left_mm - objectLeftWallDis < 10)
                {
                    cmd.vl = 0.0f;
                    cmd.va = 0.0f;
                    AlongWallStep = STEP_IDLE;
                    NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_LEFTARCWALL  go to along wall   STEP_LEFTSTRAIGHTWALL");
                }
            }
        }
        break;
    case STEP_RIGHTARCWALL:
    {
        cmd.vl = 0.0601f;
        cmd.va = -0.3f;
        if (sensor->Followir.Ir_Right_mm - objectRightWallDis < 10)
        {
            cmd.vl = 0.0f;
            cmd.va = -0.0f;
            AlongWallStep = STEP_IDLE;
            NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_RIGHTARCWALL  go to along wall   STEP_RIGHTSTRAIGHTWALL");
        }
    }
    break;
    case STEP_LEFTSTRAIGHT_LEFTPUMBER:
    {
        cmd.vl = -0.01f;
        cmd.va = -0.3f;
        if (std::fabs(originAngle - AngleModify(sensor->Posture.euler.yaw)) > 0.785)
        {
            AlongWallStep = STEP_LEFTSTRAIGHTWALL;
        }
    }
    break;
    case STEP_LEFTSTRAIGHT_RIGHTPUMBER:
    {
        cmd.vl = -0.01f;
        cmd.va = -0.5f;
        if (std::fabs(originAngle - AngleModify(sensor->Posture.euler.yaw)) > 1.57)
        {
            AlongWallStep = STEP_LEFTSTRAIGHTWALL;
        }
    }
    break;
    case STEP_RIGHTPUMBER:
    {
        NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_RIGHTSTRAIGHTWALL_BUMPER_TRIGGER");
        cmd.vl = -0.01f;
        cmd.va = 0.3f;
        if (std::fabs(originAngle - AngleModify(sensor->Posture.euler.yaw)) > 0.2)
        {
            AlongWallStep = STEP_RIGHTSTRAIGHTWALL;
        }
    }
    break;
    default:
        NAVLOG(INFO) << "Dock default step: ";
        break;
    }
}
bool IrAlongWall::allongWallInit()
{
    Init();
}
int IrAlongWall::allongWallLeft(const SensorInfo *sensor, SpeedControlInfo &cmd)
{
    // NAVLOG_EVERY_SEC(INFO, 1, "along wall pub velocity");
    switch (AlongWallStep)
    {
    case STEP_TEST_MANUAL_START:
        NAVLOG_EVERY_SEC(INFO, 1.0, "wait manual start");
        break;
    case STEP_IDLE:
        // IDLE state: begain to turn
        AlongWallStep = STEP_LEFTSTRAIGHTWALL;
        NAVLOG_EVERY_SEC(INFO, 1.0, "along wall STEP_IDLE");
        break;
    case STEP_LEFTSTRAIGHTWALL:
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            if (sensor->Bumper.Bumper_Left)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_LEFTSTRAIGHTWALL_RIGHTBUMPER_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_LEFTSTRAIGHT_LEFTPUMBER;
            }
            else
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_LEFTSTRAIGHTWALL_sensor->Bumper.Bumper_Left_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_LEFTSTRAIGHT_RIGHTPUMBER;
            }
        }
        else
        {
            LeftStraightWall(sensor, cmd);
        }
        break;
    case STEP_LEFTARCWALL:
        // Left Go ARC Along Wall
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            if (sensor->Bumper.Bumper_Left)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_LEFTARCWALL_RIGHTBUMPER_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_LEFTSTRAIGHT_LEFTPUMBER;
            }
            else
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_LEFTARCWALL_LEFTBUMPER_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_LEFTSTRAIGHT_RIGHTPUMBER;
            }
        }
        else
        {
            cmd.vl = 0.1f;
            cmd.va = 1.0f;
            if (sensor->Followir.Ir_Left_mm - objectLeftWallDis < 20)
            {
                cmd.vl = 0.0f;
                cmd.va = 0.0f;
                AlongWallStep = STEP_IDLE;
                NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_LEFTARCWALL  go to along wall   STEP_LEFTSTRAIGHTWALL");
            }
        }
        break;
    case STEP_LEFTSTRAIGHT_LEFTPUMBER:
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            cmd.vl = -0.1f;
            cmd.va = 0.0f;
        }
        else
        {
            cmd.vl = -0.01f;
            cmd.va = -0.8f;

            if (std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 1.5f)
            {
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_LEFTSTRAIGHT_LEFTPUMBER_RE;
            }
        }
        break;
    case STEP_LEFTSTRAIGHT_LEFTPUMBER_RE:
        cmd.vl = 0.0f;
        cmd.va = 0.8f;
        if (std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 1.0f || sensor->Followir.Ir_Left_mm - objectLeftWallDis < 20)
        {
            AlongWallStep = STEP_LEFTSTRAIGHTWALL;
        }
        break;
    case STEP_LEFTSTRAIGHT_RIGHTPUMBER:
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            cmd.vl = -0.1f;
            cmd.va = 0.0f;
        }
        else
        {
            cmd.vl = -0.01f;
            cmd.va = -0.8f;

            if (std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 3.0f)
            {
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_LEFTSTRAIGHT_RIGHTPUMBER_RE;
            }
        }

        break;
    case STEP_LEFTSTRAIGHT_RIGHTPUMBER_RE:
        cmd.vl = 0.0f;
        cmd.va = 0.8f;
        if (std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 2.0f || sensor->Followir.Ir_Left_mm - objectLeftWallDis < 20)
        {
            AlongWallStep = STEP_LEFTSTRAIGHTWALL;
        }
        break;
    default:
        NAVLOG(INFO) << "Dock default step: ";
        break;
    }
    NAVLOG(INFO) << "left AlongWallStep: " << AlongWallStep;
    return AlongWallStep;
}
int IrAlongWall::allongWallRight(const SensorInfo *sensor, SpeedControlInfo &cmd)
{
    // NAVLOG_EVERY_SEC(INFO, 1, "along wall pub velocity");
    switch (AlongWallStep)
    {
    case STEP_TEST_MANUAL_START:
        NAVLOG_EVERY_SEC(INFO, 1.0, "wait manual start");
        break;
    case STEP_IDLE:
        // IDLE state: begain to turn
        AlongWallStep = STEP_RIGHTSTRAIGHTWALL;
        // AlongWallStep = STEP_RIGHT_OBJECT_SIZE1; // 测试专用
       // alongObjectFlag = false;
        minDistance = 100;
        NAVLOG_EVERY_SEC(INFO, 1.0, "along wall STEP_IDLE");
        break;
    case STEP_RIGHTSTRAIGHTWALL:
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            if (sensor->Bumper.Bumper_Right)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_RIGHTSTRAIGHTWALL_RIGHTBUMPER_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_RIGHTSTRAIGHT_RIGHTPUMBER;
            }
            else
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_RIGHTSTRAIGHTWALL_ELSEBUMPER_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_RIGHTSTRAIGHT_LEFTPUMBER;
            }
        }
        else
        {
            RightStraightWall(sensor, cmd);
        }
        break;
    case STEP_RIGHTARCWALL:
        // Left Go ARC Along Wall
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            if (sensor->Bumper.Bumper_Right)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_RIGHTARCWALL_RIGHTBUMPER_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_RIGHTSTRAIGHT_RIGHTPUMBER;
            }
            else
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_RIGHTARCWALL_LEFTBUMPER_TRIGGER");
                originAngle = AngleModify(sensor->Posture.euler.yaw);
                AlongWallStep = STEP_RIGHTSTRAIGHT_LEFTPUMBER;
            }
        }
        else
        {
            cmd.vl = 0.1f;
            cmd.va = -1.0f;
            if (sensor->Followir.Ir_Right_mm - objectRightWallDis < 20)
            {
                cmd.vl = 0.0f;
                cmd.va = 0.0f;
                AlongWallStep = STEP_IDLE;
                NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_RIGHTARCWALL  go to along wall   STEP_RIGHTSTRAIGHTWALL");
            }
        }
        break;
    case STEP_RIGHTSTRAIGHT_LEFTPUMBER:
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            cmd.vl = -0.1f;
            cmd.va = 0.0f;
        }
        else
        {
            if (AlongWallMode == MODE_ALONG_WALL)
            {
                cmd.vl = -0.01f;
                cmd.va = 0.8f;
                if (std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 3.0f)
                {
                    originAngle = AngleModify(sensor->Posture.euler.yaw);
                    AlongWallStep = STEP_RIGHTSTRAIGHT_LEFTPUMBER_RE;
                }
            }
            else
            {
               /*if (sensor->Followir.Ir_Right_mm > 70)
                {
                    alongObjectFlag = true;
                }
                */ 
                AlongWallStep = STEP_RIGHT_OBJECT_SIZE1;
                cmd.vl = -0.05f;
                cmd.va = 0.0f;
                lastPositionX = sensor->Posture.pose.x;
                lastPositionY = sensor->Posture.pose.y;
            }
        }
        break;
    case STEP_RIGHTSTRAIGHT_LEFTPUMBER_RE:
        cmd.vl = 0.0f;
        cmd.va = -0.8f;
        // if(std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 1.5f || sensor->Followir.Ir_Right_mm - objectRightWallDis < 20){
        if (std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 2.0f || sensor->Followir.Ir_Right_mm - objectRightWallDis < 20)
        {
            AlongWallStep = STEP_RIGHTSTRAIGHTWALL;
        }
        break;
    case STEP_RIGHTSTRAIGHT_RIGHTPUMBER:
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            cmd.vl = -0.1f;
            cmd.va = 0.0f;
        }
        else
        {
            LOG(INFO) << "AlongWallMode :" << AlongWallMode;
            if (AlongWallMode == MODE_ALONG_WALL)
            {
                cmd.vl = -0.01f;
                cmd.va = 0.8f;
                if (std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 1.57f)
                {
                    originAngle = AngleModify(sensor->Posture.euler.yaw);
                    AlongWallStep = STEP_RIGHTSTRAIGHT_RIGHTPUMBER_RE;
                }
            }
            else
            {
                /*
                if (sensor->Followir.Ir_Right_mm > 70)
                {
                    alongObjectFlag = true;
                }
                */
                AlongWallStep = STEP_RIGHT_OBJECT_SIZE1;
                cmd.vl = -0.05f;
                cmd.va = 0.0f;
                lastPositionX = sensor->Posture.pose.x;
                lastPositionY = sensor->Posture.pose.y;
            }
        }
        break;
    case STEP_RIGHTSTRAIGHT_RIGHTPUMBER_RE:
        cmd.vl = 0.0f;
        cmd.va = -0.8f;
        // if(std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 0.785f || sensor->Followir.Ir_Right_mm - objectRightWallDis < 20){
        if (std::fabs(AngleModify(originAngle - sensor->Posture.euler.yaw)) > 1.0f || sensor->Followir.Ir_Right_mm - objectRightWallDis < 20)
        {
            AlongWallStep = STEP_RIGHTSTRAIGHTWALL;
        }
        break;
    case STEP_RIGHT_OBJECT_SIZE1:
    {
        if (sqrt(pow(lastPositionX - sensor->Posture.pose.x, 2) + pow(lastPositionY - sensor->Posture.pose.y, 2)) > 0.02)
        {
         
            NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_RIGHT_OBJECT_SIZE1");
            if (distanceTooClose)
            {
                cmd.vl = 0.0f;
                cmd.va = -0.6f;
                bumperStartAngle= sensor->Posture.euler.yaw;
                AlongWallStep =   STEP_RIGHT_ROTATUON_ANGLE;
                distanceTooClose=false;
            }
            else
            {
                cmd.vl = 0.0f;
                // cmd.va = 0.3f;
                 cmd.va = 0.6f;
                rotationAngle = 0.0;
                // static std::uint32_t disCount = 0;
               /* if (sensor->Followir.Ir_Right_mm > 70)
                {
                    alongObjectFlag = true;
                }
                */
              //   if (alongObjectFlag && sensor->Followir.Ir_Right_mm <= 70)
               if (sensor->Followir.Ir_Right_mm <= 70)
                {
                    startAngle = sensor->Posture.euler.yaw;
                    AlongWallStep = STEP_RIGHT_OBJECT_SIZE2;
                    LOG(INFO) << "startAngle=" << startAngle;
                    if (minDistance > sensor->Followir.Ir_Right_mm)
                    {
                        minDistance = sensor->Followir.Ir_Right_mm;
                    }
                }
            }
        }
    }
    break;

    case STEP_RIGHT_OBJECT_SIZE2:
    {
        NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_RIGHT_OBJECT_SIZE2");
        if (minDistance > sensor->Followir.Ir_Right_mm)
        {
            minDistance = sensor->Followir.Ir_Right_mm;
        }
        if (sensor->Followir.Ir_Right_mm > 70)
        {
            rotationAngle = std::fabs(startAngle - sensor->Posture.euler.yaw) * (180 / M_PI);
            LOG(INFO) << " rotationAngle=" << rotationAngle;
            AlongWallStep = STEP_RIGHT_ALONG_OBSTACLE;
            //  AlongWallStep = STEP_RIGHT_OBJECT_SIZE1;//测试专用
            lastPositionX = sensor->Posture.pose.x;
            lastPositionY = sensor->Posture.pose.y;
            cmd.vl = 0.0f;
            cmd.va = -0.4f;
        }
    }
    break;

    case STEP_RIGHT_ALONG_OBSTACLE:
    {
        NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_RIGHT_ALONG_OBSTACLE");
        NAVLOG_EVERY_SEC(INFO, 1.0, "minDistance=" << minDistance);
        if (sensor->Followir.Ir_Right_mm <= (minDistance + 3))
        {
            if (minDistance <= objectRightWallDis)
            {
                if (rotationAngle <= 18)
                {
                    cmd.vl = 0.06f;
                    cmd.va = 0.01f;
                }
                else
                {
                    cmd.vl = 0.05f;
                    cmd.va = 0.05f;
                }
            }
            else
            {
                cmd.vl = 0.05f;
                cmd.va = 0.00f;
            }
        }
        if (sqrt(pow(lastPositionX - sensor->Posture.pose.x, 2) + pow(lastPositionY - sensor->Posture.pose.y, 2)) > 0.05)
        {
            cmd.va = -0.6f;
            if (rotationAngle <= 11)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, " diameter<2");
                cmd.vl = -(2 * M_PI * (0.175 + 0.01 + 0.01)) / (2 * M_PI / cmd.va);
            }
            else if (rotationAngle > 11 && rotationAngle <= 16)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, " diameter=3");
                cmd.vl = -(2 * M_PI * (0.175 + 0.018 + 0.01)) / (2 * M_PI / cmd.va);
            }
            else if (rotationAngle > 16 && rotationAngle <= 18)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, " diameter=5");
                cmd.vl = -(2 * M_PI * (0.175 + 0.025 + 0.01)) / (2 * M_PI / cmd.va);
            }
            else if (rotationAngle > 18 && rotationAngle <= 25)
            {
                NAVLOG_EVERY_SEC(INFO, 1.0, " diameter=6");
                cmd.vl = -(2 * M_PI * (0.175 + 0.032 + 0.01)) / (2 * M_PI / cmd.va);
            }
            else
            {
                cmd.vl = 0.0f;
                cmd.va = 0.0f;
                AlongWallMode = MODE_ALONG_WALL;
                AlongWallStep = STEP_IDLE;
            }
            NAVLOG_EVERY_SEC(INFO, 1.0, " cmd.vl=" << cmd.vl << " , cmd.va=" << cmd.va);
        }
        if (sensor->Bumper.Bumper_Left || sensor->Bumper.Bumper_Right)
        {
            if (minDistance <= 22)
            {
                distanceTooClose = true;
            }
            cmd.vl = 0.0f;
            cmd.va = 0.0f;
            // minDistance = 100;
            AlongWallMode = MODE_ALONG_OBJECT;
            AlongWallStep = STEP_IDLE;
        }
    }
    break;
        case  STEP_RIGHT_ROTATUON_ANGLE:
      {
         NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_RIGHT_ROTATUON_ANGLE");
         if((std::fabs(bumperStartAngle - sensor->Posture.euler.yaw) * (180 / M_PI))>80.0){
            cmd.vl = 0.05f;
            cmd.va = 0.0f;
            AlongWallStep = STEP_IDLE;
         }
      }
        break;

    default:
        NAVLOG(INFO) << "Dock default step: ";
        break;
    }
    NAVLOG(INFO) << "right AlongWallStep: " << AlongWallStep;
    return AlongWallStep;
}

void IrAlongWall::LeftStraightWall(const SensorInfo *sensor, SpeedControlInfo &cmd)
{
    // Left  Go Straight Along Wall
    /*if(sensor->Followir.Ir_Left_mm - objectLeftWallDis >= 30){
        AlongWallStep = STEP_LEFTARCWALL;
        NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_LEFTSTRAIGHTWALL go to along wall   STEP_LEFTARCWALL");
    }else*/
    if (sensor->Followir.Ir_Left_mm - objectLeftWallDis >= 20 /* && sensor->Followir.Ir_Left_mm - objectLeftWallDis < 30*/)
    {
        cmd.vl = 0.1f;
        cmd.va = 0.5f;
        NAVLOG_EVERY_SEC(INFO, 1.0, "LeftStraightWall < 10 - 30");
    }
    else if (sensor->Followir.Ir_Left_mm - objectLeftWallDis < -10)
    {
        cmd.vl = 0.1f;
        cmd.va = -0.5f;
        NAVLOG_EVERY_SEC(INFO, 1.0, "LeftStraightWall < -10");
    }
    else
    {
        err = sensor->Followir.Ir_Left_mm - objectLeftWallDis;
        integral += err;
        voltage = Kp * err + Kd * (err - err_last); //+Ki*integral+Kd*(err-err_last);
        err_last = err;
        NAVLOG_EVERY_SEC(INFO, 1.0, " STEP_voltage" << voltage);
        cmd.vl = 0.1f;
        if (fabs(voltage) > 1.0)
        {
            voltage = (voltage > 0) ? 1.0 : -1.0;
        }
        cmd.va = (float)voltage;
    }
}
void IrAlongWall::RightStraightWall(const SensorInfo *sensor, SpeedControlInfo &cmd)
{
    // Right Go Straight Along Wall
    /*if(sensor->Followir.Ir_Right_mm - objectRightWallDis >= 100){
        AlongWallStep = STEP_RIGHTARCWALL;
        NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_RIGHTSTRAIGHTWALL go to along wall  STEP_RIGHTARCWALL");
    }else*/
    if (sensor->Followir.Ir_Right_mm - objectRightWallDis >= 20 /* && sensor->Followir.Ir_Right_mm - objectRightWallDis < 100*/)
    {
        cmd.vl = 0.1f;
        cmd.va = -0.5f;
        NAVLOG_EVERY_SEC(INFO, 1.0, "RightStraightWall < 10 - 30");
    }
    else if (sensor->Followir.Ir_Right_mm - objectRightWallDis < -10)
    {
        cmd.vl = 0.1f;
        cmd.va = 0.5f;
        NAVLOG_EVERY_SEC(INFO, 1.0, "RightStraightWall < -10");
    }
    else
    {
        err = sensor->Followir.Ir_Right_mm - objectRightWallDis;
        integral += err;
        voltage = Kp * err + Kd * (err - err_last); //+Ki*integral+Kd*(err-err_last);
        err_last = err;
        NAVLOG_EVERY_SEC(INFO, 1.0, "STEP_ voltage" << voltage);
        cmd.vl = 0.1f;
        if (fabs(voltage) > 1.0)
        {
            voltage = (voltage > 0) ? 1.0 : -1.0;
        }
        cmd.va = (float)(-voltage);
    }
}
