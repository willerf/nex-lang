
#pragma once

#include <stdint.h>

#include <optional>
#include <span>
#include <unordered_map>
#include <vector>

#include "ast_node.h"
#include "state.h"

struct MemoKey {
    std::span<State> lhs;
    int64_t from;
    int64_t length;

    bool operator==(const MemoKey& other) const;
};

struct MemoHash {
    int64_t operator()(const MemoKey& memo_key) const;
};

using MemoValue = std::optional<std::vector<ASTNode>>;
using MemoMap = std::unordered_map<MemoKey, MemoValue, MemoHash>;
