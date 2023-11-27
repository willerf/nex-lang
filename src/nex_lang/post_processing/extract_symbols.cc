
#include "extract_symbols.h"

#include "ast_node.h"
#include "extract_s.h"

std::vector<std::string>
extract_symbols(ASTNode root, ProgramContext& program_context) {
    return extract_s(root, program_context);
}
