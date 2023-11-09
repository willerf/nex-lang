
#include "parse_cyk.h"

#include <iostream>
#include <unordered_map>

static std::optional<std::vector<ASTNode>> recur(
    std::span<State> lhs,
    int64_t from,
    int64_t length,
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
    } else if (std::holds_alternative<Terminal>(lhs.front()) && grammar.terminals.count(std::get<Terminal>(lhs.front()))) {
        if (input[from].kind == std::get<Terminal>(lhs.front())) {
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
                std::vector<ASTNode> result = {
                    ASTNode {input[from].kind, input[from].lexeme, {}}};
                result.insert(result.end(), sub_tree->begin(), sub_tree->end());
                return result;
            }
        }
    } else if (lhs.size() == 1 && std::holds_alternative<NonTerminal>(lhs.front()) && grammar.non_terminals.count(std::get<NonTerminal>(lhs.front()))) {
        for (auto& prod :
             grammar.productions.at(std::get<NonTerminal>(lhs.front()))) {
            auto sub_tree =
                recur(prod.rhs, from, length, input, grammar, memo_map);
            memo_map[MemoKey {prod.rhs, from, length}] = sub_tree;
            if (sub_tree) {
                return std::vector<ASTNode> {
                    ASTNode {lhs.front(), "", sub_tree.value()}};
            }
        }
    } else {
        for (int64_t i = 0; i < length; i++) {
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

std::optional<ASTNode> parse_cyk(std::span<Token> input, Grammar& grammar) {
    MemoMap memo_map;
    std::vector<State> lhs = {grammar.start};
    std::optional<std::vector<ASTNode>> result =
        recur(lhs, 0, input.size(), input, grammar, memo_map);
    if (result && result->size()) {
        return result->front();
    }
    return std::nullopt;
}
