
#pragma once

#include <string>
#include <vector>

#include "ast_node.h"
#include "program_context.h"
#include "symbol_table.h"

struct ASTNode;
struct ProgramContext;

std::vector<std::string>
extract_imports(ASTNode root, ProgramContext& program_context);

std::vector<std::string>
extract_import(ASTNode root, ProgramContext& program_context);
