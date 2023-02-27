#include "map.h"
#include "src/utils/define.h"
#include "src/utils/file.h"

long Map::lastID = 0;
Color Map::colors;

Map::Map()
{

}

bool Map::Save(std::ofstream &ofstream)
{
    if (!ofstream.is_open())
    {
        ofstream.close(); // TODO : it need close?
        LOG_CHECK_STATUS(WARNING) << "fail to open map file ";
        return false;
    }

    WRITE_LIST(ofstream, mapInner);
    file_op::File::WriteItem(ofstream, lastID);
    file_op::File::WriteItem(ofstream, time);

    END();
    return true;
}

bool Map::Load(std::ifstream &ifstream)
{
    if (!ifstream.is_open())
    {
        ifstream.close();
        LOG_CHECK_STATUS(INFO) << "fail to open file.";
        return false;
    }

    READ_LIST(ifstream, mapInner);
    file_op::File::ReadItem(ifstream, lastID);
    file_op::File::ReadItem(ifstream, time);

    size_t sizeMap = mapInner.size();
    LOG_CHECK_DEBUG(INFO) << "map size: " << sizeMap;

    for (size_t i = 0; i < sizeMap; ++i)
    {
        auto &escalatorInner = mapInner.at(i);
        DLObject escalator;

        escalator.id = escalatorInner.id;
        escalator.classID = escalatorInner.classID;
        escalator.statusOperator = psl::Object::INITED_DL;
        mapUpdate = true;
        escalator.history = true;
        escalator.locationWorld.resize(2); // left and right
        mapInter.push_back(escalator);
        escalatorInner.escalator = --mapInter.end();
        escalatorInner.Fit();
    }

    END();
    return true;
}

void Map::GetMap(psl::SenmaticMap &senmaticMap) const
{
    for (const auto &object : this->mapInter)
    {
        senmaticMap.objects.push_back(object);
    }

    if (mapUpdate)
        senmaticMap.status = psl::SenmaticMap::Status::UPDATE;
    else
        senmaticMap.status = psl::SenmaticMap::Status::NO_UPDATE;
}

void Map::Update(const vector<BoxInfo> &boxes, const BoxInfo &view, const psl::Time &time)
{
    this->view = view;
    this->time = time;
}

DLObject Map::Box2Object(const BoxInfo &box)
{
    DLObject object;
    object.id = this->lastID;
    object.classID = box.classID;
    object.statusOperator = psl::Object::StatusOperate(
            psl::Object::StatusOperate::INITED_DL);
    object.time = this->time;
    object.score = box.score;
    object.status = psl::Object::Status::ENABLE;
    object.detectExtend = detectParam.modelProperty.detectExtend[object.classID];
    object.history = false;
    mapUpdate = true;
    object.viewLocationWorld = view.location.world;
    this->lastID++;
    return object;
}

cv::Mat Map::GetDisplayImage()
{
    return cv::Mat::zeros(map_type::W, map_type::H, CV_8UC3);
}

void Map::SetParam(const psl::DetectorParam &detectParam)
{
    this->detectParam = detectParam;
}