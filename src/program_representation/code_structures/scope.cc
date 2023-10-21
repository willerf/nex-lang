
#include "scope.h"

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
