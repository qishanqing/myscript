//////////////////////////////////////////////////////////////////////
///  @file     test_bind.cpp
///  @brief    sector bind test
///  @author   sunhao
///  @date     2021.12.05
//////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "src/utils/gemotry.h"
#include "src/utils/escalator_region.h"
#include "src/utils/detector_param.h"
#include "src/utils/define.h"
#include "src/utils/image_process.h"
#include "src/utils/config.h"
#include "types.h"
#include "src/map/escalator_map.h"
#include "sample/utils/utils.h"
#include "src/utils/file.h"
#include "src/utils/utils.h"

extern InputParam inputParam;

//#define WRITE_IMAGE

const float MIN_MACHINE_MOTION_DISTANCE = 0.3; // 20 cm
const std::string SECTOR_POINT = "Sector(Point";
const std::string VERTEX_POINT = "vertex Point(";
const std::string POINTS = ") Point(";
const std::string RESULT = "result";
const std::string REGION = "region";

const cv::Scalar COLOR_BACK(80, 80, 80);
const cv::Scalar COLOR_TEXT(150, 150, 150);
const cv::Scalar COLOR_AXIS(0, 0, 200);
const cv::Scalar COLOR_VIEW(255, 50, 50);
const cv::Scalar COLOR_OBJECT(50, 150, 50);
const cv::Scalar COLOR_LAST_OBJECT(0, 250, 250);
const cv::Scalar COLOR_FORGROUND(0, 140, 255);
const cv::Scalar RED(0, 0, 255);
const cv::Scalar GREEN(0, 255, 0);
const cv::Scalar BLUE(255, 50, 50);
const std::string RESULT_DIR = "bind_result";

void Show(const std::vector<Sector> &regions, const BoxInfo &view
          , const std::vector<BoxInfo> &boxes)
{
    cv::Mat image = cv::Mat::zeros(escalator_map::W, escalator_map::H, CV_8UC3);
    EscalatorMap::DrawObject(view, boxes, regions, image);
    cv::imshow("bind", image);
    cv::waitKey(0);
}

void GetSectorPoint(const std::string &strLog, psl::Location &p1
                    , psl::Location &p2, psl::Location &p3)
{
    sscanf(strLog.c_str()
           , "Sector(Point(%lf, %lf, %lf), Point(%lf, %lf, %lf), Point(%lf, %lf, %lf))"
           , &p1.x, &p1.y, &p1.z, &p2.x, &p2.y, &p2.z, &p3.x, &p3.y, &p3.z);
    return;
}

void GetVertexPoint(const std::string &strLog, psl::Location &point)
{
    sscanf(strLog.c_str(), "vertex Point(%lf, %lf, %lf)", &point.x, &point.y, &point.z);
}

void GetPoint(const std::string &strLog, psl::Location &p1, psl::Location &p2)
{
    sscanf(strLog.c_str(), "Point(%lf, %lf, %lf) Point(%lf, %lf, %lf)", &p1.x, &p1.y
           , &p1.z, &p2.x, &p2.y, &p2.z);
}

void GetRegion(const std::string &strLog, int &regionIndex, int &boxIndex)
{
    sscanf(strLog.c_str(), "region: %d box: %d valid: 0"
           , &regionIndex, &boxIndex);
}

bool GetEnd(std::fstream &File, std::string &strLog)
{
    std::getline(File, strLog);
    if (File.eof())
    {
        File.close();
        std::cout << "end line\n";
        return true;
    }
    return false;
}

void WriteFile(std::fstream &File, const std::string &strLog)
{
    File.write(strLog.c_str(), strLog.size());
    File.write("\n", 1);
}

void TestBindByLog(std::string logFile, std::vector<vector<BindResult>> &indexLabels
                   , std::vector<vector<BindResult>> &indexes
                   , std::vector<std::string> &indexNames)
{
    const int ESCALATOR = 1;
    int labelIndex = 0;
    std::string strLog;
    std::fstream fileRead;//, fileWrite;

    if (not file_op::File::Exist(logFile))
    {
        std::cout << "file: " << logFile << " not exist" << std::endl;
        return;
    }

    std::string logFileIndex = logFile.substr(0, logFile.rfind("."))
                               + std::to_string(GetTimestamp()) + ".txt";

    try
    {
        fileRead.open(logFile, std::ios::in);
    }
    catch (...)
    {
        std::cout << "open fstream failed\n";
        return;
    }

    std::getline(fileRead, strLog);

    while (!fileRead.eof())
    {
        std::vector<Sector> testCase;
        BoxInfo view;
        std::vector<BoxInfo> boxes;
        EscalatorRegion escalatorRegion;
        vector<BindResult> indexLabel;
        vector<BindResult> index;

        while (not std::strstr(strLog.c_str(), RESULT.c_str()))
        {
            if (GetEnd(fileRead, strLog))
            {
                return;
            }
        }

        while (std::strstr(strLog.c_str(), RESULT.c_str()))
        {
            sscanf(strLog.c_str(), "line%d write image to: result", &labelIndex);

            if (GetEnd(fileRead, strLog))
            {
                return;
            }
        }

        if (std::strstr(strLog.c_str(), SECTOR_POINT.c_str()))
        {
            while (std::strstr(strLog.c_str(), SECTOR_POINT.c_str()))
            {
                psl::Location p1, p2, p3;
                GetSectorPoint(strLog, p1, p2, p3);
                testCase.push_back(Sector(p1, p2, p3));

                if (GetEnd(fileRead, strLog))
                {
                    return;
                }
            }
        }

        if (std::strstr(strLog.c_str(), VERTEX_POINT.c_str()))
        {
            GetVertexPoint(strLog, view.location.world);
            if (GetEnd(fileRead, strLog))
            {
                return;
            }
        }

        if (std::strstr(strLog.c_str(), POINTS.c_str()))
        {
            while (std::strstr(strLog.c_str(), POINTS.c_str()))
            {
                BoxInfo box;
                GetPoint(strLog, box.edgeLeft.world, box.edgeRight.world);

                box.classID = ESCALATOR;
                boxes.push_back(box);

                if (GetEnd(fileRead, strLog))
                {
                    return;
                }
            }
        }

        indexLabel.resize(testCase.size());

        if (std::strstr(strLog.c_str(), REGION.c_str()))
        {
            while (std::strstr(strLog.c_str(), REGION.c_str()))
            {
                int regionIndex, boxLabel;
                GetRegion(strLog, regionIndex, boxLabel);
                indexLabel.at(regionIndex).id = boxLabel;
                if (GetEnd(fileRead, strLog))
                {
                    return;
                }
            }
        }

        // prepare region
        for (const auto r : testCase)
        {
            escalatorRegion.regions.push_back(r);
            escalatorRegion.regions.back().id = escalatorRegion.regions.size() - 1;
        }

        escalatorRegion.Bind(boxes, view, false, index);
//        escalatorRegion.Update(boxes, view);

#ifdef WRITE_IMAGE
        // write image
        cv::Mat image = cv::Mat::zeros(escalator_map::W, escalator_map::H, CV_8UC3);
        EscalatorMap::DrawObject(view, boxes, escalatorRegion.regions, image);

        for (int i = 0; i < indexLabel.size(); ++i)
        {
            std::stringstream label;
            const auto &l = indexLabel.at(i);

            label << "region: " << i << " box: " << l.id << " valid: " << l.valid;
            cv::putText(image, label.str(), cv::Point(50, 30 * (i + 1)), cv::FONT_HERSHEY_PLAIN
                        , 1.5, COLOR_FORGROUND, 2);

            label.str("");
            const auto &p = index.at(i);
            label << "region: " << i << " box: " << p.id << " valid: " << p.valid;
            cv::putText(image, label.str(), cv::Point(370, 30 * (i + 1)), cv::FONT_HERSHEY_PLAIN
                        , 1.5, BLUE, 2);

        }

        std::string file = RESULT_DIR + "/" + std::to_string(labelIndex) + ".png";
        file_op::File::MkdirFromFile(file);
        cv::imwrite(file, image);
        LOG_CHECK_DEBUG(INFO) << "write image to: " << file;
#endif
        if (indexLabel.size() != index.size())
        {
            std::cout << "wrong index.size()" << std::endl;
        }
        else
        {
            indexLabels.push_back(indexLabel);
            indexes.push_back(index);
            indexNames.push_back("line" + std::to_string(labelIndex));
        }
    }
    return;
}

void TestBind()
{
    const int ESCALATOR = 1;
    EscalatorRegion escalatorRegion;
    std::vector<Sector> testCase{
        Sector(Point(-4.612, 7.3499, 0), Point(-6.17174, 7.86634, 0), Point(-4.219, 7.67564, 0))
        , Sector(Point(-2.55979, 7.36578, 0), Point(-1.99269, 8.26371, 0), Point(3.88749, 11.4524, 0))
    };

    // prepare view
    BoxInfo view;
    view.location.world = Point(-5.24764, 3.94574, 0);

    // prepare box
    std::vector<BoxInfo> boxes;
    BoxInfo box;

    box.edgeLeft.world = Point(-7.81261, 13.7307, 0);
    box.edgeRight.world = Point(-2.91018, 13.8093, 0);

    box.classID = ESCALATOR;
    box.locationSerials.push_back(Location());
//    box.locationSerials.push_back(Location());
    boxes.push_back(box);
    box.edgeLeft.world = Point(0.47063, 12.3093, 0) ;
    box.edgeRight.world = Point(1.87957, 11.1336, 0);
    boxes.push_back(box);

    // prepare region
    for (const auto r : testCase)
    {
        escalatorRegion.regions.push_back(r);
        escalatorRegion.regions.back().id = escalatorRegion.regions.size() - 1;
        escalatorRegion.regions.back().partCount = 2;
    }

    vector<BindResult> index;

    escalatorRegion.Update(boxes, view);
    Show(escalatorRegion.regions, view, boxes);
    escalatorRegion.Update(boxes, view);
    Show(escalatorRegion.regions, view, boxes);
}

void GetPresent(const std::vector<vector<BindResult>> &indexLabels
                , const std::vector<vector<BindResult>> &indexes
                , std::vector<std::string> indexNames, const std::string logFile)
{
    std::string wrongIndex = logFile.substr(0, logFile.rfind("."))
                             + "_wrong.txt";
    std::fstream fileWrite;
    try
    {
        fileWrite.open(wrongIndex, std::ios::out);
    }
    catch (...)
    {
        std::cout << "open fstream failed\n";
        return;
    }
    if (indexLabels.size() != indexes.size() or indexNames.size() != indexes.size())
    {
        std::cout << "wrong result and labels, stop" << std::endl;
        return;
    }

    if (indexLabels.size() == 0)
    {
        std::cout << "wrong result and labels, stop" << std::endl;
        return;
    }

    int rightLabels = 0;
    int allLabel = 0;
    for (int i = 0; i < indexes.size(); ++i)
    {
        if (indexNames.at(i) == "line64121")
            int k = 0;
        int rightLabel = 0;
        auto label = indexLabels.at(i);
        auto predict = indexes.at(i);

        if (indexLabels.at(i).size() != indexes.at(i).size())
        {
            std::cout << "wrong result and labels, continue" << std::endl;
            continue;
        }

        for (int j = 0; j < indexes.at(i).size(); ++j)
        {
            allLabel++;

            if (label.at(j).id == predict.at(j).id)
            {
                rightLabel++;
            }
            else
            {
                WriteFile(fileWrite, indexNames.at(i) + " " +
                                     std::to_string(indexes.at(i).at(j).valid));
            }
        }
        rightLabels += rightLabel;
    }
    std::cout << "right binds: " << rightLabels << std::endl;
    std::cout << "all binds: " << allLabel << std::endl;
    std::cout << "right binds present: " << rightLabels / float(allLabel) << std::endl;
    std::cout << "wrongIndex has be stored in : " << wrongIndex << std::endl;

}

void TestBind(std::string logFile)
{
    std::vector<vector<BindResult>> indexLabels, indexes;
    std::vector<std::string> indexNames;

    char *p = getcwd(NULL, 0);
    file_op::File::RemoveDir(std::string(p) + "/" + RESULT_DIR);

    TestBindByLog(logFile, indexLabels, indexes, indexNames);
    GetPresent(indexLabels, indexes, indexNames, logFile);
}

TEST(FunctorTest, Bind)
{
    ConfigParam param;
    LoadConfig(inputParam.configPath + CONFIG_FILE_NAME, param);
    TestBind();
}

TEST(FunctorTest, BindByFile)
{
    ConfigParam param;
    LoadConfig(inputParam.configPath + CONFIG_FILE_NAME, param);
    std::string logFile = "bind.txt";
    TestBind(logFile);
}
