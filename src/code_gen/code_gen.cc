
#include "code_gen.h"
#include "procedure.h"
#include "ast_node.h"
#include "block.h"
#include "scope.h"
#include "pseudo_assembly.h"
#include "bin_op.h"
#include "operators.h"

#include <cassert>
#include <iostream>
#include <optional>

std::shared_ptr<Code> visit_expr(ASTNode root, std::map<std::string, std::shared_ptr<Variable>>& var_map) {
    size_t num_children = root.children.size();
    
    if (num_children == 0) {
        if (root.token.kind == "ID") {
            std::string name = root.token.lexeme;
            if (var_map.count(name)) {
                return var_map[name]->to_expr();
            }
            else {
                std::cerr << "Invalid variable name: " << name << std::endl;
                exit(1);
            }
        }
        else if (root.token.kind == "NUM") {
            return int_literal(stoi(root.token.lexeme));
        }
    }   
    if (num_children == 1) {
        ASTNode expr = root.children.at(0);
        return visit_expr(expr, var_map);
    } else if (num_children == 2) {
        ASTNode expr = root.children.at(1);
        return visit_expr(expr, var_map);
    } else if (num_children == 3) {
        ASTNode lhs = root.children.at(0);
        ASTNode mid = root.children.at(1);
        ASTNode rhs = root.children.at(2);
        if (lhs.token.kind == "LPAREN" && rhs.token.kind == "RPAREN") {
            return visit_expr(mid, var_map);
        }
        auto lhs_code = visit_expr(lhs, var_map);
        auto rhs_code = visit_expr(rhs, var_map);
        if (mid.token.kind == "PLUS") {
            return bin_op(lhs_code, op::plus(), rhs_code);
        }
        else {
            std::cerr << "Unimplemented!" << std::endl;
            exit(1);
        }
    } else if (num_children == 4) {
        return nullptr;
    }
    std::cerr << "Unreachable!" << std::endl;
    std::cerr << num_children << " " << root.token.kind << " " << root.token.lexeme << std::endl;
    exit(1);
}

std::shared_ptr<Variable> visit_vardef(ASTNode root) {
    assert(root.token.kind == "vardef");

    ASTNode id = root.children.at(0);
    std::string name = id.token.lexeme;
    return std::make_shared<Variable>(name);
}

std::shared_ptr<Code> visit_stmt(ASTNode root, std::map<std::string, std::shared_ptr<Variable>>& var_map) {
    assert(root.token.kind == "stmt");
    std::string prod = root.get_production();
    if (prod == "stmt LET vardef ASSIGN expr SEMI") {
        ASTNode vardef = root.children.at(1);
        ASTNode expr = root.children.at(3);
        auto var = visit_vardef(vardef);
        auto code = visit_expr(expr, var_map);
        if (var_map.count(var->name)) {
            std::cerr << "Duplicate variable name!" << std::endl;
        }
        var_map[var->name] = var;
        auto code_result = assign(var, code);
        return code_result;
    }
    else if (prod == "stmt ID ASSIGN expr SEMI") {
        ASTNode id = root.children.at(0);
        ASTNode expr = root.children.at(2);
        std::string name = id.token.lexeme;
        if (var_map.count(name)) {
            auto var = var_map[name];
            auto code = visit_expr(expr, var_map);
            auto code_result = assign(var, code);
            return code_result;
        }
        else {
            std::cerr << "Variable does not exist: " << name << std::endl;
            exit(1);
        }
    }
    else if (prod == "stmt IF LPAREN expr RPAREN LBRACE stmts RBRACE ELSE LBRACE stmts RBRACE") {

    }
    else if (prod == "stmt RET expr SEMI") {

    }
    else {
        std::cerr << "Invalid stmt!" << std::endl;
        exit(1);
    }

    return nullptr;
}

std::shared_ptr<Code> visit_stmts(ASTNode root, std::map<std::string, std::shared_ptr<Variable>>& var_map) {
    assert(root.token.kind == "stmts");

    std::vector<std::shared_ptr<Code>> all_code;

    ASTNode stmt = root.children.at(0);
    auto code1 = visit_stmt(stmt, var_map);
    all_code.push_back(code1);
    if (root.children.size() == 2) {
        ASTNode stmts = root.children.at(1);
        auto code2 = visit_stmts(stmts, var_map);
        all_code.push_back(code2);
    }

    return make_block(all_code);
}


std::vector<std::shared_ptr<Variable>> visit_params(ASTNode root) {
    assert(root.token.kind == "params");
    std::vector<std::shared_ptr<Variable>> result;

    ASTNode vardef = root.children.at(0);
    result.push_back(visit_vardef(vardef));

    if (root.children.size() == 3) {
        ASTNode params = root.children.at(2);
        auto child_result = visit_params(params);
        result.insert(result.end(), child_result.begin(), child_result.end());
    }

    return result;
}

std::vector<std::shared_ptr<Variable>> visit_optparams(ASTNode root) {
    assert(root.token.kind == "optparams");

    if (root.children.size()) {
        ASTNode params = root.children.at(0);
        return visit_params(params);
    }

    return {};
}
std::shared_ptr<Procedure> visit_fn(ASTNode root) {

    assert(root.token.kind == "fn");

    ASTNode id = root.children.at(1);
    std::string name = id.token.lexeme;

    ASTNode optparams = root.children.at(3);
    auto params = visit_optparams(optparams);

    ASTNode stmts = root.children.at(8);
    std::map<std::string, std::shared_ptr<Variable>> var_map;
    for (auto param : params) {
        var_map[param->name] = param;
    }
    auto code = visit_stmts(stmts, var_map);
    std::vector<std::shared_ptr<Variable>> vars;
    for (auto& [key, value] : var_map) {
        vars.push_back(value);
    }

    std::shared_ptr<Procedure> proc = std::make_shared<Procedure>(name, params);
    proc->code = make_scope(vars, code);

    return proc;
}

std::vector<std::shared_ptr<Procedure>> visit_fns(ASTNode root) {

    assert(root.token.kind == "fns");

    std::vector<std::shared_ptr<Procedure>> result;

    ASTNode fn = root.children.at(0);
    result.push_back(visit_fn(fn));
    if (root.children.size() == 2) {
        ASTNode fns = root.children.at(1);
        auto child_result = visit_fns(fns);
        result.insert(result.end(), child_result.begin(), child_result.end());
    }
    
    return result;
}

std::vector<std::shared_ptr<Procedure>> generate(ASTNode root) {

    assert(root.get_production() == "s BOF fns EOF");

    ASTNode fns = root.children.at(1);

    return visit_fns(fns);
}








