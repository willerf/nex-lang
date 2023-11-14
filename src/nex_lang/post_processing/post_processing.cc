
#include "post_processing.h"

#include "ast_node.h"
#include "module_table.h"
#include "visit_s.h"

struct Code;

std::vector<std::shared_ptr<TypedProcedure>> generate(
    ASTNode root,
    std::vector<std::shared_ptr<Code>>& static_data,
    ModuleTable& module_table
) {
    std::vector<std::shared_ptr<TypedProcedure>> result;

    result = visit_s(root, module_table, static_data);

    return result;
}
