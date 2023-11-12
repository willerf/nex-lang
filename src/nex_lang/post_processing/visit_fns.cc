
#include "visit_fns.h"

#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <string>
#include <variant>

#include "nl_type_none.h"
#include "scope.h"
#include "visit_params.h"
#include "visit_stmts.h"
#include "visit_type.h"
#include "ast_node.h"
#include "procedure.h"
#include "state.h"
#include "typed_variable.h"

struct Code;
struct NLType;
struct Variable;

std::vector<std::shared_ptr<TypedProcedure>> visit_fns(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fns);
    std::vector<std::shared_ptr<TypedProcedure>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn}) {
        // extract singular function
        ASTNode fn = root.children.at(0);
        result.push_back(visit_fn(fn, symbol_table, static_data));
    } else if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn, NonTerminal::fns}) {
        // extract code function
        ASTNode fn = root.children.at(0);
        result.push_back(visit_fn(fn, symbol_table, static_data));

        // extract rest of functions
        ASTNode fns = root.children.at(1);
        std::vector<std::shared_ptr<TypedProcedure>> child_result =
            visit_fns(fns, symbol_table, static_data);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing fns."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<TypedProcedure> visit_fn(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fn);
    std::shared_ptr<TypedProcedure> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::fn,
            Terminal::FN,
            Terminal::ID,
            Terminal::LPAREN,
            NonTerminal::optparams,
            Terminal::RPAREN,
            Terminal::ARROW,
            NonTerminal::type,
            Terminal::LBRACE,
            NonTerminal::stmts,
            Terminal::RBRACE}) {
        result = std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // add identifier to symbol table
        symbol_table[name] = result;

        // make clone of symbol table to scope params
        SymbolTable symbol_table_params = symbol_table;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params);

        std::vector<std::shared_ptr<Variable>> params;
        std::vector<std::shared_ptr<NLType>> param_types;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
            param_types.push_back(typed_variable->nl_type);
        }
        result->procedure = std::make_shared<Procedure>(name, params);
        result->param_types = param_types;

        // extract type information
        ASTNode ret_type = root.children.at(6);
        auto nl_type = visit_type(ret_type);
        result->ret_type = nl_type;

        // make clone of symbol table to scope local vars
        SymbolTable symbol_table_locals = symbol_table_params;

        // extract list of statements of procedure
        ASTNode stmts = root.children.at(8);
        std::shared_ptr<Code> code =
            visit_stmts(stmts, result, symbol_table_locals, static_data);

        // only include local variables in variable chunk
        std::vector<std::shared_ptr<Variable>> vars;
        for (auto typed_id :
             symbol_table_sub(symbol_table_locals, symbol_table_params)) {
            if (auto typed_variable =
                    std::dynamic_pointer_cast<TypedVariable>(typed_id)) {
                vars.push_back(typed_variable->variable);
            }
        }
        result->procedure->code = make_scope(vars, code);
    } else if (prod == std::vector<State> {NonTerminal::fn, Terminal::FN, Terminal::ID, Terminal::LPAREN, NonTerminal::optparams, Terminal::RPAREN, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}) {
        result = std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // add identifier to symbol table
        symbol_table[name] = result;

        // make clone of symbol table to scope params
        SymbolTable symbol_table_params = symbol_table;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params);

        std::vector<std::shared_ptr<Variable>> params;
        std::vector<std::shared_ptr<NLType>> param_types;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
            param_types.push_back(typed_variable->nl_type);
        }
        result->procedure = std::make_shared<Procedure>(name, params);
        result->param_types = param_types;

        // return type of none
        result->ret_type = std::make_shared<NLTypeNone>();

        // make clone of symbol table to scope local vars
        SymbolTable symbol_table_locals = symbol_table_params;

        // extract list of statements of procedure
        ASTNode stmts = root.children.at(6);
        std::shared_ptr<Code> code =
            visit_stmts(stmts, result, symbol_table_locals, static_data);

        // only include local variables in variable chunk
        std::vector<std::shared_ptr<Variable>> vars;
        for (auto typed_id :
             symbol_table_sub(symbol_table_locals, symbol_table_params)) {
            if (auto typed_variable =
                    std::dynamic_pointer_cast<TypedVariable>(typed_id)) {
                vars.push_back(typed_variable->variable);
            }
        }
        result->procedure->code = make_scope(vars, code);
    } else {
        std::cerr << "Invalid production found while processing fn."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}
