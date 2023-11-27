
#include "extract_typestmts.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <variant>

#include "ast_node.h"
#include "state.h"
#include "visit_type.h"

struct ProgramContext;

std::vector<std::pair<std::string, std::shared_ptr<NLType>>>
extract_typestmts(ASTNode root, ProgramContext& program_context) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::typestmts);
    std::vector<std::pair<std::string, std::shared_ptr<NLType>>> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {NonTerminal::typestmts, NonTerminal::typestmt}) {
        ASTNode typestmt = root.children.at(0);
        result.push_back(extract_typestmt(typestmt, program_context));
    } else if (prod == std::vector<State> {NonTerminal::typestmts, NonTerminal::typestmt, NonTerminal::typestmts}) {
        ASTNode typestmt = root.children.at(0);
        result.push_back(extract_typestmt(typestmt, program_context));

        ASTNode typestmts = root.children.at(1);
        auto child_result = extract_typestmts(typestmts, program_context);

        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while extracting typestmts."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::pair<std::string, std::shared_ptr<NLType>>
extract_typestmt(ASTNode root, ProgramContext& program_context) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::typestmt);
    std::pair<std::string, std::shared_ptr<NLType>> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::typestmt,
            Terminal::ID,
            Terminal::COLON,
            NonTerminal::type,
            Terminal::SEMI}) {
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        ASTNode type_node = root.children.at(2);
        std::shared_ptr<NLType> nl_type =
            visit_type(type_node, program_context);

        result = {name, nl_type};
    } else {
        std::cerr << "Invalid production found while extracting typestmt."
                  << std::endl;
        exit(1);
    }

    return result;
}
