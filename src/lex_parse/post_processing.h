
#pragma once

#include <map>

#include "ast_node.h"
#include "procedure.h"
#include "variable.h"

using VariableMap = std::map<std::string, std::shared_ptr<Variable>>;
using ProcedureMap = std::map<std::string, std::shared_ptr<Procedure>>;

std::shared_ptr<Code>
visit_expr(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map);
std::vector<std::shared_ptr<Procedure>> generate(ASTNode root);

std::shared_ptr<Variable> visit_vardef(ASTNode root);
std::vector<std::shared_ptr<Code>>
visit_optargs(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map);

std::vector<std::shared_ptr<Code>>
visit_args(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map);

std::shared_ptr<Code>
visit_stmt(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map);

std::shared_ptr<Code>
visit_stmts(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map);

std::vector<std::shared_ptr<Variable>> visit_params(ASTNode root);

std::vector<std::shared_ptr<Variable>> visit_optparams(ASTNode root);

std::shared_ptr<Procedure> visit_fn(ASTNode root, ProcedureMap& proc_map);

std::vector<std::shared_ptr<Procedure>>
visit_fns(ASTNode root, ProcedureMap& proc_map);
