
#include "visit_type.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

#include "ast_node.h"
#include "compile_error.h"
#include "nl_type_bool.h"
#include "nl_type_char.h"
#include "nl_type_i32.h"
#include "nl_type_none.h"
#include "nl_type_ptr.h"
#include "program_context.h"
#include "state.h"

std::shared_ptr<NLType>
visit_type(ASTNode root, ProgramContext& program_context) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::type);
    std::shared_ptr<NLType> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::type, Terminal::I32}) {
        result = std::make_shared<NLTypeI32>();
    } else if (prod == std::vector<State> {NonTerminal::type, Terminal::BOOL}) {
        result = std::make_shared<NLTypeBool>();
    } else if (prod == std::vector<State> {NonTerminal::type, Terminal::CHAR}) {
        result = std::make_shared<NLTypeChar>();
    } else if (prod == std::vector<State> {NonTerminal::type, Terminal::NONE}) {
        result = std::make_shared<NLTypeNone>();
    } else if (prod == std::vector<State> {NonTerminal::type, Terminal::ID}) {
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        if (program_context.type_table.contains(name)) {
            result = program_context.type_table.at(name);
        } else {
            throw CompileError("Unknown type: " + name, id.line_no);
        }
    } else if (prod == std::vector<State> {NonTerminal::type, Terminal::STAR, NonTerminal::type}) {
        ASTNode sub_type = root.children.at(1);
        std::shared_ptr<NLType> sub_nl_type =
            visit_type(sub_type, program_context);
        result = std::make_shared<NLTypePtr>(sub_nl_type);
    } else if (prod == std::vector<State> {NonTerminal::type, Terminal::LPAREN, NonTerminal::type, Terminal::RPAREN}) {
        ASTNode type_node = root.children.at(1);
        result = visit_type(type_node, program_context);
    } else {
        std::cerr << "Invalid production found while processing type."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}
