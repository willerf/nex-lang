
#pragma once

#include <map>

#include "ast_node.h"
#include "nl_type.h"
#include "procedure.h"
#include "symbol_table.h"
#include "typed_expr.h"
#include "typed_id.h"
#include "typed_procedure.h"
#include "typed_variable.h"
#include "variable.h"

TypedExpr
visit_expr(ASTNode root, bool read_address, SymbolTable& symbol_table);

std::vector<TypedExpr> visit_args(ASTNode root, SymbolTable& symbol_table);

std::vector<TypedExpr> visit_optargs(ASTNode root, SymbolTable& symbol_table);

std::shared_ptr<NLType> visit_type(ASTNode root);

std::shared_ptr<TypedVariable>
visit_vardef(ASTNode root, SymbolTable& symbol_table);

std::shared_ptr<Code> visit_stmt(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table
);

std::shared_ptr<Code> visit_stmts(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table
);

std::vector<std::shared_ptr<TypedVariable>>
visit_params(ASTNode root, SymbolTable& symbol_table);

std::vector<std::shared_ptr<TypedVariable>>
visit_optparams(ASTNode root, SymbolTable& symbol_table);

std::shared_ptr<TypedProcedure>
visit_fn(ASTNode root, SymbolTable& symbol_table);

std::vector<std::shared_ptr<TypedProcedure>>
visit_fns(ASTNode root, SymbolTable& symbol_table);

std::vector<std::shared_ptr<TypedProcedure>>
visit_s(ASTNode root, SymbolTable& symbol_table);

std::vector<std::shared_ptr<TypedProcedure>> generate(ASTNode root);
