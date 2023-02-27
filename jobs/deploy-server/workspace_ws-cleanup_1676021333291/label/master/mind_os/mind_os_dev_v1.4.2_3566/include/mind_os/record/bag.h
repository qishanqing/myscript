/*
 * bag.h
 *
 *  Created on: Aug 29, 2022
 *      Author: ubuntu
 */

#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include "../serialization/serialization.h"
#include "../no_copy.h"

namespace mind_os
{

class Bag : public NoCopy
{
public:
    Bag(std::string path);
    ~Bag();
    bool append(std::string topic, std::uint64_t stamp, std::uint8_t* buf, size_t size);
    bool next(std::string& topic, std::uint64_t& stamp, std::uint8_t* &buf, size_t& size);

    bool append(std::string topic, std::uint64_t stamp, BytesBuffer& buf);
    bool next(std::string& topic, std::uint64_t& stamp, BytesBuffer& buf);
private:
    std::string path;
    std::fstream fs;
    std::mutex mtxFile;
};

}
