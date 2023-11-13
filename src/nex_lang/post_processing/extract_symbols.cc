
#include "extract_symbols.h"

#include "ast_node.h"
#include "extract_s.h"

void extract_symbols(ASTNode root, ModuleTable& module_table) {
    extract_s(root, module_table);
}
