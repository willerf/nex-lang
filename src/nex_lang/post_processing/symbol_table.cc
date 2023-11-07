
#include "symbol_table.h"

std::vector<std::shared_ptr<TypedID>>
symbol_table_sub(SymbolTable& symbol_table_one, SymbolTable& symbol_table_two) {
    std::vector<std::shared_ptr<TypedID>> result;

    for (auto& [name, typed_id] : symbol_table_one) {
        if (!symbol_table_two.count(name
            ) /*&& symbol_table_two[name] == typed_id*/) {
            result.push_back(typed_id);
        }
    }

    return result;
}


