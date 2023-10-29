
#include "parsing.h"

#include <unordered_map>

#include "span_hash.h"
#include "state.h"

struct MemoKey {
    std::span<State> lhs;
    size_t from;
    size_t length;

    bool operator==(const MemoKey& other) const {
        return std::equal(
                   lhs.begin(),
                   lhs.end(),
                   other.lhs.begin(),
                   other.lhs.end()
               )
            && from == other.from && length == other.length;
    }
};

struct MemoHash {
    size_t operator()(const MemoKey& memo_key) const {
        SpanHash<State> span_hasher;
        std::hash<size_t> num_hasher;

        size_t h1 = span_hasher(memo_key.lhs);
        size_t h2 = num_hasher(memo_key.from);
        size_t h3 = num_hasher(memo_key.length);

        size_t seed = h1;
        seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

typedef std::optional<std::vector<ASTNode>> MemoValue;
typedef std::unordered_map<MemoKey, MemoValue, MemoHash> MemoMap;

static std::optional<std::vector<ASTNode>> recur(
    std::span<State> lhs,
    size_t from,
    size_t length,
    std::span<Token>& input,
    Grammar& grammar,
    MemoMap& memo_map
) {
    if (memo_map.count(MemoKey {lhs, from, length})) {
        return memo_map[MemoKey {lhs, from, length}];
    }
    memo_map[MemoKey {lhs, from, length}] = std::nullopt;

    if (lhs.empty()) {
        if (length) {
            return std::nullopt;
        } else {
            return std::vector<ASTNode> {};
        }
    } else if (grammar.terminals.count(lhs.front())) {
        if (input[from].kind == lhs.front()) {
            auto sub_tree = recur(
                lhs.subspan(1),
                from + 1,
                length - 1,
                input,
                grammar,
                memo_map
            );
            memo_map[MemoKey {lhs.subspan(1), from + 1, length - 1}] = sub_tree;
            if (sub_tree) {
                std::vector<ASTNode> result = {ASTNode {input[from]}};
                result.insert(result.end(), sub_tree->begin(), sub_tree->end());
            }
        }
    } else if (lhs.size() == 1 && grammar.non_terminals.count(lhs.front())) {
        for (auto& prod : grammar.productions.at(lhs.front())) {
            auto sub_tree =
                recur(prod.rhs, from, length, input, grammar, memo_map);
            memo_map[MemoKey {prod.rhs, from, length}] = sub_tree;
            if (sub_tree) {
                return std::vector<ASTNode> {
                    ASTNode {Token {lhs.front(), ""}, sub_tree.value()}};
            }
        }
    } else {
        for (size_t i = 0; i < length; i++) {
            auto sub_tree1 =
                recur(lhs.subspan(0, 1), from, i, input, grammar, memo_map);
            memo_map[MemoKey {lhs.subspan(0, 1), from, i}] = sub_tree1;
            if (sub_tree1) {
                auto sub_tree2 = recur(
                    lhs.subspan(1),
                    from + i,
                    length - i,
                    input,
                    grammar,
                    memo_map
                );
                memo_map[MemoKey {lhs.subspan(1), from + i, length - i}] =
                    sub_tree2;
                if (sub_tree2) {
                    std::vector<ASTNode> result = sub_tree1.value();
                    result.insert(
                        result.end(),
                        sub_tree2->begin(),
                        sub_tree2->end()
                    );
                    return result;
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<ASTNode> parse_cyk(std::span<Token>& input, Grammar& grammar) {
    MemoMap memo_map;
    std::vector<State> lhs = {grammar.start};
    std::optional<std::vector<ASTNode>> result =
        recur(lhs, 0, input.size(), input, grammar, memo_map);
    if (result && result->size()) {
        return result->front();
    }
    return std::nullopt;
}
