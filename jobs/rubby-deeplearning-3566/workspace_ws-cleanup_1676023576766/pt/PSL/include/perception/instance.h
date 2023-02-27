#pragma once

#include <fstream>
#include "class.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "base/Time.h"

namespace psl
{
using LOCATION_TYPE = double;
const LOCATION_TYPE DEFAULT_LOCATION = INT16_MAX;

struct Location
{
    LOCATION_TYPE x;
    LOCATION_TYPE y;
    LOCATION_TYPE z;

    Location()
    {
        this->x = DEFAULT_LOCATION;
        this->y = DEFAULT_LOCATION;
        this->z = DEFAULT_LOCATION;
    }

    Location(const LOCATION_TYPE x, const LOCATION_TYPE y, const LOCATION_TYPE z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    bool IsDefault() const
    {
        return abs(x - DEFAULT_LOCATION) < 0.0001;
    }

    Location &operator=(const Location &l)
    {
        this->x = l.x;
        this->y = l.y;
        this->z = l.z;

        return *this;
    }

    Location operator-(const Location &l) const
    {
        Location ret;
        ret.x = this->x - l.x;
        ret.y = this->y - l.y;
        ret.z = this->z - l.z;

        return ret;
    }

    Location operator+(const Location &l) const
    {
        Location ret;
        ret.x = this->x + l.x;
        ret.y = this->y + l.y;
        ret.z = this->z + l.z;

        return ret;
    }

    Location operator/(const Location &l) const
    {
        Location ret;
        ret.x = this->x / l.x;
        ret.y = this->y / l.y;
        ret.z = this->z / l.z;

        return ret;
    }

    Location operator*(const Location &l) const
    {
        Location ret;
        ret.x = this->x * l.x;
        ret.y = this->y * l.y;
        ret.z = this->z * l.z;

        return ret;
    }

    float Distance(const Location &l) const
    {
        Location diff = *this - l;
        return sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    }

    float DistanceXY(const Location &l) const
    {
        Location diff = *this - l;
        return sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    bool Write(std::ofstream &stream) const
    {
        if (!stream.is_open()) return false;

        stream << this->x << ",";
        stream << this->y << ",";
        stream << this->z;

        return true;
    }

    bool Read(std::ifstream &stream)
    {
        if (!stream.is_open()) return false;

        char flag;
        stream >> this->x >> flag;
        stream >> this->y >> flag;
        stream >> this->z;

        return true;
    }

    bool WriteBinary(std::ofstream &stream) const
    {
        if (!stream.is_open()) return false;

        stream.write(reinterpret_cast<const char *>(&x), sizeof(x));
        stream.write(reinterpret_cast<const char *>(&y), sizeof(y));
        stream.write(reinterpret_cast<const char *>(&z), sizeof(z));

        return true;
    }

    bool ReadBinary(std::ifstream &stream)
    {
        if (!stream.is_open()) return false;

        stream.read(reinterpret_cast<char *>(&x), sizeof(x));
        stream.read(reinterpret_cast<char *>(&y), sizeof(y));
        stream.read(reinterpret_cast<char *>(&z), sizeof(z));

        return true;
    }

    std::string Print() const
    {
        char message[100];
        sprintf(message, "x,y,z : %.2f, %.2f, %.2f", x, y, z);
        return message;
    }
};

struct Instance
{
    enum Status
    {
        RUN = 0, STAND // pedestrian
        , UNKNOWN = 100 // other
    };

    int id = -1;
    ClassID classID = psl::UNKNOWN;
    double confidence = 0; // the probability of the instance is classID
    Status status = UNKNOWN;
    Location locationWorld;
    Location locationMachine;
    psl::Time time;

    bool Write(std::ofstream &stream) const
    {
        if (!stream.is_open())
        {
            return false;
        }

        stream << this->id << ","
               << this->classID << ","
               << this->confidence << ","
               << this->status << ","
               << this->time << ",";
        bool ret = locationMachine.Write(stream);
        stream << ',';
        ret &= locationWorld.Write(stream);
        stream << "\n";

        return ret;
    }

    bool Read(std::ifstream &stream)
    {
        if (!stream.is_open())
        {
            return false;
        }

        char flag;
        int enumTemp;
        stream >> this->id >> flag;
        stream >> enumTemp >> flag;
        this->classID = (ClassID) enumTemp;
        stream >> enumTemp >> flag;
        stream >> this->confidence >> flag;
        this->status = (Status) enumTemp;
        stream >> this->time >> flag;

        bool ret = locationMachine.Read(stream);
        stream >> flag;
        ret &= locationWorld.Read(stream);
        stream >> flag;

        return ret;
    }
};

struct Object
{
    enum StatusOperate
    {
        INITED_DL = 0, UPDATE_DL, //  [dl]
        INITED_SLAM, UPDATE_SLAM, //  [slam]
    };

    enum Status
    {
        ENABLE = 0, DISABLE
    };

    int id = -1;    // object ID
    ClassID classID = psl::UNKNOWN; // senmatic id
    // convex for object; one point for person, two point for escalator
    std::vector<Location> locationWorld;
    psl::Time time;
    StatusOperate statusOperator; // the status of operator
    signed long int nodeID;       // node id for slam [slam]
    bool history = true; // is load from offline map [slam]
    Status status = DISABLE;
};

struct SenmaticMap
{
    enum Status
    {
        NO_UPDATE = 0, UPDATE
    };

    std::vector<Object> objects;
    bool finalUpdate = false;    //  final update befor the task close;
    Status status = NO_UPDATE;
};
}

