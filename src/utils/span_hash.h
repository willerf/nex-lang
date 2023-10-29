#pragma once

#include <functional>
#include <span>

template<typename T>
struct SpanHash {
    size_t operator()(const std::span<T>& seq) const {
        std::hash<T> hasher;
        size_t seed = seq.size();
        for (const T& elem : seq) {
            seed ^= hasher(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};
