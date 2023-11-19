
#include "extract_symbols.h"

#include "ast_node.h"
#include "extract_s.h"

std::vector<std::string>
extract_symbols(ASTNode root, ModuleTable& module_table) {
    return extract_s(root, module_table);
}
