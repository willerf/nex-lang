
#include "extract_fns.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "ast_node.h"
#include "nl_type_none.h"
#include "procedure.h"
#include "state.h"
#include "typed_procedure.h"
#include "visit_params.h"
#include "visit_type.h"

struct Variable;
struct TypedVariable;
struct NLType;
struct ProgramContext;

void extract_fns(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fns);

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn}) {
        // extract singular function
        ASTNode fn = root.children.at(0);
        extract_fn(fn, symbol_table, program_context);
    } else if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn, NonTerminal::fns}) {
        // extract code function
        ASTNode fn = root.children.at(0);
        extract_fn(fn, symbol_table, program_context);

        // extract rest of functions
        ASTNode fns = root.children.at(1);
        extract_fns(fns, symbol_table, program_context);
    } else {
        std::cerr << "Invalid production found while extracting fns."
                  << std::endl;
        exit(1);
    }
}

void extract_fn(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
) {
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

        // scope function variables
        SymbolTable symbol_table_params;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params, program_context);

        std::vector<std::shared_ptr<Variable>> params;
        std::vector<std::shared_ptr<NLType>> param_types;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
            param_types.push_back(typed_variable->nl_type);
        }

        // add identifier to symbol table
        symbol_table[{name, param_types}] = result;

        result->procedure = std::make_shared<Procedure>(name, params);
        result->params = typed_params;

        // extract type information
        ASTNode ret_type = root.children.at(6);
        auto nl_type = visit_type(ret_type, program_context);
        result->ret_type = nl_type;
    } else if (prod == std::vector<State> {NonTerminal::fn, Terminal::FN, Terminal::ID, Terminal::LPAREN, NonTerminal::optparams, Terminal::RPAREN, NonTerminal::stmtblock}) {
        std::shared_ptr<TypedProcedure> result =
            std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // scope function variables
        SymbolTable symbol_table_params;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params, program_context);

        std::vector<std::shared_ptr<Variable>> params;
        std::vector<std::shared_ptr<NLType>> param_types;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
            param_types.push_back(typed_variable->nl_type);
        }

        // add identifier to symbol table
        symbol_table[{name, param_types}] = result;

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
