
#include "parse_earley.h"

#include <algorithm>
#include <iostream>
#include <span>
#include <tuple>
#include <vector>

#include "memo_map.h"

struct CompleteEarleyItem {
    int64_t rule;
    int64_t end;
};

static std::optional<std::vector<ASTNode>> search_sets(
    std::span<State> lhs,
    int64_t from,
    int64_t length,
    std::span<Token>& input,
    std::vector<Production>& productions,
    std::vector<std::vector<CompleteEarleyItem>>& complete_sets,
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
            auto sub_tree = search_sets(
                lhs.subspan(1),
                from + 1,
                length - 1,
                input,
                productions,
                complete_sets,
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
            auto sub_tree = search_sets(
                prod.rhs,
                from,
                length,
                input,
                productions,
                complete_sets,
                grammar,
                memo_map
            );
            memo_map[MemoKey {prod.rhs, from, length}] = sub_tree;
            if (sub_tree) {
                return std::vector<ASTNode> {
                    ASTNode {lhs.front(), "", sub_tree.value()}};
            }
        }
    } else {
        for (auto item : complete_sets.at(from)) {
            Production prod = productions.at(item.rule);
            if (std::holds_alternative<NonTerminal>(lhs.front())
                && prod.lhs == std::get<NonTerminal>(lhs.front())) {
                auto sub_tree1 = search_sets(
                    lhs.subspan(0, 1),
                    from,
                    item.end - from,
                    input,
                    productions,
                    complete_sets,
                    grammar,
                    memo_map
                );
                memo_map[MemoKey {lhs.subspan(0, 1), from, item.end - from}] =
                    sub_tree1;
                if (sub_tree1) {
                    auto sub_tree2 = search_sets(
                        lhs.subspan(1),
                        item.end,
                        length - (item.end - from),
                        input,
                        productions,
                        complete_sets,
                        grammar,
                        memo_map
                    );
                    memo_map[MemoKey {
                        lhs.subspan(1),
                        item.end,
                        length - (item.end - from)}] = sub_tree2;
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
    }
    return std::nullopt;
}

struct EarleyItem {
    int64_t rule;
    int64_t start;
    int64_t next;
    bool operator==(const EarleyItem&) const = default;
};

std::optional<ASTNode> parse_earley(std::span<Token> input, Grammar& grammar) {
    std::vector<std::vector<EarleyItem>> earley_sets;
    earley_sets.push_back({});

    std::vector<Production> productions;
    for (auto& [key, value] : grammar.productions) {
        for (Production prod : value) {
            productions.push_back(prod);
        }
    }

    for (int64_t i = 0; i < productions.size(); ++i) {
        if (productions[i].lhs == grammar.start) {
            earley_sets[0].push_back(EarleyItem {i, 0, 0});
        }
    }

    int64_t x = 0;
    while (x < earley_sets.size()) {
        int64_t y = 0;
        while (y < earley_sets.at(x).size()) {
            EarleyItem item = earley_sets.at(x).at(y);
            Production prod = productions.at(item.rule);
            if (item.next == prod.rhs.size()) {
                for (int64_t i = 0; i < earley_sets.at(item.start).size();
                     ++i) {
                    EarleyItem old = earley_sets.at(item.start).at(i);
                    if (old.next < productions.at(old.rule).rhs.size()
                        && std::holds_alternative<NonTerminal>(
                            productions.at(old.rule).rhs.at(old.next)
                        )
                        && std::get<NonTerminal>(
                               productions.at(old.rule).rhs.at(old.next)
                           ) == productions.at(item.rule).lhs) {
                        EarleyItem new_entry {
                            old.rule,
                            old.start,
                            old.next + 1};
                        bool repeat = false;
                        for (int64_t j = 0; j < earley_sets.at(x).size(); j++) {
                            if (earley_sets.at(x).at(j) == new_entry) {
                                repeat = true;
                            }
                        }
                        if (!repeat) {
                            earley_sets.at(x).push_back(new_entry);
                        }
                    }
                }
            }
            else if (std::holds_alternative<Terminal>(prod.rhs.at(item.next)) && grammar.terminals.count(std::get<Terminal>(prod.rhs.at(item.next)))) {
                if (x < input.size()
                    && std::get<Terminal>(prod.rhs.at(item.next))
                        == input[x].kind) {
                    if (x + 1 == earley_sets.size()) {
                        earley_sets.push_back({});
                    }
                    earley_sets[x + 1].push_back(
                        EarleyItem {item.rule, item.start, item.next + 1}
                    );
                }
            }
            else if (std::holds_alternative<NonTerminal>(prod.rhs.at(item.next)) && grammar.non_terminals.count(std::get<NonTerminal>(prod.rhs.at(item.next)))) {
                for (int64_t i = 0; i < productions.size(); ++i) {
                    if (std::get<NonTerminal>(prod.rhs.at(item.next))
                        == productions.at(i).lhs) {
                        EarleyItem new_entry {i, x, 0};
                        bool repeat = false;
                        for (int64_t j = 0; j < earley_sets.at(x).size(); ++j) {
                            if (earley_sets.at(x).at(j) == new_entry) {
                                repeat = true;
                            }
                        }
                        if (!repeat) {
                            earley_sets.at(x).push_back(new_entry);
                        }
                    }
                }
            } else {
                return std::nullopt;
            }
            ++y;
        }
        ++x;
    }

    std::vector<std::vector<CompleteEarleyItem>> complete_sets;
    for (int64_t i = 0; i < earley_sets.size(); ++i) {
        complete_sets.push_back({});
    }

    for (int64_t i = 0; i < earley_sets.size(); ++i) {
        std::vector<EarleyItem> earley_set = earley_sets.at(i);

        for (int64_t j = 0; j < earley_set.size(); ++j) {
            EarleyItem item = earley_set.at(j);
            if (item.next == productions.at(item.rule).rhs.size()) {
                complete_sets[item.start].push_back(
                    CompleteEarleyItem {item.rule, i}
                );
            }
        }
    }

    MemoMap memo_map;
    std::vector<State> lhs = {grammar.start};
    std::optional<std::vector<ASTNode>> result = search_sets(
        lhs,
        0,
        input.size(),
        input,
        productions,
        complete_sets,
        grammar,
        memo_map
    );
    if (result && result->size()) {
        return result->front();
    }
    return std::nullopt;
}
