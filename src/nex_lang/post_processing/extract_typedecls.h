
#pragma once

#include "ast_node.h"
#include "program_context.h"

struct ASTNode;
struct ProgramContext;

void extract_typedecls(ASTNode root, ProgramContext& program_context);
void extract_typedecl(ASTNode root, ProgramContext& program_context);
