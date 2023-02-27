#include "manager.h"
#include "src/utils/file.h"

MapManager::MapManager()
{

}

MapManager::~MapManager()
{

}

void MapManager::SetParam(const psl::DetectorParam &detectParam)
{
    staticMap.SetParam(detectParam);
    escalatorMap.SetParam(detectParam);
    aircraftHatchMap.SetParam(detectParam);
}

void MapManager::Update(std::vector<BoxInfo> &boxes
                        , const BoxInfo &view, const psl::Time &time)
{
    staticMap.Update(boxes, view, time);
    escalatorMap.Update(boxes, view, time);
    aircraftHatchMap.Update(boxes, view, time);
}

bool MapManager::Save(const string &file)
{
    // TODO : 2022-08-03 11:31:33 [hao]  save in same file, not cover
    file_op::File::MkdirFromFile(file);
    ofstream.open(file);

    if (!ofstream.is_open())
    {
        ofstream.close(); // TODO : it need close?
//        LOG_CHECK_STATUS(WARNING) << "fail to create file <" << file << ">.";
        return false;
    }

    bool ret = escalatorMap.Save(ofstream);
    ret &= staticMap.Save(ofstream);
    ofstream.close();

    return ret;
}

void MapManager::Modify(psl::SenmaticMap &newMap)
{
    escalatorMap.Modify(newMap);
}

bool MapManager::Load(const string &file)
{
    if (not file_op::File::Exist(file))
    {
        return false;
    }

    // load to escalatorMap
    ifstream.open(file, std::ifstream::in);

    bool ret = escalatorMap.Load(ifstream);
    ret &= staticMap.Load(ifstream);
    ifstream.close();

    return ret;
}

void MapManager::Clear()
{
    escalatorMap.Clear();
    staticMap.Clear();
}

void MapManager::GetMap(psl::SenmaticMap &senmaticMap) const
{
    // TODO : 2022-08-03 11:35:04 [hao]  merge static and escalator
    escalatorMap.GetMap(senmaticMap);
    staticMap.GetMap(senmaticMap);
    aircraftHatchMap.GetMap(senmaticMap);
}

cv::Mat MapManager::Show(const BoxInfo &view, const vector<BoxInfo> &boxes)
{
    cv::Mat image = Map::GetDisplayImage();
    escalatorMap.Show(view, boxes, image);
    return staticMap.Show(view, boxes, image);
}

bool MapManager::RotateMap(const float angle)
{
    staticMap.RotationAllWorld(angle);

    return true;
}

void MapManager::GetDistanceValidStatus(BoxInfo view)
{
    staticMap.SetDistanceValidStatus(view);
}