
#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ast_node.h"
#include "nl_type.h"
#include "program_context.h"

struct ASTNode;
struct ProgramContext;

std::vector<std::pair<std::string, std::shared_ptr<NLType>>>
extract_typestmts(ASTNode root, ProgramContext& program_context);
std::pair<std::string, std::shared_ptr<NLType>>
extract_typestmt(ASTNode root, ProgramContext& program_context);
