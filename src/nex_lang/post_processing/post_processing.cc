
#include "post_processing.h"

#include "symbol_table.h"
#include "visit_s.h"

std::vector<std::shared_ptr<TypedProcedure>>
generate(ASTNode root, std::vector<std::shared_ptr<Code>>& static_data) {
    std::vector<std::shared_ptr<TypedProcedure>> result;

    SymbolTable symbol_table;
    result = visit_s(root, symbol_table, static_data);

    return result;
}
