
#include "visit_expr.h"

#include <stdint.h>
#include <stdlib.h>
#include <cassert>
#include <set>
#include <iostream>
#include <string>
#include <variant>

#include "assembly.h"
#include "bin_op.h"
#include "block.h"
#include "call.h"
#include "define_label.h"
#include "nl_type_bool.h"
#include "nl_type_char.h"
#include "nl_type_i32.h"
#include "nl_type_ptr.h"
#include "operators.h"
#include "pseudo_assembly.h"
#include "symbol_not_found_error.h"
#include "type_mismatch_error.h"
#include "use_label.h"
#include "visit_args.h"
#include "visit_type.h"
#include "word.h"
#include "ast_node.h"
#include "compile_error.h"
#include "label.h"
#include "nl_type.h"
#include "reg.h"
#include "state.h"
#include "typed_procedure.h"
#include "typed_variable.h"

struct Code;

static std::set<NonTerminal> expr_non_terminals = {
    NonTerminal::expr,
    NonTerminal::exprp1,
    NonTerminal::exprp2,
    NonTerminal::exprp3,
    NonTerminal::exprp4,
    NonTerminal::exprp5,
    NonTerminal::exprp6,
    NonTerminal::exprp7,
    NonTerminal::exprp8,
    NonTerminal::exprp9};

TypedExpr visit_expr(
    ASTNode root,
    bool read_address,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(
        std::holds_alternative<NonTerminal>(root.state)
        && expr_non_terminals.count(std::get<NonTerminal>(root.state))
    );
    TypedExpr result = TypedExpr {nullptr, nullptr};

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::ID}) {
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
                throw SymbolNotFoundError(name, id.line_no);
            }
        } else {
            throw SymbolNotFoundError(name, id.line_no);
        }
    } else if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::NUM}) {
        ASTNode num = root.children.at(0);
        result = TypedExpr {
            int_literal(stoi(num.lexeme)),
            std::make_shared<NLTypeI32>()};
    } else if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::TRUE}) {
        ASTNode expr = root.children.at(0);
        result = TypedExpr {int_literal(1), std::make_shared<NLTypeBool>()};
    } else if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::FALSE}) {
        ASTNode num = root.children.at(0);
        result = TypedExpr {int_literal(0), std::make_shared<NLTypeBool>()};
    } else if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::AMPERSAND, Terminal::ID}) {
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
                throw SymbolNotFoundError(name, id.line_no);
            }
        } else {
            throw SymbolNotFoundError(name, id.line_no);
        }
    } else if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::CHARLITERAL}) {
        ASTNode id = root.children.at(0);
        std::string letter_str = id.lexeme;
        if (letter_str.length() == 3) {
            result = TypedExpr {
                int_literal(static_cast<uint32_t>(letter_str[1])),
                std::make_shared<NLTypeChar>()};
        } else {
            std::cerr << "Character literal must be one character."
                      << std::endl;
            exit(1);
        }
    } else if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::STRLITERAL}) {
        ASTNode id = root.children.at(0);
        std::string str_literal = id.lexeme.substr(1, id.lexeme.length() - 2);
        std::vector<std::shared_ptr<Code>> code_str;
        std::shared_ptr<Label> label = std::make_shared<Label>(str_literal);
        code_str.push_back(make_define(label));
        for (char c : str_literal) {
            code_str.push_back(make_word(static_cast<uint32_t>(c)));
        }
        code_str.push_back(make_word(0));
        static_data.push_back(make_block(code_str));
        result = TypedExpr {
            make_block({make_lis(Reg::Result), make_use(label)}),
            std::make_shared<NLTypePtr>(std::make_shared<NLTypeChar>())};
    } else if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN}) {
        ASTNode expr = root.children.at(1);
        result = visit_expr(expr, read_address, symbol_table, static_data);
    } else if (prod == std::vector<State> {NonTerminal::exprp9, Terminal::ID, Terminal::LPAREN, NonTerminal::optargs, Terminal::RPAREN}) {
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        if (symbol_table.count(name)) {
            if (auto typed_procedure =
                    std::dynamic_pointer_cast<TypedProcedure>(symbol_table[name]
                    )) {
                ASTNode optargs = root.children.at(2);
                std::vector<TypedExpr> typed_args =
                    visit_optargs(optargs, symbol_table, static_data);

                if (typed_args.size() != typed_procedure->param_types.size()) {
                    throw CompileError(
                        "Mismatched number of parameters for function call.",
                        id.line_no
                    );
                }

                for (size_t i = 0; i < typed_args.size(); ++i) {
                    if ((*typed_args.at(i).nl_type)
                        != (*typed_procedure->param_types.at(i))) {
                        throw CompileError(
                            "Type mismatch of parameters for function call.",
                            id.line_no
                        );
                    }
                }

                std::vector<std::shared_ptr<Code>> args;
                for (auto typed_arg : typed_args) {
                    args.push_back(typed_arg.code);
                }
                result = TypedExpr {
                    make_call(typed_procedure->procedure, args),
                    typed_procedure->ret_type};
            } else {
                throw SymbolNotFoundError(name, id.line_no);
            }
        } else {
            throw SymbolNotFoundError(name, id.line_no);
        }
    } else if (prod == std::vector<State> {NonTerminal::exprp8, NonTerminal::exprp8, Terminal::AS, NonTerminal::type}) {
        ASTNode expr = root.children.at(0);
        TypedExpr expr_code =
            visit_expr(expr, read_address, symbol_table, static_data);

        ASTNode type_node = root.children.at(2);
        std::shared_ptr<NLType> nl_type = visit_type(type_node);

        return TypedExpr {expr_code.code, nl_type};
    }
    else if (root.children.size() == 1 && std::holds_alternative<NonTerminal>(root.children.at(0).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(0).state))) {
        // recursively call into next operator precedence layer
        ASTNode expr = root.children.at(0);
        result = visit_expr(expr, read_address, symbol_table, static_data);
    }
    else if (root.children.size() == 2 && std::holds_alternative<Terminal>(root.children.at(0).state) && std::holds_alternative<NonTerminal>(root.children.at(1).state) && expr_non_terminals.count(std::get<NonTerminal>(root.children.at(1).state))) {
        // extract unary operator
        ASTNode lhs_op = root.children.at(0);
        ASTNode expr = root.children.at(1);

        Terminal unary_op = std::get<Terminal>(lhs_op.state);
        TypedExpr expr_code =
            visit_expr(expr, read_address, symbol_table, static_data);
        switch (unary_op) {
            case Terminal::NOT:
                if ((*expr_code.nl_type) != NLTypeBool()) {
                    throw TypeMismatchError(
                        "Boolean operations require operands to be of type bool.",
                        lhs_op.line_no
                    );
                }
                result = TypedExpr {
                    make_block({expr_code.code, op::not_bool()}),
                    expr_code.nl_type};
                break;
            case Terminal::STAR:
                if (auto expr_type =
                        std::dynamic_pointer_cast<NLTypePtr>(expr_code.nl_type
                        )) {
                    result =
                        TypedExpr {deref(expr_code.code), expr_type->nl_type};
                } else {
                    throw TypeMismatchError(
                        "Dereference operations require operand to be of type pointer.",
                        lhs_op.line_no
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

        TypedExpr typed_lhs_code =
            visit_expr(lhs, read_address, symbol_table, static_data);
        Terminal mid_op = std::get<Terminal>(mid.state);
        TypedExpr typed_rhs_code =
            visit_expr(rhs, read_address, symbol_table, static_data);

        std::shared_ptr<Code> lhs_code = typed_lhs_code.code;
        std::shared_ptr<Code> rhs_code = typed_rhs_code.code;

        std::shared_ptr<NLType> lhs_type = typed_lhs_code.nl_type;
        std::shared_ptr<NLType> rhs_type = typed_rhs_code.nl_type;

        std::shared_ptr<NLType> result_type;
        if (mid_op == Terminal::OR || mid_op == Terminal::AND) {
            if ((*lhs_type) != NLTypeBool() || (*rhs_type) != NLTypeBool()) {
                throw TypeMismatchError(
                    "Boolean operations require operands to be of type bool.",
                    mid.line_no
                );
            }
            result_type = std::make_shared<NLTypeBool>();
        } else if (mid_op == Terminal::PLUS || mid_op == Terminal::MINUS || mid_op == Terminal::STAR || mid_op == Terminal::SLASH || mid_op == Terminal::PCT) {
            if ((*lhs_type) != NLTypeI32() || (*rhs_type) != NLTypeI32()) {
                throw TypeMismatchError(
                    "Arithmetic operations require both operands to be of type NLTypeI32.",
                    mid.line_no
                );
            }
            result_type = std::make_shared<NLTypeI32>();
        } else if (mid_op == Terminal::EQ || mid_op == Terminal::NE || mid_op == Terminal::LT || mid_op == Terminal::GT || mid_op == Terminal::LE || mid_op == Terminal::GE) {
            if ((*lhs_type) != (*rhs_type)) {
                throw TypeMismatchError(
                    "Comparison operations require operands to be of the same type.",
                    mid.line_no
                );
            }
            result_type = std::make_shared<NLTypeBool>();
        } else {
            throw TypeMismatchError(
                "Encountered unknown operation during type checking.",
                mid.line_no
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

    assert(result.code);
    assert(result.nl_type);
    return result;
}
