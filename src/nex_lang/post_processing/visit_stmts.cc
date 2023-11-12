
#include "visit_stmts.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

#include "assembly.h"
#include "ast_node.h"
#include "block.h"
#include "if_stmt.h"
#include "nl_type.h"
#include "nl_type_bool.h"
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
            Terminal::SEMI
        }) {
        // extract variable declaration and assignment
        ASTNode vardef = root.children.at(1);
        auto typed_var = visit_vardef(vardef, symbol_table);

        ASTNode expr_node = root.children.at(3);
        TypedExpr expr =
            visit_expr(expr_node, false, symbol_table, static_data);

        if ((*typed_var->nl_type) != (*expr.nl_type)) {
            throw TypeMismatchError(
                "Cannot assign expression of type'" + expr.nl_type->to_string()
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
            visit_expr(lhs, true, symbol_table, static_data);

        ASTNode expr = root.children.at(2);
        TypedExpr code = visit_expr(expr, false, symbol_table, static_data);

        if ((*mem_address.nl_type) != (*code.nl_type)) {
            throw TypeMismatchError(
                "Cannot assign expression of type'" + code.nl_type->to_string()
                    + "' to left hand side of type '"
                    + mem_address.nl_type->to_string() + "'.",
                root.children.at(1).line_no
            );
        }
        result = assign_to_address(mem_address.code, code.code);
    } else if (prod == std::vector<State> {NonTerminal::stmt, NonTerminal::expr, Terminal::SEMI}) {
        // extract run expression
        ASTNode expr = root.children.at(0);
        TypedExpr code = visit_expr(expr, false, symbol_table, static_data);
        result = code.code;
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE, Terminal::ELSE, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}) {
        // extract if statements
        ASTNode expr = root.children.at(2);
        TypedExpr comp = visit_expr(expr, false, symbol_table, static_data);

        ASTNode thens_stmts = root.children.at(5);
        SymbolTable symbol_table_thens = symbol_table;
        auto thens = visit_stmts(
            thens_stmts,
            curr_proc,
            symbol_table_thens,
            static_data
        );

        // grab scoped variables
        std::vector<std::shared_ptr<Variable>> thens_vars;
        for (auto typed_id :
             symbol_table_sub(symbol_table_thens, symbol_table)) {
            if (auto typed_variable =
                    std::dynamic_pointer_cast<TypedVariable>(typed_id)) {
                thens_vars.push_back(typed_variable->variable);
            }
        }

        ASTNode elses_stmts = root.children.at(9);
        SymbolTable symbol_table_elses = symbol_table;
        auto elses = visit_stmts(
            elses_stmts,
            curr_proc,
            symbol_table_elses,
            static_data
        );

        // grab scoped variables
        std::vector<std::shared_ptr<Variable>> elses_vars;
        for (auto typed_id :
             symbol_table_sub(symbol_table_elses, symbol_table)) {
            if (auto typed_variable =
                    std::dynamic_pointer_cast<TypedVariable>(typed_id)) {
                elses_vars.push_back(typed_variable->variable);
            }
        }

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
            make_scope(thens_vars, thens),
            make_scope(elses_vars, elses)
        );
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}) {
        // extract if statements
        ASTNode expr = root.children.at(2);
        TypedExpr comp = visit_expr(expr, false, symbol_table, static_data);

        ASTNode thens_stmts = root.children.at(5);
        SymbolTable symbol_table_thens = symbol_table;
        auto thens = visit_stmts(
            thens_stmts,
            curr_proc,
            symbol_table_thens,
            static_data
        );

        // grab scoped variables
        std::vector<std::shared_ptr<Variable>> thens_vars;
        for (auto typed_id :
             symbol_table_sub(symbol_table_thens, symbol_table)) {
            if (auto typed_variable =
                    std::dynamic_pointer_cast<TypedVariable>(typed_id)) {
                thens_vars.push_back(typed_variable->variable);
            }
        }

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
            make_scope(thens_vars, thens)
        );
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::WHILE, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}) {
        // extract while loops
        ASTNode expr = root.children.at(2);
        TypedExpr comp = visit_expr(expr, false, symbol_table, static_data);

        ASTNode stmts = root.children.at(5);
        auto code = visit_stmts(stmts, curr_proc, symbol_table, static_data);

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
            code
        );
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::RET, NonTerminal::expr, Terminal::SEMI}) {
        // extract return statements
        ASTNode expr_node = root.children.at(1);
        TypedExpr expr =
            visit_expr(expr_node, false, symbol_table, static_data);

        if ((*curr_proc->ret_type) != (*expr.nl_type)) {
            throw TypeMismatchError(
                "Cannot return expression of type '" + expr.nl_type->to_string()
                    + "' from function with return type '"
                    + curr_proc->ret_type->to_string() + "'.",
                root.children.at(0).line_no
            );
        }
        result = std::make_shared<RetStmt>(expr.code);
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
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmts);
    std::shared_ptr<Code> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt}) {
        // extract singular statement
        ASTNode stmt = root.children.at(0);
        result = visit_stmt(stmt, curr_proc, symbol_table, static_data);
    } else if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt, NonTerminal::stmts}) {
        // extract code statement
        ASTNode stmt = root.children.at(0);
        std::shared_ptr<Code> code =
            visit_stmt(stmt, curr_proc, symbol_table, static_data);

        // extract rest of statements
        ASTNode stmts = root.children.at(1);
        std::shared_ptr<Code> rest_of_code =
            visit_stmts(stmts, curr_proc, symbol_table, static_data);
        result = make_block({code, rest_of_code});
    } else {
        std::cerr << "Invalid production found while processing stmts."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}
