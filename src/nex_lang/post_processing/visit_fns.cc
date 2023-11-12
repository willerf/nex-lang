
#include "visit_fns.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

#include "ast_node.h"
#include "nl_type_none.h"
#include "procedure.h"
#include "scope.h"
#include "state.h"
#include "typed_variable.h"
#include "visit_params.h"
#include "visit_stmts.h"
#include "visit_type.h"

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
            NonTerminal::stmtblock,
        }) {
        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        if (auto typed_proc =
                std::dynamic_pointer_cast<TypedProcedure>(symbol_table.at(name)
                )) {
            result = typed_proc;
        } else {
            std::cerr << "Missing name from symbol extraction." << std::endl;
            exit(1);
        }

        // make clone of symbol table to scope params
        SymbolTable symbol_table_params = symbol_table;
        for (auto typed_var : result->params) {
            symbol_table_params[typed_var->variable->name] = typed_var;
        }

        ASTNode stmtblock = root.children.at(7);
        auto code = visit_stmtblock(
            stmtblock,
            result,
            symbol_table_params,
            static_data
        );

        result->procedure->code = code;
    } else if (prod == std::vector<State> {NonTerminal::fn, Terminal::FN, Terminal::ID, Terminal::LPAREN, NonTerminal::optparams, Terminal::RPAREN, NonTerminal::stmtblock}) {
        result = std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // add identifier to symbol table
        symbol_table[name] = result;

        // make clone of symbol table to scope params
        SymbolTable symbol_table_params = symbol_table;
        for (auto typed_var : result->params) {
            symbol_table_params[typed_var->variable->name] = typed_var;
        }

        ASTNode stmtblock = root.children.at(5);
        auto code = visit_stmtblock(
            stmtblock,
            result,
            symbol_table_params,
            static_data
        );

        result->procedure->code = code;
    } else {
        std::cerr << "Invalid production found while processing fn."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}
