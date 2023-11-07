
#include "typed_identifiers.h"

TypedVariable::TypedVariable(
    std::shared_ptr<Variable> variable,
    std::shared_ptr<NLType> nl_type
) :
    variable {variable},
    nl_type {nl_type} {}

TypedProcedure::TypedProcedure(
    std::shared_ptr<Procedure> procedure,
    std::shared_ptr<NLType> ret_type,
    std::vector<std::shared_ptr<NLType>> param_types
) :
    procedure {procedure},
    ret_type {ret_type},
    param_types {param_types} {}
