
#include "scope.h"

#include "block.h"

Scope::Scope(
    std::vector<std::shared_ptr<Variable>> variables,
    std::shared_ptr<Code> code
) :
    variables {variables},
    code {code} {}

std::shared_ptr<Code> make_scope(
    std::vector<std::shared_ptr<Variable>> variables,
    std::shared_ptr<Code> code
) {
    return std::make_shared<Scope>(variables, code);
}

std::shared_ptr<Code> make_scope(
    std::vector<std::shared_ptr<Variable>> variables,
    std::vector<std::shared_ptr<Code>> code
) {
    return make_scope(variables, make_block(code));
}
