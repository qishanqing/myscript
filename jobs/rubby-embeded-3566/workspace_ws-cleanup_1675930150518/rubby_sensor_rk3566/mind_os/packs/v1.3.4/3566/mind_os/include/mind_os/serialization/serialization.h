/*
 * serialization.h
 *
 *  Created on: Aug 9, 2022
 *      Author: ubuntu
 */

#pragma once

#include "../const_ptr.h"
#include "bytes_buffer.h"
#include <list>
#include <glog/logging.h>
#include <exception>

namespace mind_os {

// basic
template <typename T>
inline void serialize(const T& data, BytesBuffer& buffer) {
    buffer << data;
}

template <typename T>
inline void deserialize(BytesBuffer& buffer, T& data) {
    buffer >> data;
}

// vector
template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>>
inline void serialize(const std::vector<T>& data, BytesBuffer& buffer) {
    buffer << data.size();
    for (const auto& e : data) {
        serialize(e, buffer);
    }
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>>
inline void deserialize(BytesBuffer& buffer, std::vector<T>& data) {
    size_t size;
    buffer >> size;
    std::vector<T> v(size);
    for (size_t i = 0; i < size; ++i) {
        deserialize(buffer, v[i]);
    }
    data.swap(v);
}

// list
template <typename T>
inline void serialize(const std::list<T>& data, BytesBuffer& buffer) {
    buffer << data.size();
    for (const auto& e : data) {
        serialize(e, buffer);
    }
}

template <typename T>
inline void deserialize(BytesBuffer& buffer, std::list<T>& data) {
    size_t size;
    buffer >> size;
    for (size_t i = 0; i < size; ++i) {
        T e;
        deserialize(buffer, e);
        data.emplace_back(e);
    }
}

//// c-array
template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
inline void serialize(T* data, size_t size, BytesBuffer& buffer) {
    buffer.AppendData((BytesBuffer::byte*)data, size * sizeof(T));
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>, bool flag = true>
inline void serialize(T* data, size_t size, BytesBuffer& buffer) {
    for (size_t i = 0; i < size; ++i)
    {
        serialize(data[i], buffer);
    }
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
inline void deserialize(BytesBuffer& buffer, T* data, size_t size) {
    buffer.NextData((BytesBuffer::byte*)data, size * sizeof(T));
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>, bool flag = true>
inline void deserialize(BytesBuffer& buffer, T* data, size_t size) {
    for (size_t i = 0; i < size; ++i)
    {
        deserialize(buffer, data[i]);
    }
}

}
