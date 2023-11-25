
#include "post_processing.h"

#include "ast_node.h"
#include "module_table.h"
#include "program_context.h"
#include "visit_s.h"

struct Code;

std::vector<std::shared_ptr<TypedProcedure>> generate(
    ASTNode root,
    std::vector<std::shared_ptr<Code>>& static_data,
    ProgramContext& program_context
) {
    std::vector<std::shared_ptr<TypedProcedure>> result;

    result = visit_s(root, program_context, static_data);

    return result;
}
