#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "nl_type.h"
#include "typed_id.h"
#include "typed_procedure.h"
#include "typed_variable.h"

using SymbolTableKey =
    std::pair<std::string, std::vector<std::shared_ptr<NLType>>>;

struct CmpSymbolTableKey {
    bool operator()(const SymbolTableKey& lhs, const SymbolTableKey& rhs) const;
};

using SymbolTable =
    std::map<SymbolTableKey, std::shared_ptr<TypedID>, CmpSymbolTableKey>;

std::vector<std::shared_ptr<TypedID>>
symbol_table_sub(SymbolTable& symbol_table_one, SymbolTable& symbol_table_two);
