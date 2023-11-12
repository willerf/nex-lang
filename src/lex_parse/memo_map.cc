
#include "memo_map.h"

#include <algorithm>
#include <functional>
#include <variant>

#include "span_hash.h"

bool MemoKey::operator==(const MemoKey& other) const {
    return std::equal(
               lhs.begin(),
               lhs.end(),
               other.lhs.begin(),
               other.lhs.end()
           )
        && from == other.from && length == other.length;
}

int64_t MemoHash::operator()(const MemoKey& memo_key) const {
    SpanHash<State> span_hasher;
    std::hash<int64_t> num_hasher;

    int64_t h1 = span_hasher(memo_key.lhs);
    int64_t h2 = num_hasher(memo_key.from);
    int64_t h3 = num_hasher(memo_key.length);

    int64_t seed = h1;
    seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
}
