
#include "typed_variable.h"

TypedVariable::TypedVariable(
    std::shared_ptr<Variable> variable,
    std::shared_ptr<NLType> nl_type
) :
    variable {variable},
    nl_type {nl_type} {}


