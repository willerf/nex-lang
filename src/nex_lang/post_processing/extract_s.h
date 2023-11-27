
#pragma once

#include <string>
#include <vector>

#include "ast_node.h"
#include "module_table.h"
#include "program_context.h"

struct ASTNode;
struct ProgramContext;

std::vector<std::string>
extract_s(ASTNode root, ProgramContext& program_context);
