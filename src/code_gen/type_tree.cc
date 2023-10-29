/*
#include "type_tree.h"

#include "state.h"
#include "ast_node.h"
#include "lang_type.h"
#include <iostream>

std::vector<ASTNode> collect(ASTNode ast_node, lhs_kind: State) {
    if (ast_node.lhs.kind == lhs_kind) {
        return {ast_node};
    }
    else {
        std::vector<ASTNode> result;
        for (auto& child : ast_node.children) {
            std::vector<ASTNode> child_result = collect(child, lhs_kind);
            result.insert(result.end(), child_result.begin(), child_result.end());
        }
        return result;
    }
}

LangType parse_type(ASTNode ast_node) {
    std::vector<ASTNode> types = collect(ast_node, "type");

    ASTNode t = types.front();
    if (t.get_production() == "type I32") {
        return I32Type{};
    }
    else {
        std::cerr << "Unsupported type!" << std::endl; 
    }
}

typedef std::map<std::string, std::shared_ptr<std:shared_ptr<LangSymbol>>> SymbolTable;
*/




