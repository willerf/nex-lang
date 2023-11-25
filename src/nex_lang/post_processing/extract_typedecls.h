
#pragma once

#include "ast_node.h"
#include "program_context.h"

void extract_typedecls(ASTNode root, ProgramContext& program_context);
void extract_typedecl(ASTNode root, ProgramContext& program_context);
