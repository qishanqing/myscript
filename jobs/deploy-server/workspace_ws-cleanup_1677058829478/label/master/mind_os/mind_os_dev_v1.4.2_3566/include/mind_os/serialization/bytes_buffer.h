#pragma once

#include <string>
#include <string.h>
#include <stdexcept>
#include <vector>

namespace mind_os {

class BytesBuffer
{
public:
    using byte = std::uint8_t;
    /**
     * constructor and de-constructor
     */
    BytesBuffer();
    BytesBuffer(BytesBuffer& other) = delete;
    BytesBuffer(BytesBuffer&& other);
    ~BytesBuffer();
    BytesBuffer(size_t size);

    /**
     * basic functions
     */
    bool isEnable();
    void CopyFrom(byte* data, size_t size);
    void TakeFrom(byte* data, size_t size);
    void LendFrom(byte* data, size_t size);
    void Reserve(size_t size);
    void Resize(size_t size);
    void Release();
    void Reset();
    size_t Size();
    size_t Capacity();
    void IgnoreBytes(size_t size);
    byte* GetDataPointer();
    byte* GetDataPointerWithOffset();

    /**
     * write data functions
     */
    template <typename T>
    inline void operator<<(T data)
    {
        AppendData(data);
    }

    template <typename T>
    inline void operator<<(std::vector<T>& data)
    {
        AppendData(data);
    }

    inline void operator<<(std::string data)
    {
        AppendData(data);
    }

//    template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
    template <typename T>
    inline void AppendData(T data)
    {
        AppendData((byte*)&data, sizeof(T));
    }

//    template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
    template <typename T>
    void AppendData(std::vector<T>& data)
    {
        auto&& sizeOfT = sizeof(T);
        auto&& sizeOfVector = data.size();
        auto&& sizeOfData = sizeOfT * sizeOfVector;

        CheckSize(sizeOfData);

        AppendData(sizeOfVector);
        AppendData((byte*)data.data(), sizeOfData);
    }

    void AppendData(std::string data);
    void AppendData(byte* data, size_t size);

    /**
     * read data functions
     */
    template <typename T>
    inline void operator>>(T& data)
    {
        NextData(data);
    }

    template <typename T>
    inline void operator>>(std::vector<T>& data)
    {
        NextData(data);
    }

    inline void operator>>(std::string& data)
    {
        NextData(data);
    }

//    template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
    template <typename T>
    inline void NextData(T& data)
    {
        NextData((byte*)&data, sizeof(T));
    }

//    template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
    template <typename T>
    void NextData(std::vector<T>& data)
    {
        size_t size;
        NextData(size);
        data.resize(size);
        NextData((std::uint8_t*)data.data(), size * sizeof(T));
    }

    void NextData(std::string& data);
    void NextData(byte* data, size_t size);

private:
    byte* buf;
    size_t offset;
    size_t maxSize;
    bool toRelease;

    void CheckSize(size_t size);
};

}
