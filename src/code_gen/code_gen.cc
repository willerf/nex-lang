
#include "code_gen.h"
#include "procedure.h"
#include "ast_node.h"
#include "block.h"
#include "scope.h"
#include "pseudo_assembly.h"
#include "bin_op.h"
#include "operators.h"
#include "ret_stmt.h"
#include "call.h"
#include "if_stmt.h"

#include <cassert>
#include <iostream>
#include <optional>

typedef std::map<std::string, std::shared_ptr<Variable>> VariableMap;
typedef std::map<std::string, std::shared_ptr<Procedure>> ProcedureMap;

std::vector<std::shared_ptr<Code>> visit_optargs(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map);
std::shared_ptr<Code> visit_stmts(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map);

std::shared_ptr<Code> visit_expr(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
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
        return visit_expr(expr, var_map, proc_map);
    } else if (num_children == 2) {
        ASTNode expr = root.children.at(1);
        return visit_expr(expr, var_map, proc_map);
    } else if (num_children == 3) {
        ASTNode lhs = root.children.at(0);
        ASTNode mid = root.children.at(1);
        ASTNode rhs = root.children.at(2);
        if (lhs.token.kind == "LPAREN" && rhs.token.kind == "RPAREN") {
            return visit_expr(mid, var_map, proc_map);
        }
        auto lhs_code = visit_expr(lhs, var_map, proc_map);
        auto rhs_code = visit_expr(rhs, var_map, proc_map);
        if (mid.token.kind == "PLUS") {
            return bin_op(lhs_code, op::plus(), rhs_code);
        }
        else if (mid.token.kind == "MINUS") {
            return bin_op(lhs_code, op::minus(), rhs_code);
        }
        else if (mid.token.kind == "STAR") {
            return bin_op(lhs_code, op::times(), rhs_code);
        }
        else if (mid.token.kind == "SLASH") {
            return bin_op(lhs_code, op::divide(), rhs_code);
        }
        else if (mid.token.kind == "PCT") {
            return bin_op(lhs_code, op::remainder(), rhs_code);
        }
        else if (mid.token.kind == "EQ") {
            return bin_op(lhs_code, op::eq_cmp(), rhs_code);
        }
        else if (mid.token.kind == "NE") {
            return bin_op(lhs_code, op::ne_cmp(), rhs_code);
        }
        else if (mid.token.kind == "LT") {
            return bin_op(lhs_code, op::lt_cmp(), rhs_code);
        }
        else if (mid.token.kind == "GT") {
            return bin_op(lhs_code, op::gt_cmp(), rhs_code);
        }
        else if (mid.token.kind == "LE") {
            return bin_op(lhs_code, op::le_cmp(), rhs_code);
        }
        else if (mid.token.kind == "GE") {
            return bin_op(lhs_code, op::ge_cmp(), rhs_code);
        }
        else {
            std::cerr << "Unimplemented!" << std::endl;
            exit(1);
        }
    } else if (num_children == 4) {
        ASTNode id = root.children.at(0);
        ASTNode optargs = root.children.at(2);
        std::string name = id.token.lexeme;
        
        if (proc_map.count(name)) {
            auto args = visit_optargs(optargs, var_map, proc_map);
            return make_call(proc_map[name], args);
        }
        else {
            std::cerr << "Invalid procedure name: " << name << std::endl;
            exit(1);
        }     
    }
    std::cerr << "Unreachable!" << std::endl;
    std::cerr << num_children << " " << root.token.kind << " " << root.token.lexeme << std::endl;
    exit(1);
}


std::vector<std::shared_ptr<Code>> visit_args(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(root.token.kind == "args");

    std::vector<std::shared_ptr<Code>> result;

    ASTNode expr = root.children.at(0);
    result.push_back(visit_expr(expr, var_map, proc_map));

    if (root.children.size() == 3) {
        ASTNode args = root.children.at(2);
        auto child_result = visit_args(args, var_map, proc_map);
        result.insert(result.end(), child_result.begin(), child_result.end());
    }

    return result;
}

std::vector<std::shared_ptr<Code>> visit_optargs(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(root.token.kind == "optargs");

    if (root.children.size()) {
        ASTNode args = root.children.at(0);
        return visit_args(args, var_map, proc_map);
    }

    return {};
}



std::shared_ptr<Variable> visit_vardef(ASTNode root) {
    assert(root.token.kind == "vardef");

    ASTNode id = root.children.at(0);
    std::string name = id.token.lexeme;
    return std::make_shared<Variable>(name);
}

std::shared_ptr<Code> visit_stmt(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(root.token.kind == "stmt");
    std::string prod = root.get_production();
    if (prod == "stmt LET vardef ASSIGN expr SEMI") {
        ASTNode vardef = root.children.at(1);
        ASTNode expr = root.children.at(3);
        auto var = visit_vardef(vardef);
        auto code = visit_expr(expr, var_map, proc_map);
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
            auto code = visit_expr(expr, var_map, proc_map);
            auto code_result = assign(var, code);
            return code_result;
        }
        else {
            std::cerr << "Variable does not exist: " << name << std::endl;
            exit(1);
        }
    }
    else if (prod == "stmt IF LPAREN expr RPAREN LBRACE stmts RBRACE ELSE LBRACE stmts RBRACE") {
        ASTNode expr = root.children.at(2);
        ASTNode thens_stmts = root.children.at(5);
        ASTNode elses_stmts = root.children.at(9);

        auto comp = visit_expr(expr, var_map, proc_map);
        auto thens = visit_stmts(thens_stmts, var_map, proc_map);
        auto elses = visit_stmts(elses_stmts, var_map, proc_map);

        return make_if(
            comp,
            op::ne_cmp(),
            make_add(Reg::Result, Reg::Zero, Reg::Zero),
            thens,
            elses
        );
    }
    else if (prod == "stmt RET expr SEMI") {
        ASTNode expr = root.children.at(1);
        return std::make_shared<RetStmt>(visit_expr(expr, var_map, proc_map));
    }
    else {
        std::cerr << "Invalid stmt!" << std::endl;
        exit(1);
    }

    return nullptr;
}

std::shared_ptr<Code> visit_stmts(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(root.token.kind == "stmts");

    std::vector<std::shared_ptr<Code>> all_code;

    ASTNode stmt = root.children.at(0);
    auto code1 = visit_stmt(stmt, var_map, proc_map);
    all_code.push_back(code1);
    if (root.children.size() == 2) {
        ASTNode stmts = root.children.at(1);
        auto code2 = visit_stmts(stmts, var_map, proc_map);
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
std::shared_ptr<Procedure> visit_fn(ASTNode root, ProcedureMap& proc_map) {

    assert(root.token.kind == "fn");

    ASTNode id = root.children.at(1);
    std::string name = id.token.lexeme;

    ASTNode optparams = root.children.at(3);
    auto params = visit_optparams(optparams);

    std::shared_ptr<Procedure> proc = std::make_shared<Procedure>(name, params);
    proc_map[name] = proc;

    ASTNode stmts = root.children.at(8);
    VariableMap var_map;
    for (auto param : params) {
        var_map[param->name] = param;
    }
    auto code = visit_stmts(stmts, var_map, proc_map);
    std::vector<std::shared_ptr<Variable>> vars;
    for (auto& [key, value] : var_map) {
        if (std::find(params.begin(), params.end(), value) == params.end()) {
            vars.push_back(value);
        }
    }

    proc->code = make_scope(vars, code);

    return proc;
}

std::vector<std::shared_ptr<Procedure>> visit_fns(ASTNode root, ProcedureMap& proc_map) {

    assert(root.token.kind == "fns");

    std::vector<std::shared_ptr<Procedure>> result;

    ASTNode fn = root.children.at(0);
    result.push_back(visit_fn(fn, proc_map));
    if (root.children.size() == 2) {
        ASTNode fns = root.children.at(1);
        auto child_result = visit_fns(fns, proc_map);
        result.insert(result.end(), child_result.begin(), child_result.end());
    }
    
    return result;
}

std::vector<std::shared_ptr<Procedure>> generate(ASTNode root) {

    assert(root.get_production() == "s BOF fns EOF");
    
    ProcedureMap proc_map;
    ASTNode fns = root.children.at(1);

    return visit_fns(fns, proc_map);
}








