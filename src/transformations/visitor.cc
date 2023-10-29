
#include "visitor.h"

#include "beq_label.h"
#include "block.h"
#include "bne_label.h"
#include "call.h"
#include "define_label.h"
#include "if_stmt.h"
#include "ret_stmt.h"
#include "scope.h"
#include "use_label.h"
#include "var_access.h"
#include "word.h"

template<>
void Visitor<void>::visit(std::shared_ptr<Block> code) {
    for (auto c : code->code) {
        c->accept(*this);
    }
}

template<>
void Visitor<void>::visit(std::shared_ptr<Word> code) {
    visit(static_pointer_cast<Code>(code));
}

template<>
void Visitor<void>::visit(std::shared_ptr<BeqLabel> code) {
    visit(static_pointer_cast<Code>(code));
}

template<>
void Visitor<void>::visit(std::shared_ptr<BneLabel> code) {
    visit(static_pointer_cast<Code>(code));
}

template<>
void Visitor<void>::visit(std::shared_ptr<DefineLabel> code) {
    visit(static_pointer_cast<Code>(code));
}

template<>
void Visitor<void>::visit(std::shared_ptr<UseLabel> code) {
    visit(static_pointer_cast<Code>(code));
}

template<>
void Visitor<void>::visit(std::shared_ptr<VarAccess> code) {
    visit(static_pointer_cast<Code>(code));
}

template<>
void Visitor<void>::visit(std::shared_ptr<Scope> code) {
    code->code->accept(*this);
}

template<>
void Visitor<void>::visit(std::shared_ptr<IfStmt> code) {
    code->e1->accept(*this);
    code->comp->accept(*this);
    code->e2->accept(*this);
    code->thens->accept(*this);
    code->elses->accept(*this);
}

template<>
void Visitor<void>::visit(std::shared_ptr<RetStmt> code) {
    code->code->accept(*this);
}

template<>
void Visitor<void>::visit(std::shared_ptr<Call> code) {
    for (auto arg : code->arguments) {
        arg->accept(*this);
    }
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<Block> code) {
    std::vector<std::shared_ptr<Code>> result;
    for (auto c : code->code) {
        result.push_back(c->accept(*this));
    }
    return make_block(result);
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<Word> code) {
    return visit(static_pointer_cast<Code>(code));
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<BeqLabel> code) {
    return visit(static_pointer_cast<Code>(code));
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<BneLabel> code) {
    return visit(static_pointer_cast<Code>(code));
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<DefineLabel> code) {
    return visit(static_pointer_cast<Code>(code));
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<UseLabel> code) {
    return visit(static_pointer_cast<Code>(code));
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<VarAccess> code) {
    return visit(static_pointer_cast<Code>(code));
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<Scope> code) {
    return make_scope(code->variables, code->code->accept(*this));
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<IfStmt> code) {
    return std::make_shared<IfStmt>(
        code->else_label,
        code->e1->accept(*this),
        code->comp->accept(*this),
        code->e2->accept(*this),
        code->thens->accept(*this),
        code->elses->accept(*this)
    );
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<RetStmt> code) {
    return std::make_shared<RetStmt>(code->code->accept(*this));
}

template<>
std::shared_ptr<Code>
Visitor<std::shared_ptr<Code>>::visit(std::shared_ptr<Call> code) {
    std::vector<std::shared_ptr<Code>> arguments;
    for (auto arg : code->arguments) {
        arguments.push_back(arg->accept(*this));
    }
    return std::make_shared<Call>(code->procedure, arguments);
}
