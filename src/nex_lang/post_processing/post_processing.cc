
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
#include "duplicate_symbol_error.h"
#include "if_stmt.h"
#include "nl_type.h"
#include "operators.h"
#include "procedure.h"
#include "pseudo_assembly.h"
#include "ret_stmt.h"
#include "scope.h"
#include "symbol_not_found_error.h"
#include "type_mismatch_error.h"
#include "var_access.h"
#include "while_loop.h"

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

TypedExpr
visit_expr(ASTNode root, bool read_address, SymbolTable& symbol_table) {
    assert(
        std::holds_alternative<NonTerminal>(root.state)
        && expr_non_terminals.count(std::get<NonTerminal>(root.state))
    );
    TypedExpr result = TypedExpr {nullptr, nullptr};

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::ID}) {
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        if (symbol_table.count(name)) {
            if (auto typed_var =
                    std::dynamic_pointer_cast<TypedVariable>(symbol_table[name]
                    )) {
                result = TypedExpr {
                    typed_var->variable->to_expr(read_address),
                    typed_var->nl_type};
            } else {
                throw SymbolNotFoundError(name);
            }
        } else {
            throw SymbolNotFoundError(name);
        }
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::NUM}) {
        ASTNode num = root.children.at(0);
        result = TypedExpr {
            int_literal(stoi(num.lexeme)),
            std::make_shared<NLTypeI32>()};
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::TRUE}) {
        ASTNode expr = root.children.at(0);
        result = TypedExpr {int_literal(1), std::make_shared<NLTypeBool>()};
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::FALSE}) {
        ASTNode num = root.children.at(0);
        result = TypedExpr {int_literal(0), std::make_shared<NLTypeBool>()};
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::AMPERSAND, Terminal::ID}) {
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;
        if (symbol_table.count(name)) {
            if (auto typed_var =
                    std::dynamic_pointer_cast<TypedVariable>(symbol_table[name]
                    )) {
                result = TypedExpr {
                    typed_var->variable->to_expr(true),
                    std::make_shared<NLTypePtr>(typed_var->nl_type)};
            } else {
                throw SymbolNotFoundError(name);
            }
        } else {
            throw SymbolNotFoundError(name);
        }
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN}) {
        ASTNode expr = root.children.at(1);
        result = visit_expr(expr, read_address, symbol_table);
    } else if (prod == std::vector<State> {NonTerminal::exprp8, Terminal::ID, Terminal::LPAREN, NonTerminal::optargs, Terminal::RPAREN}) {
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        if (symbol_table.count(name)) {
            if (auto typed_procedure =
                    std::dynamic_pointer_cast<TypedProcedure>(symbol_table[name]
                    )) {
                ASTNode optargs = root.children.at(2);
                std::vector<TypedExpr> typed_args =
                    visit_optargs(optargs, symbol_table);
                std::vector<std::shared_ptr<Code>> args;
                for (auto typed_arg : typed_args) {
                    args.push_back(typed_arg.first);
                }
                result = TypedExpr {
                    make_call(typed_procedure->procedure, args),
                    typed_procedure->ret_type};
            } else {
                throw SymbolNotFoundError(name);
            }
        } else {
            throw SymbolNotFoundError(name);
        }
    }
    else if (root.children.size() == 1 && std::holds_alternative<NonTerminal>(root.children.at(0).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(0).state))) {
        // recursively call into next operator precedence layer
        ASTNode expr = root.children.at(0);
        result = visit_expr(expr, read_address, symbol_table);
    }
    else if (root.children.size() == 2 && std::holds_alternative<Terminal>(root.children.at(0).state) && std::holds_alternative<NonTerminal>(root.children.at(1).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(1).state))) {
        // extract unary operator
        ASTNode lhs_op = root.children.at(0);
        ASTNode expr = root.children.at(1);

        Terminal unary_op = std::get<Terminal>(lhs_op.state);
        TypedExpr expr_code = visit_expr(expr, read_address, symbol_table);
        switch (unary_op) {
            case Terminal::NOT:
                if ((*expr_code.second) != NLTypeBool()) {
                    throw TypeMismatchError(
                        "Boolean operations require operands to be of type bool."
                    );
                }
                result = TypedExpr {
                    make_block({expr_code.first, op::not_bool()}),
                    expr_code.second};
                break;
            case Terminal::STAR:
                if (auto expr_type =
                        std::dynamic_pointer_cast<NLTypePtr>(expr_code.second
                        )) {
                    result =
                        TypedExpr {deref(expr_code.first), expr_type->nl_type};
                } else {
                    throw TypeMismatchError(
                        "Dereference operations require operand to be of type pointer."
                    );
                }
                break;
            default:
                std::cerr
                    << "Unimplemented unary operator found while processing expr."
                    << std::endl;
                exit(1);
        }
    }
    else if (root.children.size() == 3 && std::holds_alternative<NonTerminal>(root.children.at(0).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(0).state)) && std::holds_alternative<Terminal>(root.children.at(1).state) && std::holds_alternative<NonTerminal>(root.children.at(2).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(2).state))) {
        // extract binary operator
        ASTNode lhs = root.children.at(0);
        ASTNode mid = root.children.at(1);
        ASTNode rhs = root.children.at(2);

        TypedExpr typed_lhs_code = visit_expr(lhs, read_address, symbol_table);
        Terminal mid_op = std::get<Terminal>(mid.state);
        TypedExpr typed_rhs_code = visit_expr(rhs, read_address, symbol_table);

        std::shared_ptr<Code> lhs_code = typed_lhs_code.first;
        std::shared_ptr<Code> rhs_code = typed_rhs_code.first;

        std::shared_ptr<NLType> lhs_type = typed_lhs_code.second;
        std::shared_ptr<NLType> rhs_type = typed_rhs_code.second;

        std::shared_ptr<NLType> result_type;
        if (mid_op == Terminal::OR || mid_op == Terminal::AND) {
            if ((*lhs_type) != NLTypeBool() || (*rhs_type) != NLTypeBool()) {
                throw TypeMismatchError(
                    "Boolean operations require operands to be of type bool."
                );
            }
            result_type = std::make_shared<NLTypeBool>();
        } else if (mid_op == Terminal::PLUS || mid_op == Terminal::MINUS || mid_op == Terminal::STAR || mid_op == Terminal::SLASH || mid_op == Terminal::PCT) {
            if ((*lhs_type) != NLTypeI32() || (*rhs_type) != NLTypeI32()) {
                throw TypeMismatchError(
                    "Arithmetic operations require both operands to be of type NLTypeI32."
                );
            }
            result_type = std::make_shared<NLTypeI32>();
        } else if (mid_op == Terminal::EQ || mid_op == Terminal::NE || mid_op == Terminal::LT || mid_op == Terminal::GT || mid_op == Terminal::LE || mid_op == Terminal::GE) {
            if ((*lhs_type) != (*rhs_type)) {
                throw TypeMismatchError(
                    "Comparison operations require operands to be of the same type."
                );
            }
            result_type = std::make_shared<NLTypeBool>();
        } else {
            throw TypeMismatchError(
                "Encountered unknown operation during type checking."
            );
        }

        switch (mid_op) {
            case Terminal::OR:
                result = TypedExpr {
                    bin_op(lhs_code, op::or_bool(), rhs_code),
                    result_type};
                break;
            case Terminal::AND:
                result = TypedExpr {
                    bin_op(lhs_code, op::and_bool(), rhs_code),
                    result_type};
                break;
            case Terminal::PLUS:
                result = TypedExpr {
                    bin_op(lhs_code, op::plus(), rhs_code),
                    result_type};
                break;
            case Terminal::MINUS:
                result = TypedExpr {
                    bin_op(lhs_code, op::minus(), rhs_code),
                    result_type};
                break;
            case Terminal::STAR:
                result = TypedExpr {
                    bin_op(lhs_code, op::times(), rhs_code),
                    result_type};
                break;
            case Terminal::SLASH:
                result = TypedExpr {
                    bin_op(lhs_code, op::divide(), rhs_code),
                    result_type};
                break;
            case Terminal::PCT:
                result = TypedExpr {
                    bin_op(lhs_code, op::remainder(), rhs_code),
                    result_type};
                break;
            case Terminal::EQ:
                result = TypedExpr {
                    bin_op(lhs_code, op::eq_cmp(), rhs_code),
                    result_type};
                break;
            case Terminal::NE:
                result = TypedExpr {
                    bin_op(lhs_code, op::ne_cmp(), rhs_code),
                    result_type};
                break;
            case Terminal::LT:
                result = TypedExpr {
                    bin_op(lhs_code, op::lt_cmp(), rhs_code),
                    result_type};
                break;
            case Terminal::GT:
                result = TypedExpr {
                    bin_op(lhs_code, op::gt_cmp(), rhs_code),
                    result_type};
                break;
            case Terminal::LE:
                result = TypedExpr {
                    bin_op(lhs_code, op::le_cmp(), rhs_code),
                    result_type};
                break;
            case Terminal::GE:
                result = TypedExpr {
                    bin_op(lhs_code, op::ge_cmp(), rhs_code),
                    result_type};
                break;
            default:
                std::cerr
                    << "Unimplemented binary operator found while processing expr."
                    << std::endl;
                exit(1);
        }
    } else {
        std::cerr << "Invalid production found while processing expr."
                  << std::endl;
        exit(1);
    }

    assert(result.first);
    assert(result.second);
    return result;
}

std::vector<TypedExpr> visit_args(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::args);
    std::vector<TypedExpr> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::args, NonTerminal::expr}) {
        // extract singular argument
        ASTNode expr = root.children.at(0);
        result.push_back(visit_expr(expr, false, symbol_table));
    } else if (prod == std::vector<State> {NonTerminal::args, NonTerminal::expr, Terminal::COMMA, NonTerminal::args}) {
        // extract first argument
        ASTNode expr = root.children.at(0);
        result.push_back(visit_expr(expr, false, symbol_table));

        // extract rest of arguments
        ASTNode args = root.children.at(2);
        auto child_result = visit_args(args, symbol_table);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing args."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<TypedExpr> visit_optargs(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::optargs);
    std::vector<TypedExpr> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::optargs}) {
        // function call passing in no arguments
    } else if (prod == std::vector<State> {NonTerminal::optargs, NonTerminal::args}) {
        // extract arguments
        ASTNode args = root.children.at(0);
        result = visit_args(args, symbol_table);
    } else {
        std::cerr << "Invalid production found while processing optargs."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<NLType> visit_type(ASTNode root) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::type);
    std::shared_ptr<NLType> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::type, Terminal::I32}) {
        result = std::make_shared<NLTypeI32>();
    } else if (prod == std::vector<State> {NonTerminal::type, Terminal::BOOL}) {
        result = std::make_shared<NLTypeBool>();
    } else if (prod == std::vector<State> {NonTerminal::type, Terminal::STAR, NonTerminal::type}) {
        ASTNode sub_type = root.children.at(1);
        std::shared_ptr<NLType> sub_nl_type = visit_type(sub_type);
        result = std::make_shared<NLTypePtr>(sub_nl_type);
    } else {
        std::cerr << "Invalid production found while processing type."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}

std::shared_ptr<TypedVariable>
visit_vardef(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::vardef);
    std::shared_ptr<TypedVariable> result = nullptr;

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

        if (symbol_table.count(name)) {
            throw DuplicateSymbolError(name);
        } else {
            ASTNode var_type = root.children.at(2);
            std::shared_ptr<NLType> nl_type = visit_type(var_type);

            std::shared_ptr<Variable> variable =
                std::make_shared<Variable>(name);
            result = std::make_shared<TypedVariable>(variable, nl_type);
            symbol_table[name] = result;
        }
    } else {
        std::cerr << "Invalid production found while processing vardef."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}

std::shared_ptr<Code> visit_stmt(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmt);
    std::shared_ptr<Code> result = nullptr;

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
        auto typed_var = visit_vardef(vardef, symbol_table);

        ASTNode expr_node = root.children.at(3);
        TypedExpr expr = visit_expr(expr_node, false, symbol_table);

        if ((*typed_var->nl_type) != (*expr.second)) {
            throw TypeMismatchError(
                "Cannot assign expression of type'" + expr.second->to_string()
                + "' to left hand side of type '"
                + typed_var->nl_type->to_string() + "'."
            );
        }
        result = assign(typed_var->variable, expr.first);
    } else if (prod == std::vector<State> {NonTerminal::stmt, NonTerminal::expr, Terminal::ASSIGN, NonTerminal::expr, Terminal::SEMI}) {
        // extract variable assignment
        ASTNode lhs = root.children.at(0);
        TypedExpr mem_address = visit_expr(lhs, true, symbol_table);

        ASTNode expr = root.children.at(2);
        TypedExpr code = visit_expr(expr, false, symbol_table);

        if ((*mem_address.second) != (*code.second)) {
            throw TypeMismatchError(
                "Cannot assign expression of type'" + code.second->to_string()
                + "' to left hand side of type '"
                + mem_address.second->to_string() + "'."
            );
        }
        result = assign_to_address(mem_address.first, code.first);
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::IF, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE, Terminal::ELSE, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}) {
        // extract if statements
        ASTNode expr = root.children.at(2);
        TypedExpr comp = visit_expr(expr, false, symbol_table);

        ASTNode thens_stmts = root.children.at(5);
        SymbolTable symbol_table_thens = symbol_table;
        auto thens = visit_stmts(thens_stmts, curr_proc, symbol_table_thens);

        ASTNode elses_stmts = root.children.at(9);
        SymbolTable symbol_table_elses = symbol_table;
        auto elses = visit_stmts(elses_stmts, curr_proc, symbol_table_elses);

        if ((*comp.second) != NLTypeBool()) {
            throw TypeMismatchError(
                "If statement condition must result in bool type."
            );
        }
        result = make_if(
            comp.first,
            op::ne_cmp(),
            make_add(Reg::Result, Reg::Zero, Reg::Zero),
            thens,
            elses
        );

    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::WHILE, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN, Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}) {
        // extract while loops
        ASTNode expr = root.children.at(2);
        TypedExpr comp = visit_expr(expr, false, symbol_table);

        ASTNode stmts = root.children.at(5);
        auto code = visit_stmts(stmts, curr_proc, symbol_table);

        if ((*comp.second) != NLTypeBool()) {
            throw TypeMismatchError(
                "While loop statement condition must result in bool type."
            );
        }
        return make_while(
            comp.first,
            op::ne_cmp(),
            make_add(Reg::Result, Reg::Zero, Reg::Zero),
            code
        );
    } else if (prod == std::vector<State> {NonTerminal::stmt, Terminal::RET, NonTerminal::expr, Terminal::SEMI}) {
        // extract return statements
        ASTNode expr_node = root.children.at(1);
        TypedExpr expr = visit_expr(expr_node, false, symbol_table);

        if ((*curr_proc->ret_type) != (*expr.second)) {
            throw TypeMismatchError(
                "Cannot return expression of type '" + expr.second->to_string()
                + "' from function with return type '"
                + curr_proc->ret_type->to_string() + "'."
            );
        }
        result = std::make_shared<RetStmt>(expr.first);
    } else {
        std::cerr << "Invalid production found while processing stmt."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}

std::shared_ptr<Code> visit_stmts(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::stmts);
    std::shared_ptr<Code> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt}) {
        // extract singular statement
        ASTNode stmt = root.children.at(0);
        result = visit_stmt(stmt, curr_proc, symbol_table);
    } else if (prod == std::vector<State> {NonTerminal::stmts, NonTerminal::stmt, NonTerminal::stmts}) {
        // extract first statement
        ASTNode stmt = root.children.at(0);
        std::shared_ptr<Code> code = visit_stmt(stmt, curr_proc, symbol_table);

        // extract rest of statements
        ASTNode stmts = root.children.at(1);
        std::shared_ptr<Code> rest_of_code =
            visit_stmts(stmts, curr_proc, symbol_table);
        result = make_block({code, rest_of_code});
    } else {
        std::cerr << "Invalid production found while processing stmts."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}

std::vector<std::shared_ptr<TypedVariable>>
visit_params(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::params);
    std::vector<std::shared_ptr<TypedVariable>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::params, NonTerminal::vardef}) {
        // extract singular parameter
        ASTNode vardef = root.children.at(0);
        result.push_back(visit_vardef(vardef, symbol_table));
    } else if (prod == std::vector<State> {NonTerminal::params, NonTerminal::vardef, Terminal::COMMA, NonTerminal::params}) {
        // extract first parameter
        ASTNode vardef = root.children.at(0);
        result.push_back(visit_vardef(vardef, symbol_table));

        // extract rest of parameters
        ASTNode params = root.children.at(2);
        std::vector<std::shared_ptr<TypedVariable>> child_result =
            visit_params(params, symbol_table);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing params."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<TypedVariable>>
visit_optparams(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::optparams);
    std::vector<std::shared_ptr<TypedVariable>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::optparams}) {
        // function has no parameters
    } else if (prod == std::vector<State> {NonTerminal::optparams, NonTerminal::params}) {
        // extract parameters
        ASTNode params = root.children.at(0);
        result = visit_params(params, symbol_table);
    } else {
        std::cerr << "Invalid production found while processing optparams."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<TypedProcedure>
visit_fn(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fn);
    std::shared_ptr<TypedProcedure> result;

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
        result = std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // add identifier to symbol table
        symbol_table[name] = result;

        // make clone of symbol table to scope params
        SymbolTable symbol_table_params = symbol_table;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params);

        std::vector<std::shared_ptr<Variable>> params;
        std::vector<std::shared_ptr<NLType>> param_types;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
            param_types.push_back(typed_variable->nl_type);
        }
        result->procedure = std::make_shared<Procedure>(name, params);
        result->param_types = param_types;

        // extract type information
        ASTNode ret_type = root.children.at(6);
        auto nl_type = visit_type(ret_type);
        result->ret_type = nl_type;

        // make clone of symbol table to scope local vars
        SymbolTable symbol_table_locals = symbol_table_params;

        // extract list of statements of procedure
        ASTNode stmts = root.children.at(8);
        std::shared_ptr<Code> code =
            visit_stmts(stmts, result, symbol_table_locals);

        // only include local variables in variable chunk
        std::vector<std::shared_ptr<Variable>> vars;
        for (auto typed_id :
             symbol_table_sub(symbol_table_locals, symbol_table_params)) {
            if (auto typed_variable =
                    std::dynamic_pointer_cast<TypedVariable>(typed_id)) {
                vars.push_back(typed_variable->variable);
            }
        }
        result->procedure->code = make_scope(vars, code);
    } else {
        std::cerr << "Invalid production found while processing fn."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}

std::vector<std::shared_ptr<TypedProcedure>>
visit_fns(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fns);
    std::vector<std::shared_ptr<TypedProcedure>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn}) {
        // extract singular function
        ASTNode fn = root.children.at(0);
        result.push_back(visit_fn(fn, symbol_table));
    } else if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn, NonTerminal::fns}) {
        // extract first function
        ASTNode fn = root.children.at(0);
        result.push_back(visit_fn(fn, symbol_table));

        // extract rest of functions
        ASTNode fns = root.children.at(1);
        std::vector<std::shared_ptr<TypedProcedure>> child_result =
            visit_fns(fns, symbol_table);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing fns."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<TypedProcedure>>
visit_s(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::s);
    std::vector<std::shared_ptr<TypedProcedure>> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::s,
            Terminal::BOFS,
            NonTerminal::fns,
            Terminal::EOFS}) {
        // extract functions of program
        ASTNode fns = root.children.at(1);
        result = visit_fns(fns, symbol_table);
    } else {
        std::cerr << "Invalid production found while processing s."
                  << std::endl;
        exit(1);
    };

    return result;
}

std::vector<std::shared_ptr<TypedProcedure>> generate(ASTNode root) {
    std::vector<std::shared_ptr<TypedProcedure>> result;

    SymbolTable symbol_table;
    result = visit_s(root, symbol_table);

    return result;
}
