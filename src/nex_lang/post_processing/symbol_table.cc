
#include "symbol_table.h"

#include <algorithm>
#include <compare>

bool CmpSymbolTableKey::operator()(
    const SymbolTableKey& lhs,
    const SymbolTableKey& rhs
) const {
    if (lhs.first != rhs.first) {
        return lhs.first < rhs.first;
    }

    return std::lexicographical_compare(
        lhs.second.begin(),
        lhs.second.end(),
        rhs.second.begin(),
        rhs.second.end(),
        [](const std::shared_ptr<NLType>& a, const std::shared_ptr<NLType>& b) {
            return *a < *b;
        }
    );
}

std::vector<std::shared_ptr<TypedID>>
symbol_table_sub(SymbolTable& symbol_table_one, SymbolTable& symbol_table_two) {
    std::vector<std::shared_ptr<TypedID>> result;

    for (auto& [key, value] : symbol_table_one) {
        if (!symbol_table_two.count(key)) {
            result.push_back(value);
        }
    }

    return result;
}
