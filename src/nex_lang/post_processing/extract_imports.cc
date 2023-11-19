
#include "extract_imports.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "ast_node.h"
#include "state.h"

std::vector<std::string>
extract_imports(ASTNode root, ModuleTable& module_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::imports);
    std::vector<std::string> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::imports}) {
        // No more imports
    } else if (prod == std::vector<State> {NonTerminal::imports, NonTerminal::import, NonTerminal::imports}) {
        ASTNode import = root.children.at(0);
        result = extract_import(import, module_table);

        ASTNode imports = root.children.at(1);
        std::vector<std::string> child_result =
            extract_imports(imports, module_table);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while extracting imports."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::string>
extract_import(ASTNode root, ModuleTable& module_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::import);
    std::vector<std::string> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::import,
            Terminal::IMPORT,
            Terminal::ID,
            Terminal::SEMI}) {
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        result.push_back(name);
    } else {
        std::cerr << "Invalid production found while extracting import."
                  << std::endl;
        exit(1);
    }

    return result;
}
