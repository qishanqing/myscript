//
// Created by lcy on 22-11-29.
//

#include "Relocation.h"

bool Relocation::RelocationInit(float dis){
    reLocation_step_cur = 1;
    Relocation_go = dis;
    LOGMOTION(INFO) << "reLocationInit";
    return true;
}
int Relocation::RelocationRun(SensorInfo * sensor , SpeedControlInfo & cmd){
    if(flg_reverse){
        int flg = reverserun(sensor->Posture , cmd);
        if(flg == 3){
            if(reLocation_step_cur == 6 ||
				reLocation_step_cur == 12 ||
				reLocation_step_cur == 18){
                reLocation_step_cur ++;
                flg_reverse = false;
            }
        }
		return 0;
    }else{
		if(reLocation_step_cur == 6||
			reLocation_step_cur == 12 ||
			reLocation_step_cur == 18){
			if(sensor->Bumper.BumperTriggered){
				flg_reverse = true;
				reverse_step_cur = 1;
				return 0;
			}
		}
    }
    switch (reLocation_step_cur){
        case 1:
            RunTurnInit((float)Pi_*2 , sensor->Posture.euler.yaw);//旋转一周
            reLocation_step_cur = 2;
            break;
        case 2:
            if(RunTurnLeft(sensor->Posture.euler.yaw , cmd)){
                reLocation_step_cur = 3;
            }
            break;
        case 3:
            RunTurnInit((float)Pi_*5/6 , sensor->Posture.euler.yaw);//旋转120°
            reLocation_step_cur = 4;
            break;
        case 4:
            if(RunTurnLeft(sensor->Posture.euler.yaw , cmd)){
                reLocation_step_cur = 5;
            }
            break;
        case 5:
            RunDriveInit(Relocation_go , sensor->Posture);
            reLocation_step_cur = 6;//直行１m
            break;
        case 6:
            if(RunDrive(sensor->Posture , cmd)){
                reLocation_step_cur = 7;
            }
            break;
		case 7:
            RunTurnInit((float)Pi_*2 , sensor->Posture.euler.yaw);//旋转一周
            reLocation_step_cur = 8;
            break;
        case 8:
            if(RunTurnLeft(sensor->Posture.euler.yaw , cmd)){
                reLocation_step_cur = 9;
            }
            break;
        case 9:
            RunTurnInit(Relocation_turn , sensor->Posture.euler.yaw);//旋转120°
            reLocation_step_cur = 10;
            break;
        case 10:
            if(RunTurnLeft(sensor->Posture.euler.yaw , cmd)){
                reLocation_step_cur = 11;
            }
            break;
        case 11:
            RunDriveInit(Relocation_go , sensor->Posture);
            reLocation_step_cur = 12;//直行１m
            break;
        case 12:
            if(RunDrive(sensor->Posture , cmd)){
                reLocation_step_cur = 13;
            }
            break;
		case 13:
            RunTurnInit((float)Pi_*2 , sensor->Posture.euler.yaw);//旋转一周
            reLocation_step_cur = 14;
            break;
        case 14:
            if(RunTurnLeft(sensor->Posture.euler.yaw , cmd)){
                reLocation_step_cur = 15;
            }
            break;
        case 15:
            RunTurnInit(Relocation_turn , sensor->Posture.euler.yaw);//旋转120°
            reLocation_step_cur = 16;
            break;
        case 16:
            if(RunTurnLeft(sensor->Posture.euler.yaw , cmd)){
                reLocation_step_cur = 17;
            }
            break;
        case 17:
            RunDriveInit(Relocation_go , sensor->Posture);
            reLocation_step_cur = 18;//直行１m
            break;
        case 18:
            if(RunDrive(sensor->Posture , cmd)){
                reLocation_step_cur = 19;
            }
            break;
        default:
            RunParking(cmd);
            break;
    }
    return reLocation_step_cur;
}

int Relocation::reverserun(const PoseStamped & p , SpeedControlInfo & cmd){
    switch (reverse_step_cur){
        case 1:
            RunReverseInit(0.02 , p);
            reverse_step_cur = 2;
            break;
        case 2:
            if(RunReverse(p , cmd)){
                reverse_step_cur = 3;
            }
            break;
        default:
            RunParking(cmd);
            break;
    }
	return reverse_step_cur;
}

void Relocation::FirstBuildInit(double yaw){
    RunTurnInit((float)Pi_ * 2 , yaw);
}
bool Relocation::FirstBuildRun(double yaw , SpeedControlInfo & cmd){
    return RunTurnLeft(yaw , cmd);
}