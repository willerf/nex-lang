
#include "print.h"

#include <bitset>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include "reg.h"

void Print::visit(std::shared_ptr<Code> code) {
    auto& c = *code.get();
    std::cout << std::string(depth, ' ') << "Err: " << typeid(c).name()
              << std::endl;
}

void Print::visit(std::shared_ptr<Block> block) {
    std::cout << std::string(depth, ' ') << "Block(" << std::endl;
    depth += 4;
    for (auto code : block->code) {
        code->accept(*this);
    }
    depth -= 4;
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

void Print::visit(std::shared_ptr<Word> word) {
    std::cout << std::string(depth, ' ') << "Word("
              << std::bitset<32>(word->bits) << ")" << std::endl;
}

void Print::visit(std::shared_ptr<BeqLabel> beq_label) {
    std::cout << std::string(depth, ' ') << "BeqLabel(" << beq_label->s << ", "
              << beq_label->t << ", " << beq_label->label->name << ")"
              << std::endl;
}

void Print::visit(std::shared_ptr<BneLabel> bne_label) {
    std::cout << std::string(depth, ' ') << "BneLabel(" << bne_label->s << ", "
              << bne_label->t << ", " << bne_label->label->name << ")"
              << std::endl;
}

void Print::visit(std::shared_ptr<DefineLabel> define_label) {
    std::cout << std::string(depth, ' ') << "DefineLabel("
              << define_label->label->name << ")" << std::endl;
}

void Print::visit(std::shared_ptr<UseLabel> use_label) {
    std::cout << std::string(depth, ' ') << "UseLabel("
              << use_label->label->name << ")" << std::endl;
}

void Print::visit(std::shared_ptr<VarAccess> var_access) {
    std::cout << std::string(depth, ' ') << "VarAccess(" << var_access->reg
              << ", " << var_access->variable->name << ")" << std::endl;
}

void Print::visit(std::shared_ptr<Scope> scope) {
    std::cout << std::string(depth, ' ') << "Scope(" << std::endl;
    depth += 4;
    scope->code->accept(*this);
    depth -= 4;
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

void Print::visit(std::shared_ptr<IfStmt> if_stmt) {
    std::cout << std::string(depth, ' ') << "IfStmt(" << std::endl;
    depth += 4;
    if_stmt->e1->accept(*this);
    if_stmt->comp->accept(*this);
    if_stmt->e2->accept(*this);
    if_stmt->thens->accept(*this);
    if_stmt->elses->accept(*this);
    depth -= 4;
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}
