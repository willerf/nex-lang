
#include "elim_scopes.h"

std::shared_ptr<Code> ElimScopes::visit(std::shared_ptr<Code> code) {
    return code;
}

std::shared_ptr<Code> ElimScopes::visit(std::shared_ptr<Block> block) {
    std::vector<std::shared_ptr<Code>> result;
    for (auto c : block->code) {
        result.push_back(c->accept(*this));
    }
    return make_block(result);
}

std::shared_ptr<Code> ElimScopes::visit(std::shared_ptr<Scope> scope) {
    variables.insert(
        variables.end(),
        scope->variables.begin(),
        scope->variables.end()
    );
    return scope->code->accept(*this);
}

std::vector<std::shared_ptr<Variable>> ElimScopes::get() {
    return variables;
}
