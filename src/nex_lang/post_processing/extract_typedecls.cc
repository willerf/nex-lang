
#include "extract_typedecls.h"

#include <cassert>
#include <iostream>
#include <vector>

#include "extract_typestmts.h"
#include "nl_type_struct.h"
#include "state.h"
#include "visit_type.h"

void extract_typedecls(ASTNode root, ProgramContext& program_context) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::typedecls);

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::typedecls}) {
        // no more typedecls
    } else if (prod == std::vector<State> {NonTerminal::typedecls, NonTerminal::typedecl, NonTerminal::typedecls}) {
        ASTNode typedecl = root.children.at(0);
        extract_typedecl(typedecl, program_context);

        ASTNode typedecls = root.children.at(1);
        extract_typedecls(typedecls, program_context);
    } else {
        std::cerr << "Invalid production found while extracting typedecls."
                  << std::endl;
        exit(1);
    }
}

void extract_typedecl(ASTNode root, ProgramContext& program_context) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::typedecl);

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::typedecl,
            Terminal::TYPE,
            Terminal::ID,
            Terminal::ASSIGN,
            NonTerminal::type,
            Terminal::SEMI}) {
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        ASTNode type_node = root.children.at(3);
        std::shared_ptr<NLType> nl_type =
            visit_type(type_node, program_context);

        program_context.type_table[name] = nl_type;
    } else if (prod == std::vector<State> {NonTerminal::typedecl, Terminal::STRUCT, Terminal::ID, Terminal::LBRACE, NonTerminal::typestmts, Terminal::RBRACE}) {
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        ASTNode typestmts = root.children.at(3);
        auto child_result = extract_typestmts(typestmts, program_context);

        std::shared_ptr<NLType> nl_type =
            std::make_shared<NLTypeStruct>(child_result);
        program_context.type_table[name] = nl_type;
    } else {
        std::cerr << "Invalid production found while extracting typedecl."
                  << std::endl;
        exit(1);
    }
}
