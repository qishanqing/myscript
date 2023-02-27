//////////////////////////////////////////////////////////////////////
///  @file     test_bind.cpp
///  @brief    sector bind test
///  @author   sunhao
///  @date     2022.07.25
//////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "src/utils/define.h"
#include "src/utils/file.h"
#include "src/instance/individual.h"
#include "types.h"
#include "sample/utils/utils.h"

extern InputParam inputParam;

void TriangleEstimate()
{
    psl::CameraMoudleParam param;
    ConfigParam configParam;
    Individual individual;

    std::string configFile = inputParam.configPath + CONFIG_FILE_NAME;
    std::string cameraConfigFile = inputParam.imagePath + "/config.yaml";
    const float RADIAN_2_ANGLE = 180 / M_PI;

    GetCameraConfig(cameraConfigFile, param);
    Status error = LoadConfig(configFile, configParam);

    if (SUCCESS != error.code)
    {
        ERROR_PRINT(error.message);
        exit(0);
    }

    individual.InitCamera(param);

    std::cout << "angle: 10\ny,y angle,distance(cm),distance angle(cm)" << std::endl;

    for (int i = 200; i < HEIGHT; i += 3)
    {
        BoxInfo box;
        box.SetRect(cv::Rect(300, i, 40, 0));

        configParam.structure.cameraAngle = 0;
        individual.SetParam(configParam);
        psl::Location loc = individual.TriangleEstimate(box);

        configParam.structure.cameraAngle = 10 / RADIAN_2_ANGLE;
        individual.SetParam(configParam);
        psl::Location locAngle = individual.TriangleEstimate(box);

        double bottom = box.box.y + box.box.height;
        double bottomCorrect = bottom;

        const float cy = param._left_camera.at(RESOLUTION)._P[6];;
        const float angle = configParam.structure.cameraAngle;

        if ((angle - 0) > 0.001)
        {
            const float f = param._left_camera.at(RESOLUTION)._P[0];;
            bottomCorrect = f * tan(atan((bottom - cy) / f) - angle) + cy;
        }

        std::cout << std::fixed << std::setprecision(1) << int(i - cy)
                  << "," << (int)(bottomCorrect - cy)
                  << "," << loc.y * 100 << "," << locAngle.y * 100 << std::endl;
    }
}

TEST(FunctorTest, TriangleEstimate)
{
    ConfigParam param;
    LoadConfig(inputParam.configPath + CONFIG_FILE_NAME, param);
    TriangleEstimate();
}