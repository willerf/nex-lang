
#include "extract_fns.h"

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
#include "visit_type.h"

struct Code;
struct NLType;
struct Variable;

void extract_fns(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fns);

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn}) {
        // extract singular function
        ASTNode fn = root.children.at(0);
        extract_fn(fn, symbol_table);
    } else if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn, NonTerminal::fns}) {
        // extract code function
        ASTNode fn = root.children.at(0);
        extract_fn(fn, symbol_table);

        // extract rest of functions
        ASTNode fns = root.children.at(1);
        extract_fns(fns, symbol_table);
    } else {
        std::cerr << "Invalid production found while extracting fns."
                  << std::endl;
        exit(1);
    }
}

void extract_fn(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fn);

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
        std::shared_ptr<TypedProcedure> result =
            std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // add identifier to symbol table
        symbol_table[name] = result;

        // scope function variables
        SymbolTable symbol_table_params;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params);

        std::vector<std::shared_ptr<Variable>> params;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
        }
        result->procedure = std::make_shared<Procedure>(name, params);
        result->params = typed_params;

        // extract type information
        ASTNode ret_type = root.children.at(6);
        auto nl_type = visit_type(ret_type);
        result->ret_type = nl_type;
    } else if (prod == std::vector<State> {NonTerminal::fn, Terminal::FN, Terminal::ID, Terminal::LPAREN, NonTerminal::optparams, Terminal::RPAREN, NonTerminal::stmtblock}) {
        std::shared_ptr<TypedProcedure> result =
            std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // add identifier to symbol table
        symbol_table[name] = result;

        // scope function variables
        SymbolTable symbol_table_params;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params);

        std::vector<std::shared_ptr<Variable>> params;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
        }
        result->procedure = std::make_shared<Procedure>(name, params);
        result->params = typed_params;

        // return type of none
        result->ret_type = std::make_shared<NLTypeNone>();
    } else {
        std::cerr << "Invalid production found while extracting fn."
                  << std::endl;
        exit(1);
    }
}
