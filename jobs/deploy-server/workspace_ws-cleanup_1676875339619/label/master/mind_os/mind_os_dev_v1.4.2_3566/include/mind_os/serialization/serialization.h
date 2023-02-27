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
#include <map>
#include <unordered_map>

namespace mind_os {

//  -----------------------  basic ---------------------------
template <typename T>
inline void serialize(BytesBuffer& buffer, const T& data) {
    buffer << data;
}

template <typename T>
inline void deserialize(T& data, BytesBuffer& buffer) {
    buffer >> data;
}

template <typename T>
inline void serialize(const T& data, BytesBuffer& buffer) {
    serialize(buffer, data);
}

template <typename T>
inline void deserialize(BytesBuffer& buffer, T& data) {
    deserialize(data, buffer);
}

//  -----------------------  vector ---------------------------
template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>>
inline void serialize(BytesBuffer& buffer, const std::vector<T>& data) {
    buffer << data.size();
    for (const auto& e : data) {
        serialize(e, buffer);
    }
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>>
inline void deserialize(std::vector<T>& data, BytesBuffer& buffer) {
    size_t size;
    buffer >> size;
    std::vector<T> v(size);
    for (size_t i = 0; i < size; ++i) {
        deserialize(buffer, v[i]);
    }
    data.swap(v);
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>>
inline void serialize(const std::vector<T>& data, BytesBuffer& buffer) {
    serialize(buffer, data);
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>>
inline void deserialize(BytesBuffer& buffer, std::vector<T>& data) {
    deserialize(data, buffer);
}

//  -----------------------  list ---------------------------
template <typename T>
inline void serialize(BytesBuffer& buffer, const std::list<T>& data) {
    buffer << data.size();
    for (const auto& e : data) {
        serialize(e, buffer);
    }
}

template <typename T>
inline void deserialize(std::list<T>& data, BytesBuffer& buffer) {
    size_t size;
    buffer >> size;
    for (size_t i = 0; i < size; ++i) {
        T e;
        deserialize(buffer, e);
        data.emplace_back(e);
    }
}

template <typename T>
inline void serialize(const std::list<T>& data, BytesBuffer& buffer) {
    serialize(buffer, data);
}


template <typename T>
inline void deserialize(BytesBuffer& buffer, std::list<T>& data) {
    deserialize(data, buffer);
}

//  -----------------------  map ---------------------------
template<typename K, typename V>
inline void serialize(BytesBuffer& buffer, const std::map<K, V>& data) {
    buffer << data.size();
    for (auto& a : data)
    {
        serialize(a.first, buffer);
        serialize(a.second, buffer);
    }
};

template<typename K, typename V>
inline void deserialize(std::map<K, V>& data, BytesBuffer& buffer) {
    size_t size;
    buffer >> size;
    for (;size > 0;--size)
    {
        K k;
        V v;
        deserialize(buffer, k);
        deserialize(buffer, v);
        data.insert(std::make_pair(k, v));
    }
};

template<typename K, typename V>
inline void serialize(const std::map<K, V>& data, BytesBuffer& buffer) {
    serialize(buffer, data);
};

template<typename K, typename V>
inline void deserialize(BytesBuffer& buffer, std::map<K, V>& data) {
    deserialize(data, buffer);
};

//  -----------------------  unordered_map ---------------------------
template<typename K, typename V>
inline void serialize(BytesBuffer& buffer, const std::unordered_map<K, V>& data) {
    buffer << data.size();
    for (auto& a : data)
    {
        serialize(a.first, buffer);
        serialize(a.second, buffer);
    }
};

template<typename K, typename V>
inline void deserialize(std::unordered_map<K, V>& data, BytesBuffer& buffer) {
    size_t size;
    buffer >> size;
    for (;size > 0;--size)
    {
        K k;
        V v;
        deserialize(buffer, k);
        deserialize(buffer, v);
        data.insert(std::make_pair(k, v));
    }
};

template<typename K, typename V>
inline void serialize(const std::unordered_map<K, V>& data, BytesBuffer& buffer) {
    serialize(buffer, data);
};

template<typename K, typename V>
inline void deserialize(BytesBuffer& buffer, std::unordered_map<K, V>& data) {
    deserialize(data, buffer);
};

//  -----------------------  c-array-basic ---------------------------
template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
inline void serialize(BytesBuffer& buffer, T* data, size_t size) {
    buffer.AppendData((BytesBuffer::byte*)data, size * sizeof(T));
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
inline void deserialize(T* data, size_t size, BytesBuffer& buffer) {
    buffer.NextData((BytesBuffer::byte*)data, size * sizeof(T));
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
inline void serialize(T* data, size_t size, BytesBuffer& buffer) {
    serialize(buffer, data, size);
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<!std::is_class_v<std::remove_reference_t<T>>, T>>
inline void deserialize(BytesBuffer& buffer, T* data, size_t size) {
    deserialize(data, size, buffer);
}

//  -----------------------  c-array-class ---------------------------
template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>, bool flag = true>
inline void serialize(BytesBuffer& buffer, T* data, size_t size) {
    for (size_t i = 0; i < size; ++i)
    {
        serialize(data[i], buffer);
    }
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>, bool flag = true>
inline void deserialize(T* data, size_t size, BytesBuffer& buffer) {
    for (size_t i = 0; i < size; ++i)
    {
        deserialize(buffer, data[i]);
    }
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>, bool flag = true>
inline void serialize(T* data, size_t size, BytesBuffer& buffer) {
    serialize(buffer, data, size);
}

template <typename T, typename TYPE_GUARD = typename std::enable_if_t<std::is_class_v<std::remove_reference_t<T>>, T>, bool flag = true>
inline void deserialize(BytesBuffer& buffer, T* data, size_t size) {
    deserialize(data, size, buffer);
}

}
