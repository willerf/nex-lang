
#include "post_processing.h"

#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <set>

#include "ast_node.h"
#include "bin_op.h"
#include "block.h"
#include "call.h"
#include "if_stmt.h"
#include "operators.h"
#include "procedure.h"
#include "pseudo_assembly.h"
#include "ret_stmt.h"
#include "scope.h"

static std::set<NonTerminal> expr_non_terminals = {
    NonTerminal::expr,
    NonTerminal::exprp1,
    NonTerminal::exprp2,
    NonTerminal::exprp3,
    NonTerminal::exprp4,
    NonTerminal::exprp5,
    NonTerminal::exprp6,
    NonTerminal::exprp7,
    NonTerminal::exprp8};

std::shared_ptr<Code>
visit_expr(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(
        std::holds_alternative<NonTerminal>(root.state)
        && expr_non_terminals.count(std::get<NonTerminal>(root.state))
    );
    std::shared_ptr<Code> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::ID}) {
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        if (var_map.count(name)) {
            result = var_map[name]->to_expr();
        } else {
            std::cerr << "Variable does not exist: " << name << std::endl;
            exit(1);
        }
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::NUM}) {
        ASTNode num = root.children.at(0);
        result = int_literal(stoi(num.lexeme));
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN}) {
        ASTNode expr = root.children.at(1);
        result = visit_expr(expr, var_map, proc_map);
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::ID, Terminal::LPAREN, NonTerminal::optargs, Terminal::RPAREN}) {
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        if (proc_map.count(name)) {
            ASTNode optargs = root.children.at(2);
            std::vector<std::shared_ptr<Code>> args =
                visit_optargs(optargs, var_map, proc_map);
            result = make_call(proc_map[name], args);
        } else {
            std::cerr << "Procedure does not exist: " << name << std::endl;
            exit(1);
        }
    }
    else if (root.children.size() == 1 && std::holds_alternative<NonTerminal>(root.children.at(0).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(0).state))) {
        // recursively call into next operator precedence layer
        ASTNode expr = root.children.at(0);
        result = visit_expr(expr, var_map, proc_map);
    }
    else if (root.children.size() == 2 && std::holds_alternative<NonTerminal>(root.children.at(1).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(1).state))) {
        // extract unary operator
    }
    else if (root.children.size() == 3 && std::holds_alternative<NonTerminal>(root.children.at(0).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(0).state)) && std::holds_alternative<Terminal>(root.children.at(1).state) && std::holds_alternative<NonTerminal>(root.children.at(2).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(2).state))) {
        // extract binary operator
        ASTNode lhs = root.children.at(0);
        ASTNode mid = root.children.at(1);
        ASTNode rhs = root.children.at(2);

        std::shared_ptr<Code> lhs_code = visit_expr(lhs, var_map, proc_map);
        Terminal mid_op = std::get<Terminal>(mid.state);
        std::shared_ptr<Code> rhs_code = visit_expr(rhs, var_map, proc_map);
        switch (mid_op) {
            case Terminal::OR:
                result = bin_op(lhs_code, op::or_bool(), rhs_code);
                break;
            case Terminal::AND:
                result = bin_op(lhs_code, op::and_bool(), rhs_code);
                break;
            case Terminal::PLUS:
                result = bin_op(lhs_code, op::plus(), rhs_code);
                break;
            case Terminal::MINUS:
                result = bin_op(lhs_code, op::minus(), rhs_code);
                break;
            case Terminal::STAR:
                result = bin_op(lhs_code, op::times(), rhs_code);
                break;
            case Terminal::SLASH:
                result = bin_op(lhs_code, op::divide(), rhs_code);
                break;
            case Terminal::PCT:
                result = bin_op(lhs_code, op::remainder(), rhs_code);
                break;
            case Terminal::EQ:
                result = bin_op(lhs_code, op::eq_cmp(), rhs_code);
                break;
            case Terminal::NE:
                result = bin_op(lhs_code, op::ne_cmp(), rhs_code);
                break;
            case Terminal::LT:
                result = bin_op(lhs_code, op::lt_cmp(), rhs_code);
                break;
            case Terminal::GT:
                result = bin_op(lhs_code, op::gt_cmp(), rhs_code);
                break;
            case Terminal::LE:
                result = bin_op(lhs_code, op::le_cmp(), rhs_code);
                break;
            case Terminal::GE:
                result = bin_op(lhs_code, op::ge_cmp(), rhs_code);
                break;
            default:
                std::cerr
                    << "Unimplemented operator found while processing expr."
                    << std::endl;
                exit(1);
        }
    } else {
        std::cerr << "Invalid production found while processing expr."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<Code>>
visit_args(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::args);
    std::vector<std::shared_ptr<Code>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::args, NonTerminal::expr}) {
        // extract singular argument
        ASTNode expr = root.children.at(0);
        result.push_back(visit_expr(expr, var_map, proc_map));
    } else if (prod == std::vector<State> {NonTerminal::args, NonTerminal::expr, Terminal::COMMA, NonTerminal::args}) {
        // extract first argument
        ASTNode expr = root.children.at(0);
        result.push_back(visit_expr(expr, var_map, proc_map));

        // extract rest of arguments
        ASTNode args = root.children.at(2);
        auto child_result = visit_args(args, var_map, proc_map);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing args."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<Code>>
visit_optargs(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::optargs);
    std::vector<std::shared_ptr<Code>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::optargs}) {
        // function call passing in no arguments
    } else if (prod == std::vector<State> {NonTerminal::optargs, NonTerminal::args}) {
        // extract arguments
        ASTNode args = root.children.at(0);
        result = visit_args(args, var_map, proc_map);
    } else {
        std::cerr << "Invalid production found while processing optargs."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<Variable> visit_vardef(ASTNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::vardef);
    std::shared_ptr<Variable> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::vardef,
            Terminal::ID,
            Terminal::COLON,
            NonTerminal::type}) {
        // extract variable definition
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;
        result = std::make_shared<Variable>(name);
    } else {
        std::cerr << "Invalid production found while processing vardef."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<Code>
visit_stmt(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmt);
    std::shared_ptr<Code> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::stmt,
            Terminal::LET,
            NonTerminal::vardef,
            Terminal::ASSIGN,
            NonTerminal::expr,
            Terminal::SEMI}) {
        // extract variable declaration and assignment
        ASTNode vardef = root.children.at(1);
        auto var = visit_vardef(vardef);

        ASTNode expr = root.children.at(3);
        auto code = visit_expr(expr, var_map, proc_map);
        if (var_map.count(var->name)) {
            std::cerr << "Duplicate variable name: " << var->name << std::endl;
            exit(1);
        } else {
            var_map[var->name] = var;
            result = assign(var, code);
        }
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::ID, Terminal::ASSIGN, NonTerminal::expr, Terminal::SEMI}) {
        // extract variable assignment
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        if (var_map.count(name)) {
            std::shared_ptr<Variable> var = var_map[name];
            ASTNode expr = root.children.at(2);
            std::shared_ptr<Code> code = visit_expr(expr, var_map, proc_map);
            result = assign(var, code);
        } else {
            std::cerr << "Variable does not exist: " << name << std::endl;
            exit(1);
        }
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE, Terminal::ELSE, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}) {
        // extract if statements
        ASTNode expr = root.children.at(2);
        auto comp = visit_expr(expr, var_map, proc_map);

        ASTNode thens_stmts = root.children.at(5);
        auto thens = visit_stmts(thens_stmts, var_map, proc_map);

        ASTNode elses_stmts = root.children.at(9);
        auto elses = visit_stmts(elses_stmts, var_map, proc_map);

        result = make_if(
            comp,
            op::ne_cmp(),
            make_add(Reg::Result, Reg::Zero, Reg::Zero),
            thens,
            elses
        );
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::RET, NonTerminal::expr, Terminal::SEMI}) {
        // extract return statements
        ASTNode expr = root.children.at(1);
        result = std::make_shared<RetStmt>(visit_expr(expr, var_map, proc_map));
    } else {
        std::cerr << "Invalid production found while processing stmt."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<Code>
visit_stmts(ASTNode root, VariableMap& var_map, ProcedureMap& proc_map) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmts);
    std::shared_ptr<Code> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt}) {
        // extract singular statement
        ASTNode stmt = root.children.at(0);
        result = visit_stmt(stmt, var_map, proc_map);
    } else if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt, NonTerminal::stmts}) {
        // extract first statement
        ASTNode stmt = root.children.at(0);
        std::shared_ptr<Code> code = visit_stmt(stmt, var_map, proc_map);

        // extract rest of statements
        ASTNode stmts = root.children.at(1);
        std::shared_ptr<Code> rest_of_code =
            visit_stmts(stmts, var_map, proc_map);
        result = make_block({code, rest_of_code});
    } else {
        std::cerr << "Invalid production found while processing stmts."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<Variable>> visit_params(ASTNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::params);
    std::vector<std::shared_ptr<Variable>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::params, NonTerminal::vardef}) {
        // extract singular parameter
        ASTNode vardef = root.children.at(0);
        result.push_back(visit_vardef(vardef));
    } else if (prod == std::vector<State> {NonTerminal::params, NonTerminal::vardef, Terminal::COMMA, NonTerminal::params}) {
        // extract first parameter
        ASTNode vardef = root.children.at(0);
        result.push_back(visit_vardef(vardef));

        // extract rest of parameters
        ASTNode params = root.children.at(2);
        std::vector<std::shared_ptr<Variable>> child_result =
            visit_params(params);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing params."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<Variable>> visit_optparams(ASTNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::optparams);
    std::vector<std::shared_ptr<Variable>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::optparams}) {
        // function has no parameters
    } else if (prod == std::vector<State> {NonTerminal::optparams, NonTerminal::params}) {
        // extract parameters
        ASTNode params = root.children.at(0);
        result = visit_params(params);
    } else {
        std::cerr << "Invalid production found while processing optparams."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<Procedure> visit_fn(ASTNode root, ProcedureMap& proc_map) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fn);
    std::shared_ptr<Procedure> result;

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
            Terminal::LBRACE,
            NonTerminal::stmts,
            Terminal::RBRACE}) {
        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<Variable>> params =
            visit_optparams(optparams);

        result = std::make_shared<Procedure>(name, params);
        proc_map[name] = result;

        // TODO: extract type information

        // set up variable map with parameter variables
        VariableMap var_map;
        for (auto param : params) {
            var_map[param->name] = param;
        }

        // extract list of statements of procedure
        ASTNode stmts = root.children.at(8);
        std::shared_ptr<Code> code = visit_stmts(stmts, var_map, proc_map);

        // only include local variables in variable chunk
        std::vector<std::shared_ptr<Variable>> vars;
        for (auto& [key, value] : var_map) {
            if (std::find(params.begin(), params.end(), value)
                == params.end()) {
                vars.push_back(value);
            }
        }

        result->code = make_scope(vars, code);
    } else {
        std::cerr << "Invalid production found while processing fn."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<Procedure>>
visit_fns(ASTNode root, ProcedureMap& proc_map) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fns);
    std::vector<std::shared_ptr<Procedure>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn}) {
        // extract singular function
        ASTNode fn = root.children.at(0);
        result.push_back(visit_fn(fn, proc_map));
    } else if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn, NonTerminal::fns}) {
        // extract first function
        ASTNode fn = root.children.at(0);
        result.push_back(visit_fn(fn, proc_map));

        // extract rest of functions
        ASTNode fns = root.children.at(1);
        std::vector<std::shared_ptr<Procedure>> child_result =
            visit_fns(fns, proc_map);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing fns."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<Procedure>> generate(ASTNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::s);
    std::vector<std::shared_ptr<Procedure>> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::s,
            Terminal::BOFS,
            NonTerminal::fns,
            Terminal::EOFS}) {
        // set up procedure map
        ProcedureMap proc_map;

        // extract functions of program
        ASTNode fns = root.children.at(1);
        result = visit_fns(fns, proc_map);
    } else {
        std::cerr << "Invalid production found while processing s."
                  << std::endl;
        exit(1);
    };

    return result;
}
