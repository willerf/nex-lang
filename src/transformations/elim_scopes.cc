
#include "elim_scopes.h"

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
