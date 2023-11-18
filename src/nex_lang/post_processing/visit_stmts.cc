
#include "visit_stmts.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>

#include "assembly.h"
#include "ast_node.h"
#include "block.h"
#include "call.h"
#include "if_stmt.h"
#include "nl_type.h"
#include "nl_type_bool.h"
#include "nl_type_ptr.h"
#include "operators.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "ret_stmt.h"
#include "scope.h"
#include "state.h"
#include "type_mismatch_error.h"
#include "typed_expr.h"
#include "typed_variable.h"
#include "visit_expr.h"
#include "visit_vardef.h"
#include "while_loop.h"

struct Variable;

std::shared_ptr<Code> visit_stmt(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table,
    ModuleTable& module_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmt);
    std::shared_ptr<Code> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::stmt,
            Terminal::LET,
            NonTerminal::vardef,
            Terminal::ASSIGN,
            NonTerminal::expr,
            Terminal::SEMI}) {
        // extract variable declaration and assignment
        ASTNode vardef = root.children.at(1);
        auto typed_var = visit_vardef(vardef, symbol_table);

        ASTNode expr_node = root.children.at(3);
        TypedExpr expr = visit_expr(
            expr_node,
            false,
            symbol_table,
            module_table,
            static_data
        );

        if ((*typed_var->nl_type) != (*expr.nl_type)) {
            throw TypeMismatchError(
                "Cannot assign expression of type '" + expr.nl_type->to_string()
                    + "' to left hand side of type '"
                    + typed_var->nl_type->to_string() + "'.",
                root.children.at(2).line_no
            );
        }
        result = assign(typed_var->variable, expr.code);
    } else if (prod == std::vector<State> {NonTerminal::stmt, NonTerminal::expr, Terminal::ASSIGN, NonTerminal::expr, Terminal::SEMI}) {
        // extract variable assignment
        ASTNode lhs = root.children.at(0);
        TypedExpr mem_address =
            visit_expr(lhs, true, symbol_table, module_table, static_data);

        ASTNode expr = root.children.at(2);
        TypedExpr code =
            visit_expr(expr, false, symbol_table, module_table, static_data);

        if ((*mem_address.nl_type) != (*code.nl_type)) {
            throw TypeMismatchError(
                "Cannot assign expression of type '" + code.nl_type->to_string()
                    + "' to left hand side of type '"
                    + mem_address.nl_type->to_string() + "'.",
                root.children.at(1).line_no
            );
        }
        result = assign_to_address(mem_address.code, code.code);
    } else if (prod == std::vector<State> {NonTerminal::stmt, NonTerminal::expr, Terminal::SEMI}) {
        // extract run expression
        ASTNode expr = root.children.at(0);
        TypedExpr code =
            visit_expr(expr, false, symbol_table, module_table, static_data);
        result = code.code;
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, NonTerminal::stmtblock, Terminal::ELSE, NonTerminal::stmtblock}) {
        // extract if else statements
        ASTNode expr = root.children.at(2);
        TypedExpr comp =
            visit_expr(expr, false, symbol_table, module_table, static_data);

        ASTNode stmtblock_thens = root.children.at(4);
        auto thens = visit_stmtblock(
            stmtblock_thens,
            curr_proc,
            symbol_table,
            module_table,
            static_data
        );

        ASTNode stmtblock_elses = root.children.at(6);
        auto elses = visit_stmtblock(
            stmtblock_elses,
            curr_proc,
            symbol_table,
            module_table,
            static_data
        );

        if ((*comp.nl_type) != NLTypeBool()) {
            throw TypeMismatchError(
                "If statement condition must result in bool type.",
                root.children.at(0).line_no
            );
        }
        result = make_if(
            comp.code,
            op::ne_cmp(),
            make_add(Reg::Result, Reg::Zero, Reg::Zero),
            thens,
            elses
        );
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, NonTerminal::stmtblock}) {
        // extract if statements
        ASTNode expr = root.children.at(2);
        TypedExpr comp =
            visit_expr(expr, false, symbol_table, module_table, static_data);

        ASTNode stmtblock_thens = root.children.at(4);
        auto thens = visit_stmtblock(
            stmtblock_thens,
            curr_proc,
            symbol_table,
            module_table,
            static_data
        );

        if ((*comp.nl_type) != NLTypeBool()) {
            throw TypeMismatchError(
                "If statement condition must result in bool type.",
                root.children.at(0).line_no
            );
        }
        result = make_if(
            comp.code,
            op::ne_cmp(),
            make_add(Reg::Result, Reg::Zero, Reg::Zero),
            thens
        );
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::WHILE, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, NonTerminal::stmtblock}) {
        // extract while loops
        ASTNode expr = root.children.at(2);
        TypedExpr comp =
            visit_expr(expr, false, symbol_table, module_table, static_data);

        ASTNode stmtblock = root.children.at(4);
        auto stmts = visit_stmtblock(
            stmtblock,
            curr_proc,
            symbol_table,
            module_table,
            static_data
        );

        if ((*comp.nl_type) != NLTypeBool()) {
            throw TypeMismatchError(
                "While loop statement condition must result in bool type.",
                root.children.at(0).line_no
            );
        }
        return make_while(
            comp.code,
            op::ne_cmp(),
            make_add(Reg::Result, Reg::Zero, Reg::Zero),
            stmts
        );
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::RET, NonTerminal::expr, Terminal::SEMI}) {
        // extract return statements
        ASTNode expr_node = root.children.at(1);
        TypedExpr expr = visit_expr(
            expr_node,
            false,
            symbol_table,
            module_table,
            static_data
        );

        if ((*curr_proc->ret_type) != (*expr.nl_type)) {
            throw TypeMismatchError(
                "Cannot return expression of type '" + expr.nl_type->to_string()
                    + "' from function with return type '"
                    + curr_proc->ret_type->to_string() + "'.",
                root.children.at(0).line_no
            );
        }
        result = std::make_shared<RetStmt>(expr.code);
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::DELETE, NonTerminal::expr, Terminal::SEMI}) {
        // extract return statements
        ASTNode expr_node = root.children.at(1);
        TypedExpr expr = visit_expr(
            expr_node,
            false,
            symbol_table,
            module_table,
            static_data
        );

        std::shared_ptr<NLTypePtr> nl_type_ptr =
            std::dynamic_pointer_cast<NLTypePtr>(expr.nl_type);
        if (!nl_type_ptr) {
            throw TypeMismatchError(
                "Cannot delete non-pointer type.",
                root.children.at(0).line_no
            );
        }

        std::shared_ptr<TypedProcedure> typed_proc =
            std::dynamic_pointer_cast<TypedProcedure>(
                module_table.at("heap").at("heap_free")
            );
        assert(typed_proc);

        result = make_call(typed_proc->procedure, {expr.code});
    } else {
        std::cerr << "Invalid production found while processing stmt."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}

std::shared_ptr<Code> visit_stmts(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table,
    ModuleTable& module_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmts);
    std::shared_ptr<Code> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt}) {
        // extract singular statement
        ASTNode stmt = root.children.at(0);
        result = visit_stmt(
            stmt,
            curr_proc,
            symbol_table,
            module_table,
            static_data
        );
    } else if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt, NonTerminal::stmts}) {
        // extract code statement
        ASTNode stmt = root.children.at(0);
        std::shared_ptr<Code> code = visit_stmt(
            stmt,
            curr_proc,
            symbol_table,
            module_table,
            static_data
        );

        // extract rest of statements
        ASTNode stmts = root.children.at(1);
        std::shared_ptr<Code> rest_of_code = visit_stmts(
            stmts,
            curr_proc,
            symbol_table,
            module_table,
            static_data
        );
        result = make_block({code, rest_of_code});
    } else {
        std::cerr << "Invalid production found while processing stmts."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}

std::shared_ptr<Code> visit_stmtblock(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table,
    ModuleTable& module_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmtblock);
    std::shared_ptr<Code> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::stmtblock,
            Terminal::LBRACE,
            NonTerminal::stmts,
            Terminal::RBRACE}) {
        // extract statements

        ASTNode stmts_node = root.children.at(1);

        SymbolTable symbol_table_scoped = symbol_table;
        auto stmts = visit_stmts(
            stmts_node,
            curr_proc,
            symbol_table_scoped,
            module_table,
            static_data
        );

        // grab scoped variables
        std::vector<std::shared_ptr<Variable>> scoped_vars;
        for (auto typed_id :
             symbol_table_sub(symbol_table_scoped, symbol_table)) {
            if (auto typed_variable =
                    std::dynamic_pointer_cast<TypedVariable>(typed_id)) {
                scoped_vars.push_back(typed_variable->variable);
            }
        }
        result = make_scope(scoped_vars, stmts);
    } else {
        std::cerr << "Invalid production found while processing stmtblock."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}
