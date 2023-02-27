//
// Created by gby on 23-01-31.
//

#include "Recovery.h"

bool Recovery::RecoveryInit(const RecoveryStyle& _recoveryStyle_)
{
     _NavMap = s_map.NavMap;
    _ForbidenMap = s_map.ForbidenMap;

    switch (_recoveryStyle_)
    {
    case RecoveryStyle::Recovery_Forbiden:
        break;

    case RecoveryStyle::Recovery_Obs:
        _StepObs = StepObs::StepObs_INIT;
        _StepObsClear = EStepObsClear::EStepObsClear_DEFAULT;
        _RobotPoseLattice = PoseStamped();
        _ClearNum = 4;
        _SearchPoint.clear();
        _StepObsAction = ESteapObsAction::ESteapObsAction_DEFAULT;
        _DiffYaw = 0.0;
        _targetActionPoint = cv::Point(0, 0);
        break;
    case RecoveryStyle::Recovery_Surrounded:
        _RobotPoseSurround = PoseStamped();
        _StepSurround = StepSurround::StepSurround_INIT;
        break;
    case RecoveryStyle::Recovery_Physics:
        break;
    
    default:
        LOGMOTION_RECOVERY(INFO) << "脱困 " << (int)_recoveryStyle_ << " 模式 初始化错误";
        return false;
        break;
    }

    return true;
}

/********************RecoveryForbidenRun********************/
NodeStatus Recovery::RecoveryForbidenRun(DataDown *_dataDown_)
{
    if(_ForbidenMap.at<uint8_t>(_RobotPoseLattice.point) != CellType::ForbiddenUnknowCell)
    {
        LOGMOTION_RECOVERY(INFO) << "控制层判断机器进入虚拟墙或虚拟禁区";
    }
    else
    {
        LOGMOTION_RECOVERY(INFO) << "控制层判断机器未处于虚拟墙或虚拟禁区, 但此时信赖规划层, 认为机器处于虚拟墙或虚拟禁区中";
    }
    _dataDown_->cmd.vl = 0.0;
    _dataDown_->cmd.va = 0.0;
    return NodeStatus::FAILURE;
}
/********************RecoveryForbidenRun********************/


/********************RecoveryObs********************/
void Recovery::StepObsClear(int _num_, DataDown *_dataDown_)
{
    switch (_StepObsClear)
    {
    case EStepObsClear::EStepObsClear_DEFAULT:
        // _StepObsClear = EStepObsClear::EStepObsClear_INIT;
        _StepObsClear = EStepObsClear::EStepObsClear_CLEAR;
        break;
    // case EStepObsClear::EStepObsClear_INIT:
    //     BasicAction::RunTurnInit(2 * Pi_, _RobotPoseLattice.euler.yaw);
    //     _StepObsClear = EStepObsClear::EStepObsClear_TURN;
    //     break;
    // case EStepObsClear::EStepObsClear_TURN:
    //     if(BasicAction::RunTurnLeft(_RobotPoseLattice.euler.yaw, _dataDown_->cmd))
    //     {
    //         _StepObsClear = EStepObsClear::EStepObsClear_CLEAR;
    //     }
    //     break;
    case EStepObsClear::EStepObsClear_CLEAR:
        _StepObsClear = EStepObsClear::EStepObsClear_DEFAULT;
        _NavMap = s_map.NavMap;

        switch (_num_)
        {
        case 0:
            _NavMap.at<uint8_t>(_RobotPoseLattice.point) = CellType::NavFreeCell;
            break;
        
        case 1:
            for(int i = _RobotPoseLattice.point.x - 1; i <= _RobotPoseLattice.point.x + 1; i++)
                for(int j = _RobotPoseLattice.point.y - 1; j <= _RobotPoseLattice.point.y + 1; j++)
                    _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            break;

        case 2:
            for(int i = _RobotPoseLattice.point.x - 2; i <= _RobotPoseLattice.point.x + 2; i++)
            {
                for(int j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
                {
                    if((i != _RobotPoseLattice.point.x - 2 && j != _RobotPoseLattice.point.y - 2) && (i != _RobotPoseLattice.point.x - 2 && j != _RobotPoseLattice.point.y + 2) && 
                        (i != _RobotPoseLattice.point.x + 2 && j != _RobotPoseLattice.point.y - 2) && (i != _RobotPoseLattice.point.x + 2 && j != _RobotPoseLattice.point.y + 2))
                    {
                        _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
                    }
                }
            }
            break;
        
        case 3:
            for(int i = _RobotPoseLattice.point.x - 3; i <= _RobotPoseLattice.point.x + 3; i++)
            {
                for(int j = _RobotPoseLattice.point.y - 3; j <= _RobotPoseLattice.point.y + 3; j++)
                {
                    if((i != _RobotPoseLattice.point.x - 3 && j != _RobotPoseLattice.point.y - 3) && (i != _RobotPoseLattice.point.x - 3 && j != _RobotPoseLattice.point.y + 3) && 
                        (i != _RobotPoseLattice.point.x + 3 && j != _RobotPoseLattice.point.y - 3) && (i != _RobotPoseLattice.point.x + 3 && j != _RobotPoseLattice.point.y + 3))
                    {
                        _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
                    }
                }
            }
            break;

        case 4:
            for(int i = _RobotPoseLattice.point.x - 3; i <= _RobotPoseLattice.point.x + 3; i++)
                for(int j = _RobotPoseLattice.point.y - 3; j <= _RobotPoseLattice.point.y + 3; j++)
                        _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y - 4; i <= _RobotPoseLattice.point.x + 2; i++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y + 4; i <= _RobotPoseLattice.point.x + 2; i++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x + 4, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 4, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            break;

        case 5:
            for(int i = _RobotPoseLattice.point.x - 4; i <= _RobotPoseLattice.point.x + 4; i++)
                for(int j = _RobotPoseLattice.point.y - 4; j <= _RobotPoseLattice.point.y + 4; j++)
                        _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y - 5; i <= _RobotPoseLattice.point.x + 2; i++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y + 5; i <= _RobotPoseLattice.point.x + 2; i++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x + 5, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 5, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            break;

        case 6:
            for(int i = _RobotPoseLattice.point.x - 4; i <= _RobotPoseLattice.point.x + 4; i++)
                for(int j = _RobotPoseLattice.point.y - 5; j <= _RobotPoseLattice.point.y + 5; j++)
                        _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y - 6; i <= _RobotPoseLattice.point.x + 2; i++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y + 6; i <= _RobotPoseLattice.point.x + 2; i++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x + 6, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 6, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y - 4) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y - 3) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y + 4) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y + 3) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y - 4) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y - 3) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y + 4) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y + 3) = CellType::NavFreeCell;
            break;
        
        case 7:
            for(int i = _RobotPoseLattice.point.x - 5; i <= _RobotPoseLattice.point.x + 5; i++)
                for(int j = _RobotPoseLattice.point.y - 5; j <= _RobotPoseLattice.point.y + 5; j++)
                        _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 3, j = _RobotPoseLattice.point.y - 7; i <= _RobotPoseLattice.point.x + 3; i++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 3, j = _RobotPoseLattice.point.y + 7; i <= _RobotPoseLattice.point.x + 3; i++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x + 7, j = _RobotPoseLattice.point.y - 3; j <= _RobotPoseLattice.point.y + 3; j++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            for(int i = _RobotPoseLattice.point.x - 7, j = _RobotPoseLattice.point.y - 3; j <= _RobotPoseLattice.point.y + 3; j++)
                _NavMap.at<uint8_t>(i, j) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y - 6) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y - 6) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y - 6) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y - 6) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y - 4) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y - 3) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y - 4) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y - 3) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y + 6) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y + 6) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y + 6) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y + 6) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y + 4) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y + 3) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y + 4) = CellType::NavFreeCell;
            _NavMap.at<uint8_t>(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y + 3) = CellType::NavFreeCell;
            break;
        
        default:
            LOGMOTION_RECOVERY(INFO) << "_num_ is: " << _num_ << " , WRONG";
            break;
        }
        
        _StepObs = StepObs::StepObs_SEARCH;
        break;
    
    default:
        LOGMOTION_RECOVERY(INFO) << "_StepObsClear is: " << _StepObsClear << " , WRONG";
        break;
    }
}

bool Recovery::StepObsSearch(int _num_)
{
    switch (_num_)
    {
    case 0:
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 1, _RobotPoseLattice.point.y - 1));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x, _RobotPoseLattice.point.y - 1));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 1, _RobotPoseLattice.point.y - 1));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 1, _RobotPoseLattice.point.y));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 1, _RobotPoseLattice.point.y + 1));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x, _RobotPoseLattice.point.y + 1));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 1, _RobotPoseLattice.point.y + 1));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 1, _RobotPoseLattice.point.y));
        break;

    case 1:
        for(int i = _RobotPoseLattice.point.x - 1, j = _RobotPoseLattice.point.y - 2; i <= _RobotPoseLattice.point.x + 1; i++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        for(int i = _RobotPoseLattice.point.x + 2, j = _RobotPoseLattice.point.y - 1; j <= _RobotPoseLattice.point.y + 1; j++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        for(int i = _RobotPoseLattice.point.x + 1, j = _RobotPoseLattice.point.y + 2; i >= _RobotPoseLattice.point.x - 1; i--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y + 1; j >= _RobotPoseLattice.point.y - 1; j--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        break;

    case 2:
        for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y - 3; i <= _RobotPoseLattice.point.x + 2; i++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 2, _RobotPoseLattice.point.y - 2));
        for(int i = _RobotPoseLattice.point.x + 3, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 2, _RobotPoseLattice.point.y + 2));
        for(int i = _RobotPoseLattice.point.x + 2, j = _RobotPoseLattice.point.y + 3; i >= _RobotPoseLattice.point.x - 2; i--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 2, _RobotPoseLattice.point.y + 2));
        for(int i = _RobotPoseLattice.point.x - 3, j = _RobotPoseLattice.point.y + 2; j >= _RobotPoseLattice.point.y - 2; j--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 2, _RobotPoseLattice.point.y - 2));
        break;

    case 3:
        for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y - 4; i <= _RobotPoseLattice.point.x + 2; i++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y - 3));
        for(int i = _RobotPoseLattice.point.x + 4, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y + 3));
        for(int i = _RobotPoseLattice.point.x + 2, j = _RobotPoseLattice.point.y + 4; i >= _RobotPoseLattice.point.x - 2; i--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y + 3));
        for(int i = _RobotPoseLattice.point.x - 4, j = _RobotPoseLattice.point.y + 2; j >= _RobotPoseLattice.point.y - 2; j--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y - 3));
        break;

    case 4:
        for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y - 5; i <= _RobotPoseLattice.point.x + 2; i++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y - 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y - 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y - 3));
        for(int i = _RobotPoseLattice.point.x + 5, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y + 3));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y + 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y + 4));
        for(int i = _RobotPoseLattice.point.x + 2, j = _RobotPoseLattice.point.y + 5; i >= _RobotPoseLattice.point.x - 2; i--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y + 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y + 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y + 3));
        for(int i = _RobotPoseLattice.point.x - 5, j = _RobotPoseLattice.point.y + 2; j >= _RobotPoseLattice.point.y - 2; j--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y - 3));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y - 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y - 4));
        break;

    case 5:
        for(int i = _RobotPoseLattice.point.x - 2, j = _RobotPoseLattice.point.y - 6; i <= _RobotPoseLattice.point.x + 2; i++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y - 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y - 3));
        for(int i = _RobotPoseLattice.point.x + 6, j = _RobotPoseLattice.point.y - 2; j <= _RobotPoseLattice.point.y + 2; j++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y + 3));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y + 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y + 5));
        for(int i = _RobotPoseLattice.point.x + 2, j = _RobotPoseLattice.point.y + 6; i >= _RobotPoseLattice.point.x - 2; i--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y + 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y + 3));
        for(int i = _RobotPoseLattice.point.x - 6, j = _RobotPoseLattice.point.y + 2; j >= _RobotPoseLattice.point.y - 2; j--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y - 3));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y - 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y - 5));
        break;

    case 6:
        for(int i = _RobotPoseLattice.point.x - 3, j = _RobotPoseLattice.point.y - 7; i <= _RobotPoseLattice.point.x + 3; i++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y - 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y - 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y - 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y - 3));
        for(int i = _RobotPoseLattice.point.x + 7, j = _RobotPoseLattice.point.y - 3; j <= _RobotPoseLattice.point.y + 3; j++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y + 3));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y + 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y + 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 3, _RobotPoseLattice.point.y + 6));
        for(int i = _RobotPoseLattice.point.x + 3, j = _RobotPoseLattice.point.y + 7; i >= _RobotPoseLattice.point.x - 3; i--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y + 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y + 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y + 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y + 3));
        for(int i = _RobotPoseLattice.point.x - 7, j = _RobotPoseLattice.point.y + 3; j >= _RobotPoseLattice.point.y - 3; j--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y - 3));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y - 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y - 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 3, _RobotPoseLattice.point.y - 6));
        break;

    case 7:
        for(int i = _RobotPoseLattice.point.x - 3, j = _RobotPoseLattice.point.y - 8; i <= _RobotPoseLattice.point.x + 3; i++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y - 7));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y - 7));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y - 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y - 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 7, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 7, _RobotPoseLattice.point.y - 4));
        for(int i = _RobotPoseLattice.point.x + 8, j = _RobotPoseLattice.point.y - 3; j <= _RobotPoseLattice.point.y + 3; j++)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 7, _RobotPoseLattice.point.y + 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 7, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 6, _RobotPoseLattice.point.y + 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y + 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 5, _RobotPoseLattice.point.y + 7));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x + 4, _RobotPoseLattice.point.y + 7));
        for(int i = _RobotPoseLattice.point.x + 3, j = _RobotPoseLattice.point.y + 8; i >= _RobotPoseLattice.point.x - 3; i--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y + 7));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y + 7));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y + 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y + 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 7, _RobotPoseLattice.point.y + 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 7, _RobotPoseLattice.point.y + 4));
        for(int i = _RobotPoseLattice.point.x - 8, j = _RobotPoseLattice.point.y + 3; j >= _RobotPoseLattice.point.y - 3; j--)
            _SearchPoint.emplace_back(cv::Point(i, j));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 7, _RobotPoseLattice.point.y - 4));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 7, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y - 5));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 6, _RobotPoseLattice.point.y - 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y - 6));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 5, _RobotPoseLattice.point.y - 7));
        _SearchPoint.emplace_back(cv::Point(_RobotPoseLattice.point.x - 4, _RobotPoseLattice.point.y - 7));
        break;

    default:
        LOGMOTION_RECOVERY(INFO) << "StepObsSearch _num_ is: " << _num_ << " , WRONG";
        return false;
        break;
    }

    vector<cv::Point> _SearchPoint_bak;
    _SearchPoint_bak.clear();
    for(int i = 0; i < _SearchPoint.size(); i++)
    {
        LOGMOTION_RECOVERY(INFO) << "_SearchPoint[" << i << "] is(x,y): (" << _SearchPoint[i].x << ", " << _SearchPoint[i].y << ") valueNavMap = " << (int)_NavMap.at<uint8_t>(_SearchPoint[i]) << ", valueForbMap = " << (int)_ForbidenMap.at<uint8_t>(_SearchPoint[i]);
        if(_NavMap.at<uint8_t>(_SearchPoint[i]) == CellType::NavFreeCell && _ForbidenMap.at<uint8_t>(_SearchPoint[i]) == CellType::ForbiddenUnknowCell)
            _SearchPoint_bak.emplace_back(_SearchPoint[i]);
    }
    _SearchPoint.clear();
    _SearchPoint = _SearchPoint_bak;

    // 检查并且显示 _SearchPoint:
    if(_SearchPoint.empty())
    {
        LOGMOTION_RECOVERY(INFO) << "_num_ = " << _num_ << " 时外围点均不符合规定, _SearchPoint为空, 直接++进行下一次判断";
        _SearchPoint.clear();
        _StepObs = StepObs::StepObs_INIT;
        _ClearNum++;
        if(_ClearNum > 7)
        {
            LOGMOTION_RECOVERY(INFO) << "_num_ = " << _num_ << ", _ClearNum超过最大值7!!!";
            return false;
        }
    }
    else
    {
        for(int i = 0; i < _SearchPoint.size(); i++)
        {
            LOGMOTION_RECOVERY(INFO) << "_num_: " << _num_ << ", 其外围点 _SearchPoint[" << i << "] is(x, y): (" << _SearchPoint[i].x << ", " << _SearchPoint[i].y << ")";
        }
        _StepObs = StepObs::StepObs_ACTION;
    }

    return true;
}

double Recovery::ESteapObsAction_CalDiff(const SensorInfo *_sensor_, cv::Point _robotPoint_, cv::Point _targetPoint_)
{
    cv::Point2f point1 = s_map.MapToWorld(_robotPoint_);
    cv::Point2f point2 = s_map.MapToWorld(_targetPoint_);
    double a_p2p = atan2((double)(point2.y - point1.y) , (double)(point2.x - point1.x));
    double abssum = (fabs(a_p2p) + fabs(_sensor_->Posture.euler.yaw));
    int dir = _sensor_->Posture.euler.yaw >= 0 ? 1:-1;
    double CourseYaw = ((a_p2p > 0 && _sensor_->Posture.euler.yaw > 0) || (a_p2p < 0 && _sensor_->Posture.euler.yaw < 0) || (a_p2p == 0 && _sensor_->Posture.euler.yaw == 0)) ? (a_p2p - _sensor_->Posture.euler.yaw):(abssum > Pi_ ? dir*(2*Pi_ - abssum):(-1*dir*abssum));
    return CourseYaw;
}

void Recovery::ESteapObsAction_RunDrive(const SensorInfo *_sensor_, PoseStamped _curPose_, cv::Point _targetPoint_, DataDown *_dataDown_)
{
    cv::Point2f targetPose = s_map.MapToWorld(_targetPoint_);
    PoseStamped curPose = _curPose_;
    double distance = (curPose.pose.x - targetPose.x) * (curPose.pose.x - targetPose.x) + (curPose.pose.y - targetPose.y) * (curPose.pose.y - targetPose.y);

    if(SquareDis_p2p(curPose.pose, targetPose) < distance)
    {
        _dataDown_->cmd.va = 0.0;
        _dataDown_->cmd.vl = 0.05;
    }

    if(_sensor_->Posture.point.x != _targetPoint_.x && _sensor_->Posture.point.y != _targetPoint_.y)
    {
        LOGMOTION_RECOVERY(INFO) << "向 _SearchPoint 可通行点: (" << _targetPoint_.x << ", " << _targetPoint_.y << ") 移动中...";
    }

    if(_sensor_->Posture.point.x == _targetPoint_.x && _sensor_->Posture.point.y == _targetPoint_.y)
    {
        LOGMOTION_RECOVERY(INFO) << "移动到 _SearchPoint 可通行点: (" << _targetPoint_.x << ", " << _targetPoint_.y << ") 成功";
        _dataDown_->cmd.va = 0.0;
        _dataDown_->cmd.vl = 0.0;

        if(_ForbidenMap.at<uint8_t>(_sensor_->Posture.point) != CellType::ForbiddenUnknowCell)
        {
            LOGMOTION_RECOVERY(INFO) << "当前点依然处于困境";
            _StepObsAction = ESteapObsAction::ESteapObsAction_BACK;
        }
    }

    if(_sensor_->Bumper.BumperTriggered)
    {
        LOGMOTION_RECOVERY(INFO) << "碰撞传感器触发...";
        _StepObsAction = ESteapObsAction::ESteapObsAction_BUMPER;
    }
}

void Recovery::ESteapObsAction_Bumper(const SensorInfo *_sensor_, PoseStamped _targetPose_, DataDown *_dataDown_)
{
    if(_sensor_->Posture.point.x != _targetPose_.point.x && _sensor_->Posture.point.y != _targetPose_.point.y)
    {
        _dataDown_->cmd.va = 0.0;
        _dataDown_->cmd.vl = -0.05;
    }

    if(_sensor_->Posture.point.x == _targetPose_.point.x && _sensor_->Posture.point.y == _targetPose_.point.y)
    {
        LOGMOTION_RECOVERY(INFO) << "Bumper触发, 后退回到起始栅格点";
        _StepObsAction = ESteapObsAction::ESteapObsAction_DEFAULT;
    }
}

void Recovery::ESteapObsAction_Back(const SensorInfo *_sensor_, PoseStamped _targetPose_, DataDown *_dataDown_)
{
    if(_sensor_->Posture.point.x != _targetPose_.point.x && _sensor_->Posture.point.y != _targetPose_.point.y)
    {
        _dataDown_->cmd.va = 0.0;
        _dataDown_->cmd.vl = -0.05;
    }

    if(_sensor_->Posture.point.x == _targetPose_.point.x && _sensor_->Posture.point.y == _targetPose_.point.y)
    {
        LOGMOTION_RECOVERY(INFO) << "BACK 回到起始栅格点";
        _StepObsAction = ESteapObsAction::ESteapObsAction_DEFAULT;
    }
}

bool Recovery::StepObsAction(const SensorInfo *_sensor_, DataDown *_dataDown_)
{
    if(_SearchPoint.empty())
    {
        LOGMOTION_RECOVERY(INFO) << "in Recovery::StepObsAction(), _SearchPoint is empty!!!";
        return false;
    }

    switch (_StepObsAction)
    {
    case ESteapObsAction::ESteapObsAction_DEFAULT:
        if(_SearchPoint.empty())
        {
            _ClearNum++;
            _StepObs = StepObs::StepObs_INIT;
            return true;
        }
        else
        {
            _targetActionPoint = _SearchPoint.back();
            _SearchPoint.pop_back();
            _StepObsAction = ESteapObsAction::ESteapObsAction_CALDIFF;            
        }
        break;
    case ESteapObsAction::ESteapObsAction_CALDIFF:
        _DiffYaw = ESteapObsAction_CalDiff(_sensor_, _RobotPoseLattice.point, _targetActionPoint);
        _StepObsAction = ESteapObsAction::ESteapObsAction_TURNINIT;
        break;
    case ESteapObsAction::ESteapObsAction_TURNINIT:
        BasicAction::RunTurnInit(fabs(_DiffYaw), _sensor_->Posture.euler.yaw);
        _StepObsAction = ESteapObsAction::ESteapObsAction_TURN;
        break;
    case ESteapObsAction::ESteapObsAction_TURN:
        if (_DiffYaw >= 0)
        {
            if (BasicAction::RunTurnLeft(_sensor_->Posture.euler.yaw, _dataDown_->cmd, 0.2))
            {
                _StepObsAction = ESteapObsAction::ESteapObsAction_DRIVEINIT;
            }
        }
        else
        {
            if (BasicAction::RunTurnRight(_sensor_->Posture.euler.yaw, _dataDown_->cmd, -0.2))
            {
                _StepObsAction = ESteapObsAction::ESteapObsAction_DRIVEINIT;
            }
        }
        break;
    case ESteapObsAction::ESteapObsAction_DRIVEINIT:
        _StepObsAction = ESteapObsAction::ESteapObsAction_DRIVE;
        break;
    case ESteapObsAction::ESteapObsAction_DRIVE:
        ESteapObsAction_RunDrive(_sensor_, _RobotPoseLattice, _targetActionPoint, _dataDown_);
        break;
    case ESteapObsAction::ESteapObsAction_BUMPER:
        ESteapObsAction_Bumper(_sensor_, _RobotPoseLattice, _dataDown_);
        break;
    case ESteapObsAction::ESteapObsAction_BACK:
        ESteapObsAction_Back(_sensor_, _RobotPoseLattice, _dataDown_);
        break;

    default:
        LOGMOTION_RECOVERY(INFO) << "_StepObsAction  is: " << _StepObsAction << " , WRONG";
        return false;
        break;
    }

    return true;
}

NodeStatus Recovery::RecoveryObsRun(const SensorInfo *_sensor_, DataDown *_dataDown_)
{
    _RobotPoseLattice = _sensor_->Posture;
    if(_ForbidenMap.at<uint8_t>(_RobotPoseLattice.point) == CellType::ForbiddenUnknowCell)
    {
        LOGMOTION_RECOVERY(INFO) << "机器人当前位姿处于 ForbiddenUnknowCell , 脱困成功";
        return NodeStatus::SUCCESS;
    }

    if(_NavMap.at<uint8_t>(_RobotPoseLattice.point) != CellType::NavFreeCell)
    {
        LOGMOTION_RECOVERY(INFO) << "控制层判断机器进入障碍物膨胀层";
    }
    else
    {
        LOGMOTION_RECOVERY(INFO) << "控制层判断机器未处于障碍物膨胀层, 但此时信赖规划层, 认为机器处于障碍物膨胀层";
    }

    if(_ClearNum > 7)
        return NodeStatus::FAILURE;

    switch (_StepObs)
    {
    case StepObs::StepObs_INIT:
        _StepObs = StepObs::StepObs_CLEAR;
        break;
    case StepObs::StepObs_CLEAR:
        StepObsClear(_ClearNum, _dataDown_);
        break;
    case StepObs::StepObs_SEARCH:
        if(!StepObsSearch(_ClearNum)) return NodeStatus::FAILURE;
        break;
    case StepObs::StepObs_ACTION:
        if(!StepObsAction(_sensor_, _dataDown_)) return NodeStatus::FAILURE;
        break;

    default:
        LOGMOTION_RECOVERY(INFO) << "_StepObs is: " << _StepObs << " , WRONG";
        return NodeStatus::FAILURE;
        break;
    }
    return NodeStatus::RUNNING;
}
/********************RecoveryObs********************/


NodeStatus Recovery::RecoverySurroundedRun(const SensorInfo *_sensor_, DataDown *_dataDown_)
{
    _RobotPoseSurround = _sensor_->Posture;
    if(_ForbidenMap.at<uint8_t>(_RobotPoseSurround.point) == CellType::ForbiddenUnknowCell)
    {
        LOGMOTION_RECOVERY(INFO) << "机器人当前位姿处于 ForbiddenUnknowCell , 脱困成功";
        return NodeStatus::SUCCESS;
    }

    if(_NavMap.at<uint8_t>(_RobotPoseSurround.point) != CellType::NavFreeCell)
    {
        LOGMOTION_RECOVERY(INFO) << "控制层判断机器进入障碍物包络区域";
    }
    else
    {
        LOGMOTION_RECOVERY(INFO) << "控制层判断机器未处于障碍物包络区域, 但此时信赖规划层, 认为机器处于障碍物包络区域";
    }

    switch (_StepSurround)
    {
    case StepSurround::StepSurround_INIT:
        _StepSurround = StepSurround::StepSurround_DRIVE;
        break;
    case StepSurround::StepSurround_DRIVE:
        if(_sensor_->Bumper.BumperTriggered)
        {
            LOGMOTION_RECOVERY(INFO) << "碰撞传感器触发......";
            _StepSurround = StepSurround::StepSurround_BUMPER;
        }
        else
        {
            _dataDown_->cmd.vl = 0.05;
            _dataDown_->cmd.va = 0.0;
        }
        break;
    case StepSurround::StepSurround_BUMPER:
        if(_sensor_->Bumper.BumperTriggered)
        {
            LOGMOTION_RECOVERY(INFO) << "bumper触发, backing......";
            _dataDown_->cmd.vl = -0.05;
            _dataDown_->cmd.va = 0.0;
        }
        else
        {
            LOGMOTION_RECOVERY(INFO) << "障碍物包络区域脱困失败......";
            return NodeStatus::FAILURE;
        }
        break;
    
    default:
        break;
    }
    return NodeStatus::RUNNING;
}

NodeStatus Recovery::RecoveryPhysicsRun(DataDown *_dataDown_)
{
    LOGMOTION_RECOVERY(INFO) << "机器人被物理围困";
    return NodeStatus::FAILURE;
}