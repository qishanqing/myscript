//
// Created by hao on 2021/7/2.
//

#include "detector_param.h"
#include "utils.h"
#include "src/utils/map_define.h"

const float EXP = 0.01;

BoxInfo::BoxInfo(const cv::Rect box)
{
    this->SetRect(box);
}

void BoxInfo::SetRect(const cv::Rect box)
{
    this->box = box;
    this->height = box.height;
    this->width = box.width;
    this->cx = box.x + (box.width >> 1);
    this->cy = box.y + (box.height >> 1);
}

BoxInfo::BoxInfo()
{
}

BoxInfo &BoxInfo::FromCorner(int xmin, int ymin, int xmax, int ymax)
{
    this->height = ymax - ymin;
    this->width = xmax - xmin;
    this->box = cv::Rect(xmin, ymin, this->width, this->height);
    this->cx = box.x + (box.width >> 1);
    this->cy = box.y + (box.height >> 1);
}

BoxInfo &BoxInfo::FromCenter(int x, int y, int w, int h)
{
    this->box = cv::Rect(x - (w >> 1), y - (h >> 1), w, h);
    this->cx = x;
    this->cy = y;
    this->width = w;
    this->height = h;

    return *this;
}

bool BoxInfo::IsEscalator() const
{
    return this->classID == ESCALATOR or this->classID == ESCALATOR_MODEL;
}

bool BoxInfo::IsEscalatorHandrails() const
{
    return this->classID == ESCALATOR_HANDRAILS or
           this->classID == ESCALATOR_HANDRAILS_MODEL;
}

bool BoxInfo::IsPerson() const
{
    return this->classID == PERSON or this->classID == PERSON_DUMMY;
}

psl::ClassID BoxInfo::ConvertClassID() const
{
    switch (this->classID)
    {
        case PERSON:
            return psl::PERSON;
            break;
        case PERSON_DUMMY:
            return psl::UNKNOWN;
            break;
        case ESCALATOR:
            return psl::ELEVATOR;
            break;
        case ESCALATOR_MODEL:
            return psl::ELEVATOR;
            break;
        default:
            return psl::UNKNOWN;
            break;
    }
}

psl::Instance BoxInfo::ConvertInstace() const
{
    psl::Instance object;

    object.classID = this->ConvertClassID();
    object.confidence = this->score;

    if (IsEscalator())
    {
        object.locationMachine = this->edgeLeft.machine;
    }
    else
    {
        object.locationMachine = this->location.machine;
    }

    return object;
}

float BoxInfo::IoU(const BoxInfo &box) const
{
    return ::IoU(this->box, box.box);
}

bool BoxInfo::Intersection(const BoxInfo &box) const
{
    float xmin0 = this->box.x;
    float ymin0 = this->box.y;
    float xmax0 = this->box.x + this->box.width;
    float ymax0 = this->box.y + this->box.height;

    float xmin1 = box.box.x;
    float ymin1 = box.box.y;
    float xmax1 = box.box.x + box.box.width;
    float ymax1 = box.box.y + box.box.height;

    float w = fmax(0.0f, fmin(xmax0, xmax1) - fmax(xmin0, xmin1));
    float h = fmax(0.0f, fmin(ymax0, ymax1) - fmax(ymin0, ymin1));

    return w > 0 and h > 0;
}

Location::Location()
{

}

Location::Location(const psl::Location &location)
{
    // TODO : bad
    this->machine = location;
    this->world = location;
}

bool Location::PoseValid() const
{
    if (abs(this->machine.x - this->world.x) < EXP
        or abs(this->machine.y - this->world.y) < EXP)
    {
        return false;
    }

    return true;
}

bool Location::Inview() const
{
    return machine.Distance(psl::Location(0, 0, 0)) > MAX_DISTANCE_4_DETECT or
           machine.x < MIN_DISTANCE_4_DETECT;
}

bool DetectResult::Empty() const
{
    return boxes.empty();
}

void DetectResult::Swap(DetectResult& ret)
{
    this->boxes.swap(ret.boxes);
    this->pose = ret.pose;
}
